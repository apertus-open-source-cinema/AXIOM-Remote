#pragma once

#include <cstdint>
#include <vector>
#include <hardware/dma.h> // Include DMA header

// Include the generated PIO header (implicitly included by cpp file)
// #include "pargen.pio.h" // Not strictly needed here if only used in cpp

#include "ili_def.h" // Keep your ILI9341 command definitions

// --- Pin Definitions ---
// Ensure these match your actual hardware wiring and the assumptions
// made by the ili9341_program_init function (data_base, ctrl_base)
// ctrl_base pins are assumed consecutive: RD, WR, DC, CS
#define ILI_PIN_RST 0
#define ILI_PIN_TE 1
#define ILI_PIN_DB0 2 // Base data pin (D0-D7)
#define ILI_PIN_RD 10 // Base control pin (RD=ctrl_base+0, WR=ctrl_base+1, DC=ctrl_base+2, CS=ctrl_base+3)
// The following are implicitly defined by ILI_PIN_RD being the base for the 4 side-set pins
// #define ILI_PIN_WR  11 // ctrl_base + 1
// #define ILI_PIN_DC  12 // ctrl_base + 2
// #define ILI_PIN_CS  13 // ctrl_base + 3

// --- PIO Configuration ---
// These are now defined globally in the CPP file, but could be members if preferred.
// #define ILI_PIO pio0
// #define ILI_SM 0

// --- Display Dimensions ---
// Defined in ili_def.h presumably

enum class Orientation : uint8_t {
    PORTRAIT          = 0x48, // MY=0, MX=1, MV=0, ML=0, BGR=1
    LANDSCAPE_Y_FLIP  = 0x28, // MY=0, MX=0, MV=1, ML=0, BGR=1 (Standard Landscape)
    LANDSCAPE_X_FLIP  = 0xA8, // MY=1, MX=0, MV=1, ML=0, BGR=1 (Landscape 180) - Check datasheet/experiment
    LANDSCAPE_90      = 0x28, // Alias for standard landscape
    LANDSCAPE_270     = 0xA8, // Alias for landscape 180
    PORTRAIT_INVERTED = 0x88, // MY=1, MX=0, MV=0, ML=0, BGR=1
    // Add other orientations based on MADCTL bits (MY, MX, MV, ML, BGR) if needed
    // The names below might not map directly to the MADCTL values provided.
    // Double-check the MADCTL register description for ILI9341.
    // LANDSCAPE_90_INVERTED = 0x60, // Example, verify
    // LANDSCAPE_270_INVERTED = 0x90  // Example, verify
};

class ILI9341Display {
  private:
    bool pio_initialized   = false;
    uint16_t currentWidth  = ILI9341_TFTWIDTH;
    uint16_t currentHeight = ILI9341_TFTHEIGHT;

    // DMA channel and configuration for pixel transfers
    int tx_dma_chan = -1; // Use int, standard practice for Pico SDK channel numbers
    dma_channel_config tx_dma_cfg;

    // Internal helper functions using pargen.pio wrappers
    void WriteCommand(uint8_t command, const uint16_t* parameters = nullptr, size_t len = 0);
    void ReadBytes(uint8_t command, uint8_t* buffer, size_t len);
    void ResetDisplay();
    void ExecuteInitSequence();

    // WriteData/WriteData16 are less relevant now as DrawPixmap uses DMA directly
    // void WriteData(const uint8_t* buff, size_t buff_size); // Not typically used with DMA approach
    // void WriteData16(uint16_t data); // Not typically used with DMA approach
    void FillScreenRaw(uint16_t color);

  public:
    ILI9341Display();
    ~ILI9341Display();

    void WaitForTearingEffect();

    /**
     * @brief Initializes the PIO, DMA, and the ILI9341 display controller.
     * Must be called before any other display operations.
     */
    void Init();

    /**
     * @brief Sets the rectangular area for subsequent drawing operations (like DrawPixmap).
     * Sends CASET, PASET, and RAMWR commands.
     * @param x Starting column.
     * @param y Starting row (page).
     * @param width Width of the area.
     * @param height Height of the area.
     */
    void SetArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    /**
     * @brief Sets the display orientation and updates internal width/height.
     * @param orientation The desired display orientation.
     */
    void SetOrientation(Orientation orientation);

    /**
     * @brief Draws a pixmap (array of 16-bit colors) to the display using DMA.
     * Assumes SetArea has been called previously or calls it internally.
     * The pixmap data must persist until the DMA transfer is complete.
     * @param x Starting column.
     * @param y Starting row (page).
     * @param width Width of the pixmap.
     * @param height Height of the pixmap.
     * @param pixmap Pointer to the 16-bit (RGB565) pixel data.
     */
    void DrawPixmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* pixmap);

    /**
     * @brief Placeholder for future use (e.g., periodic tasks). Currently does nothing.
     */
    void Update();

    // --- Optional Public Read Functions ---
    /**
     * @brief Reads the 4-byte device ID.
     * @param buffer Pointer to a 4-byte buffer to store the ID.
     */
    void ReadDisplayID(uint8_t* buffer);

    /**
     * @brief Reads the 5-byte device status.
     * @param buffer Pointer to a 5-byte buffer to store the status.
     */
    void ReadDisplayStatus(uint8_t* buffer);

    /**
     * @brief Measures and prints the time interval between two consecutive TE pulses.
     * @return The measured interval in microseconds, or 0 if a timeout occurred.
     */
    uint32_t MeasureTEInterval();

    bool IsBusy()
    {
        return dma_channel_is_busy(tx_dma_chan);
    }
};
