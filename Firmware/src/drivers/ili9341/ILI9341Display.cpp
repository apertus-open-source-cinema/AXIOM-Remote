#include "ILI9341Display.h"

#include <cstdint>
#include <cstdio>
#include <cstring> // For memcpy if needed, though direct packing is used
#include <hardware/clocks.h>
#include <hardware/dma.h> // Ensure DMA header is included
#include <hardware/gpio.h>
#include <hardware/pio.h>
#include <pico.h>
#include <pico/stdlib.h> // Includes time functions like sleep_ms
#include <pico/time.h>
#include <vector>

#include "pargen.pio.h" // Include the generated PIO header

// --- Global PIO Configuration ---
// Using globals for simplicity in this example. Could be class members.
PIO pio = pio0;
uint sm = 0;     // PIO state machine index
uint offset = 0; // PIO program offset

static constexpr uint32_t TE_WAIT_TIMEOUT_US = 30000;

// --- Init Sequence Data ---
// (Keep the initcmd array as it was)
// const uint8_t initcmd[] = {
//     // clang-format off
//     // 0xEF, 3, 0x03, 0x80, 0x02,
//     // 0xCF, 3, 0x00, 0xC1, 0x30,
//     // 0xED, 4, 0x64, 0x03, 0x12, 0x81,
//     // 0xE8, 3, 0x85, 0x00, 0x78,
//     // 0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
//     // 0xF7, 1, 0x20,
//     // 0xEA, 2, 0x00, 0x00,
//     // ILI9341_PWCTR1, 1, 0x23,             // Power control 1
//     // ILI9341_PWCTR2, 1, 0x10,             // Power control 2
//     // ILI9341_VMCTR1, 2, 0x3e, 0x28,       // VCM control 1
//     // ILI9341_VMCTR2, 1, 0x86,             // VCM control 2
//     // ILI9341_MADCTL, 1, (uint8_t)Orientation::LANDSCAPE_90, // Memory Access Control (Default Portrait BGR)
//     // ILI9341_VSCRSADD, 1, 0x00,           // Vertical scroll zero
//     // ILI9341_PIXFMT, 1, 0x55,             // Pixel format: 16bpp
//     // ILI9341_FRMCTR1, 2, 0x00, 0x18,       // Frame rate control
//     // ILI9341_DFUNCTR, 3, 0x08, 0x82, 0x27, // Display function control
//     // 0x35, 1, 0x00,                       // TEON (Tearing Effect Line ON), Parameter 0x00 for V-Blanking only
//     // 0xF2, 1, 0x00,                       // Enable 3G (seems necessary)
//     // ILI9341_GAMMASET, 1, 0x01,           // Gamma curve selected
//     // ILI9341_GMCTRP1, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
//     // Positive gamma correction
//     // ILI9341_GMCTRN1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
//     // Negative gamma correction
//     // ILI9341_SLPOUT, 0x80,                // Exit Sleep + Delay Marker (120ms)
//     // ILI9341_DISPON, 0x80,                // Display on + Delay Marker (120ms)
//     // 0x00                                 // End of list marker

//     ILI9341_SLPOUT, 0x80,                // Exit Sleep + 120ms delay
//     ILI9341_PWCTR1, 1, 0x23,             // Power control 1
//     ILI9341_PWCTR2, 1, 0x10,             // Power control 2
//     ILI9341_VMCTR1, 2, 0x35, 0x28,       // VCM control 1
//     ILI9341_MADCTL, 1, 0x60,             // Landscape 90°, BGR
//     ILI9341_PIXFMT, 1, 0x55,             // 16-bit RGB565
//     ILI9341_DISPON, 0x80,                // Display on + 120ms delay
//     0x00
//     // clang-format on
// };

const uint8_t initcmd[] = {
    // clang-format off
    // ILI9341_SWRESET, 0x80,                // Software Reset, 120ms delay
    ILI9341_PWCTR1, 1, 0x23,             // Power Control 1, VRH ~4.5V for 3.3V
    ILI9341_PWCTR2, 1, 0x10,             // Power Control 2
    ILI9341_VMCTR1, 2, 0x3E, 0x28,       // VCOM Control 1, tuned for 3.3V
    ILI9341_VMCTR2, 1, 0x86,       // VCOM Control 1, tuned for 3.3V
    // ILI9341_FRMCTR1, 2, 0x00, 0x1E,      // Frame Rate Control, ~60 Hz
    ILI9341_MADCTL, 1, 0x48,             // Memory Access Control, Landscape, BGR (use 0x68 for right-to-left if mirrored)
    ILI9341_PIXFMT, 1, 0x55,             // Pixel Format, 16-bit RGB565
    0x35, 1, 0x00,                       // Tearing Effect ON, V-blanking (remove if no TE pin)
    ILI9341_SLPOUT, 0 | 0x80,         // Sleep Out, 0 args + 120ms delay flag
    ILI9341_DISPON, 0 | 0x80,         // Display ON, 0 args + 120ms delay flag
    0x00                                  // End of list
    // clang-format on
};

// const uint8_t initcmd[] = {
//     // clang-format off

//     // Power Control 1 (VRH = 4.6V)
//     // 0xC0, 1, 0x23,

//     // // Power Control 2 (SAP, BT)
//     // 0xC1, 1, 0x10,

//     // VCOM Control 1
//     0xC5, 2, 0x3E, 0x28,

//     // VCOM Control 2
//     0xC7, 1, 0x86,

//     // Memory Access Control (MADCTL)
//     0x36, 1, 0x28,   // Adjust for orientation (0x48 for Landscape, 0x28 for Portrait, etc.)

//     // Pixel Format Set (16-bit RGB565)
//     0x3A, 1, 0x55,

//     // Frame Rate Control (normal mode, full colors)
//     0xB1, 2, 0x00, 0x18,

//     // Display Function Control
//     0xB6, 3, 0x08, 0x82, 0x27,

//     // Gamma Curve Select
//     0x26, 1, 0x01,

//     // Positive Gamma Correction
//     0xE0, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
//                  0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03,
//                  0x0E, 0x09, 0x00,

//     // Negative Gamma Correction
//     0xE1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
//                  0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C,
//                  0x31, 0x36, 0x0F,

//     // Sleep Out
//     0x11, 0 | 0x80,    // Delay after command

//     // Display ON
//     0x29, 0 | 0x80,    // Delay after command

//     0x00               // End of list

//     // clang-format on
// };

// --- Constructor ---
ILI9341Display::ILI9341Display() :
    pio_initialized(false), currentWidth(ILI9341_TFTWIDTH), currentHeight(ILI9341_TFTHEIGHT),
    tx_dma_chan(dma_claim_unused_channel(true)) // Claim DMA channel in constructor
{
    if (tx_dma_chan < 0) {
        // Use panic or a more robust error handling mechanism
        printf("FATAL: Failed to claim DMA channel!\n");
        // Consider setting an error flag instead of proceeding
    }
}

// --- Destructor ---
ILI9341Display::~ILI9341Display()
{
    if (tx_dma_chan >= 0) {
        // Halt and unclaim DMA channel
        dma_channel_abort(tx_dma_chan);
        dma_channel_unclaim(tx_dma_chan);
    }
    // Disable PIO state machine if it was initialized
    if (pio_initialized && pio_sm_is_claimed(pio, sm)) {
        pio_sm_set_enabled(pio, sm, false);
        pio_remove_program(pio, &pargen_program, offset);
        pio_sm_unclaim(pio, sm);
    }
    // Consider resetting GPIOs if needed
}

// --- Init Function ---
void ILI9341Display::Init()
{
    if (pio_initialized) {
        return;
    }
    if (tx_dma_chan < 0) {
        printf("Error: Cannot initialize display, DMA channel not available.\n");
        return;
    }

    printf("ILI9341 Parallel PIO Init (Using PIO%d, SM%d)\n", pio_get_index(pio), sm);

    // 1. Hardware Reset (Moved to be the first step)
    gpio_init(ILI_PIN_RST);
    gpio_set_dir(ILI_PIN_RST, GPIO_OUT);

    gpio_init(ILI_PIN_TE);
    gpio_set_dir(ILI_PIN_TE, GPIO_IN);
    gpio_pull_down(ILI_PIN_TE); // Add pull-down to TE pin

    // 2. PIO Initialization
    // Claim SM before adding program
    pio_sm_claim(pio, sm);
    offset = pio_add_program(pio, &pargen_program);
    // Use the specific ILI9341 init function from pargen.pio.h
    // It sets up data pins (8 starting at ILI_PIN_DB0) and
    // control pins (4 starting at ILI_PIN_RD) and clock divider.
    // ili9341_program_init(pio, sm, offset, ILI_PIN_DB0, ILI_PIN_RD);
    pargen_program_init(pio, sm, offset, ILI_PIN_DB0, 8, ILI_PIN_RD, 8.0f);
    pio_sm_set_enabled(pio, sm, true);

    // 3. DMA Configuration (Prepare for pixel transfers)
    // Use the class member tx_dma_chan (claimed in constructor)
    // and configure the class member tx_dma_cfg.
    tx_dma_cfg = dma_channel_get_default_config(tx_dma_chan);

    channel_config_set_transfer_data_size(&tx_dma_cfg, DMA_SIZE_16);   // Transfer 16-bit pixels
    channel_config_set_read_increment(&tx_dma_cfg, true);              // Increment read address (source pixmap)
    channel_config_set_write_increment(&tx_dma_cfg, false);            // Don't increment write address (PIO FIFO)
    channel_config_set_dreq(&tx_dma_cfg, pio_get_dreq(pio, sm, true)); // Pace DMA by PIO TX FIFO

    pio_initialized = true;
    printf("PIO and DMA Initialized.\n");

    uint8_t buf[5] = {0};
    ReadBytes(0x09, buf, 5);
    printf("Read Display Status: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

    // 4. Send Software Initialization Commands
    ResetDisplay(); // Perform the reset sequence
    ExecuteInitSequence();

    memset(buf, 0, sizeof(buf)); // Clear buffer before re-reading status
    ReadBytes(0x09, buf, 5);
    printf("Read Display Status: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

    // 5. Set Default Orientation (optional, done in init sequence)
    // ExecuteInitSequence will set an initial orientation.
    // If you want to override it immediately, call SetOrientation here.
    // For now, let ExecuteInitSequence handle the initial setup.
    SetOrientation(Orientation::LANDSCAPE_90); // Example if LANDSCAPE_90 is defined

    printf("ILI9341 Initialization Complete.\n");

    // Set an initial full screen window.
    // The actual drawing will be done by DrawPixmap.
    // ili9341_cmd_caset(pio, sm, offset, 0, 240 - 1);
    // ili9341_cmd_paset(pio, sm, offset, 0, 320 - 1);
}

// --- Raw Screen Fill for Debugging ---
void ILI9341Display::FillScreenRaw(uint16_t color)
{
    if (!pio_initialized) {
        printf("FillScreenRaw: PIO not initialized!\n");
        return;
    }
    printf("ILI9341Display::FillScreenRaw with color 0x%04X\n", color);

    // Set full screen area and prepare for RAMWR
    SetArea(0, 0, currentWidth, currentHeight); // This sends CASET, PASET, and RAMWR

    // We are now in RAMWR mode. Use pargen_data_dma to fill with the specified color.
    // The 'size' parameter for pargen_data_dma is the total byte count.
    uint32_t byte_count = (uint32_t)currentWidth * currentHeight * 2;

    // Ensure any previous DMA transfer is finished (important!)
    // This might be overly cautious if FillScreenRaw is only called during init,
    // but good practice if it could be called at other times.
    if (dma_channel_is_busy(tx_dma_chan)) {
        dma_channel_wait_for_finish_blocking(tx_dma_chan);
    }

    // Call pargen_data_dma with inc = false to repeatedly send the same color.
    // 'color' is a uint16_t, so we pass its address.
    pargen_data_dma(pio, sm, offset, tx_dma_chan, &tx_dma_cfg, &color, byte_count, false);

    printf("ILI9341Display::FillScreenRaw done.\n");
}

// --- Execute Init Sequence ---
// void ILI9341Display::ExecuteInitSequence()
// {
//     printf("Sending Simplified Init Sequence (based on working test)...\n");

//     // Software Reset
//     printf("CMD: SWRESET (0x01)\n");
//     ili9341_cmd(pio, sm, offset, ILI9341_SWRESET);
//     sleep_ms(150);

//     // Sleep Out
//     printf("CMD: SLPOUT (0x11)\n");
//     ili9341_cmd(pio, sm, offset, ILI9341_SLPOUT);
//     sleep_ms(150);

//     // Memory Access Control (MADCTL) - Set to Landscape with MX=1
//     // 0x28 = MY=0, MX=0, MV=1, BGR=1 (Landscape, L->R column)
//     // 0x68 = MY=0, MX=1, MV=1, BGR=1 (Landscape, R->L column, should fix mirroring)
//     uint8_t madctl_val = 0x28;
//     // If Orientation::LANDSCAPE_90 is defined as 0x68 in your .h, you could use:
//     // uint8_t madctl_val = static_cast<uint8_t>(Orientation::LANDSCAPE_90);
//     printf("CMD: MADCTL (0x36), Arg: 0x%02X\n", madctl_val);
//     ili9341_cmd_arg(pio, sm, offset, ILI9341_MADCTL, madctl_val);

//     if (madctl_val & 0x20) { // Check MV bit (bit 5) for landscape
//         currentWidth = ILI9341_TFTHEIGHT; // Width becomes 320
//         currentHeight = ILI9341_TFTWIDTH;  // Height becomes 240
//     } else { // Portrait
//         currentWidth = ILI9341_TFTWIDTH;
//         currentHeight = ILI9341_TFTHEIGHT;
//     }

//     // Pixel Format Set (COLMOD) - Using 0x55 (16-bit/pixel) from working test
//     printf("CMD: COLMOD (0x3A), Arg: 0x55\n");
//     ili9341_cmd_arg(pio, sm, offset, ILI9341_PIXFMT, 0x55); // PIXFMT is 0x3A

//     // Display ON
//     printf("CMD: DISPON (0x29)\n");
//     ili9341_cmd(pio, sm, offset, ILI9341_DISPON);
//     sleep_ms(50); // Small delay after display on

//     printf("Init Sequence Sent.\n");
// }

// void ILI9341Display::ExecuteInitSequence()
// {
//     printf("Sending Simplified Init Sequence (based on working test)...\n");

//     // Software Reset
//     printf("CMD: SWRESET (0x01)\n");
//     ili9341_cmd(pio, sm, offset, ILI9341_SWRESET);
//     sleep_ms(150);

//     // Sleep Out
//     printf("CMD: SLPOUT (0x11)\n");
//     ili9341_cmd(pio, sm, offset, ILI9341_SLPOUT);
//     sleep_ms(150);

//     // Memory Access Control (MADCTL) - Set to Landscape with MX=1
//     // 0x28 = MY=0, MX=0, MV=1, BGR=1 (Landscape, L->R column)
//     // 0x68 = MY=0, MX=1, MV=1, BGR=1 (Landscape, R->L column, should fix mirroring)
//     uint8_t madctl_val = 0x38;
//     // If Orientation::LANDSCAPE_90 is defined as 0x68 in your .h, you could use:
//     // uint8_t madctl_val = static_cast<uint8_t>(Orientation::LANDSCAPE_90);
//     printf("CMD: MADCTL (0x36), Arg: 0x%02X\n", madctl_val);
//     ili9341_cmd_arg(pio, sm, offset, ILI9341_MADCTL, madctl_val);

//     if (madctl_val & 0x20)
//     {                                     // Check MV bit (bit 5) for landscape
//         currentWidth = ILI9341_TFTHEIGHT; // Width becomes 320
//         currentHeight = ILI9341_TFTWIDTH; // Height becomes 240
//     }
//     else
//     { // Portrait
//         currentWidth = ILI9341_TFTWIDTH;
//         currentHeight = ILI9341_TFTHEIGHT;
//     }

//     // Pixel Format Set (COLMOD) - Using 0x55 (16-bit/pixel) from working test
//     printf("CMD: COLMOD (0x3A), Arg: 0x55\n");
//     ili9341_cmd_arg(pio, sm, offset, ILI9341_PIXFMT, 0x55); // PIXFMT is 0x3A

//     printf("CMD: TEON (0x35), Arg: 0x00\n");
//     ili9341_cmd_arg(pio, sm, offset, 0x35, 0x00);

//     SetArea(0, 0, 320, 240); // This sends CASET, PASET, and now RAMWR
//     // Display ON
//     printf("CMD: DISPON (0x29)\n");
//     ili9341_cmd(pio, sm, offset, ILI9341_DISPON);
//     sleep_ms(50); // Small delay after display on

//     printf("Init Sequence Sent.\n");

//     // TEST: Fill screen with RED immediately after init sequence
//     // This bypasses LVGL entirely to check basic display functionality.
//     // printf("Attempting raw screen fill (RED)...\n");
//     // FillScreenRaw(0xF800); // Red
//     // printf("Raw screen fill attempt (RED) finished.\n");
//     // sleep_ms(1000); // Keep it red for a second
// }

void ILI9341Display::ExecuteInitSequence()
{
    uint8_t cmd, num_args_raw;
    const uint8_t* addr = initcmd; // Initialize addr to point to the initcmd array
    std::vector<uint16_t> params_for_write_command;

    // Perform Software Reset before processing the main init command sequence.
    // This is crucial for a clean state. Hardware reset is done in Init() already.
    printf("CMD: SWRESET (0x01)\n");
    WriteCommand(ILI9341_SWRESET);
    sleep_ms(150); // Important delay after SWRESET

    while (true) {
        cmd = *addr++;     // Read command
        if (cmd == 0x00) { // End of list marker
            break;
        }

        num_args_raw = *addr++;                         // Read num_args_raw (which includes delay flag)
        bool delay_after = (num_args_raw & 0x80);       // Check for delay marker
        uint8_t num_actual_args = num_args_raw & ~0x80; // Clear delay marker bit

        printf("CMD: 0x%02X, NumArgs: %d", cmd, num_actual_args);
        if (delay_after) {
            printf(" (Delay After)");
        }

        params_for_write_command.clear();
        if (num_actual_args > 0) {
            printf(", Params: ");
            params_for_write_command.reserve(num_actual_args);
            for (uint8_t i = 0; i < num_actual_args; ++i) {
                uint8_t param_val = *(addr + i); // Read parameter using current addr + offset
                printf("0x%02X ", param_val);
                params_for_write_command.push_back(static_cast<uint16_t>(param_val));

                // Update currentWidth/Height if processing MADCTL
                if (cmd == ILI9341_MADCTL && i == 0) { // MADCTL has 1 parameter
                    if (param_val & 0x20) {            // Check MV bit (bit 5) for landscape
                        currentWidth = ILI9341_TFTHEIGHT;
                        currentHeight = ILI9341_TFTWIDTH;
                    }
                    else { // Portrait
                        currentWidth = ILI9341_TFTWIDTH;
                        currentHeight = ILI9341_TFTHEIGHT;
                    }
                    printf(" (MADCTL processed, W:%d H:%d)", currentWidth, currentHeight);
                }
            }
            addr += num_actual_args; // Advance addr past all parameters for this command
            WriteCommand(cmd, params_for_write_command.data(), num_actual_args);
        }
        else {
            printf("(none)");
            WriteCommand(cmd); // Command with no arguments
        }
        printf("\n");

        if (delay_after) // Check delay_after flag from num_args_raw
        {
            uint16_t delay_ms_val = 5;                            // Default short delay
            if (cmd == ILI9341_SLPOUT || cmd == ILI9341_DISPON) { // These typically need longer delays
                delay_ms_val = 120;
            }
            sleep_ms(delay_ms_val);
            printf("Delay %dms executed.\n", delay_ms_val);
        }
    } // End of while loop

    printf("Initcmd array processing finished.\n");
    // TEON is now part of initcmd

    // The MADCTL is now handled within the loop if present in initcmd.
    // DISPON is also handled by initcmd if present.
    // SetArea will be called by LVGL's flush callback as needed.
    printf("ExecuteInitSequence finished.\n");

    // SetArea(0, 0, 320, 240); // This sends CASET, PASET, and now RAMWR
}

// --- Hardware Reset ---
void ILI9341Display::ResetDisplay()
{
    printf("Hardware Reset...\n");
    gpio_put(ILI_PIN_RST, 0);
    sleep_ms(10); // Short delay low
    gpio_put(ILI_PIN_RST, 1);
    sleep_ms(120); // Longer delay high
    printf("Hardware Reset Complete.\n");
}

void ILI9341Display::WaitForTearingEffect()
{
    if (!pio_initialized)
        return; // Should not happen if called from DrawPixmap

    uint32_t start_us = time_us_32();

    // The TE signal is typically active HIGH during blanking periods (safe to write).
    // 1. Wait for TE to go LOW (display is actively scanning).
    //    This ensures we don't miss the rising edge if we start polling while it's already high.
    while (gpio_get(ILI_PIN_TE)) {
        if (time_us_32() - start_us > TE_WAIT_TIMEOUT_US) {
            printf("TE Timeout waiting for LOW\n");
            return;
        }
        tight_loop_contents(); // Essential for performance in tight loops
    }

    // 2. Wait for TE to go HIGH (start of V-Blanking, safe to write new frame).
    while (!gpio_get(ILI_PIN_TE)) {
        if (time_us_32() - start_us > TE_WAIT_TIMEOUT_US) {
            printf("TE Timeout waiting for HIGH\n");
            return;
        }
        tight_loop_contents();
    }
}

// --- Internal Read Function ---
void ILI9341Display::ReadBytes(uint8_t command, uint8_t* buffer, size_t len)
{
    if (!pio_initialized || len == 0 || !buffer) {
        return;
    }
    // Allocate buffer for PIO read (reads in 32-bit chunks)
    size_t num_words = (len + 3) / 4;
    std::vector<uint16_t> read_data(num_words);

    printf("Reading %d bytes for CMD: 0x%02X\n", len, command);
    pargen_cmd_rdat(pio, sm, offset, command, read_data.data(), len);

    // Unpack the 32-bit words into the 8-bit buffer
    for (size_t i = 0; i < len; ++i) {
        buffer[i] = (read_data[i / 4] >> (8 * (i % 4))) & 0xFF;
    }
    printf("Read data: ");
    for (size_t i = 0; i < len; ++i)
        printf("0x%02X ", buffer[i]);
    printf("\n");
}

// --- Internal Write Command Helper ---
void ILI9341Display::WriteCommand(uint8_t command, const uint16_t* parameters, size_t len)
{
    if (!pio_initialized) {
        return;
    }
    if (len == 0) {
        ili9341_cmd(pio, sm, offset, command);
    }
    else if (len == 1) {
        ili9341_cmd_arg(pio, sm, offset, command, parameters[0]);
    }
    else {
        // Pack parameters into uint32_t array for pargen_cmd_wdat
        // size_t num_words = (len + 3) / 4;
        // std::vector<uint16_t> params(num_words, 0);
        // for (size_t i = 0; i < len; ++i)
        // {
        //     params[i / 4] |= (static_cast<uint32_t>(parameters[i]) << (8 * (i % 4)));
        // }
        pargen_cmd_wdat(pio, sm, offset, command, parameters, len);
    }
}

// --- Set Drawing Area ---
void ILI9341Display::SetArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (!pio_initialized || width == 0 || height == 0) {
        return;
    }
    // Ensure area is within bounds (optional but recommended)
    x = std::min(x, (uint16_t)(currentWidth - 1));
    y = std::min(y, (uint16_t)(currentHeight - 1));
    uint16_t x_end = x + width - 1;
    uint16_t y_end = y + height - 1;
    x_end = std::min(x_end, (uint16_t)(currentWidth - 1));
    y_end = std::min(y_end, (uint16_t)(currentHeight - 1));

    // Use the specific PIO helpers for CASET and PASET
    ili9341_cmd_caset(pio, sm, offset, x, x_end);
    ili9341_cmd_paset(pio, sm, offset, y, y_end);
    // Send RAM Write command to prepare for pixel data
    ili9341_cmd(pio, sm, offset, ILI9341_RAMWR);
}

// --- Set Display Orientation ---
void ILI9341Display::SetOrientation(Orientation orientation)
{
    if (!pio_initialized) {
        return;
    }

    uint16_t data = static_cast<uint16_t>(orientation);
    WriteCommand(ILI9341_MADCTL, &data, 1);

    // Update internal dimensions based on MV (Memory Access Control) bit
    // MV=1 (bit 5) swaps width/height
    if (data & 0x28) { // Check MV bit
        currentWidth = ILI9341_TFTHEIGHT;
        currentHeight = ILI9341_TFTWIDTH;
    }
    else {
        currentWidth = ILI9341_TFTWIDTH;
        currentHeight = ILI9341_TFTHEIGHT;
    }

    printf("Orientation set (MADCTL=0x%02X). Width=%d, Height=%d\n", data, currentWidth, currentHeight);
    // After changing orientation, the drawing area might need resetting
    // SetArea(0, 0, currentWidth, currentHeight); // Optional: Reset to full screen
}

// --- Draw Pixmap using DMA ---
void ILI9341Display::DrawPixmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* pixmap)
{
    // printf("ILI9341Display::DrawPixmap CALLED. x=%u, y=%u, w=%u, h=%u. Pixmap: %p\n", x, y, width, height, (const
    // void*)pixmap); if (!pio_initialized || !pixmap || width == 0 || height == 0 || tx_dma_chan < 0)
    // {
    //     printf("ERROR: DrawPixmap preconditions not met (pio_init=%d, pixmap=%p, w=%d, h=%d, dma=%d)\n",
    //            pio_initialized, (void*)pixmap, width, height, tx_dma_chan);
    //     return;
    // }

    uint32_t num_pixels = (uint32_t)width * height; // Total number of pixels
    // This 'dma_transfer_count' is the total byte count to be passed to pargen_data_dma
    uint32_t dma_transfer_count = num_pixels * 2;

    // 1. Set the drawing area
    // printf("Area: %d,%d %dx%d\n", x, y, width, height);
    SetArea(x, y, width, height); // This sends CASET, PASET, and now RAMWR

    // 2. Start DMA transfer directly
    // DMA transfer count is in units specified by DMA_SIZE (16-bit here, from tx_dma_cfg)

    // Ensure any previous DMA transfer is finished (important!)
    // dma_channel_wait_for_finish_blocking(tx_dma_chan);

    // Configure and start DMA.
    // tx_dma_cfg (set in Init) has DMA_SIZE_16, read_increment=true, write_increment=false, DREQ.
    // dma_channel_configure(
    //     tx_dma_chan,
    //     &tx_dma_cfg,        // Use pre-configured settings
    //     &pio->txf[sm],      // Write target: PIO TX FIFO
    //     (const void*)pixmap,// Read source: Pixel data buffer
    //     dma_transfer_count, // Number of 16-bit transfers
    //     true                // Start immediately
    // );

    // uint32_t draw_start_us = time_us_32();

    // 3. Wait for DMA transfer to complete
    // This is crucial if pargen_data_dma itself is non-blocking.
    // Most PIO DMA helper functions are blocking.
    // printf("ILI9341Display::DrawPixmap: Calling pargen_data_dma. DMA chan: %d, Transfer count (bytes): %lu\n",
    // tx_dma_chan, dma_transfer_count); The 'true' for 'inc' in pargen_data_dma is correct for reading from pixmap.
    pargen_data_dma(pio, sm, offset, tx_dma_chan, &tx_dma_cfg, (const uint16_t*)pixmap, dma_transfer_count, true);
    // printf("ILI9341Display::DrawPixmap: pargen_data_dma FINISHED.\n");

    // Ensure PIO SM has finished sending all data from its TX FIFO.
    // This is critical if pargen_data_dma only waits for DMA to complete loading the FIFO,
    // not for the PIO to empty its FIFO and send all data to the display.
    while (!pio_sm_is_tx_fifo_empty(pio, sm)) {
        tight_loop_contents(); // Use tight_loop_contents for minimal delay while waiting.
                               // This is important for performance-sensitive loops on RP2040.
    }

    // uint32_t draw_duration_us = time_us_32() - draw_start_us;
    // if (draw_duration_us > 1000)
    // { // Log if drawing takes more than 1ms
    //     printf("DrawPixmap duration: %lu us\n", draw_duration_us);
    // }
    // sleep_us(10); // This sleep is likely not needed if pargen_data_dma is blocking.
    // printf("FRAME DONE\n");
}

// --- Update Function ---
void ILI9341Display::Update()
{
    // Placeholder - currently does nothing.
    // Could be used for tasks like checking touch input, etc.
}

// --- Public Read Functions ---
void ILI9341Display::ReadDisplayID(uint8_t* buffer)
{
    // Uses RDDID (0x04), reads 4 bytes (dummy + 3 ID bytes)
    ReadBytes(ILI9341_RDDID, buffer, 4);
}

void ILI9341Display::ReadDisplayStatus(uint8_t* buffer)
{
    // Uses RDDST (0x09), reads 5 bytes (dummy + 4 status bytes)
    ReadBytes(ILI9341_RDDST, buffer, 5);
}

uint32_t ILI9341Display::MeasureTEInterval()
{
    if (!pio_initialized) {
        printf("MeasureTEInterval: PIO not initialized!\n");
        return 0;
    }

    printf("Measuring TE interval...\n");
    uint32_t start_wait_us;
    uint32_t t1_us = 0, t2_us = 0;

    // Wait for the first rising edge of TE
    // 1. Wait for TE to go LOW (if it's already high or to catch the end of a pulse)
    start_wait_us = time_us_32();
    while (gpio_get(ILI_PIN_TE)) {
        if (time_us_32() - start_wait_us > TE_WAIT_TIMEOUT_US * 2) { // Longer timeout for initial sync
            printf("TE Interval: Timeout waiting for first LOW.\n");
            return 0;
        }
        tight_loop_contents();
    }
    // 2. Wait for TE to go HIGH (first pulse start)
    start_wait_us = time_us_32();
    while (!gpio_get(ILI_PIN_TE)) {
        if (time_us_32() - start_wait_us > TE_WAIT_TIMEOUT_US) {
            printf("TE Interval: Timeout waiting for first HIGH.\n");
            return 0;
        }
        tight_loop_contents();
    }
    t1_us = time_us_32();
    printf("TE Pulse 1 detected at %lu us\n", t1_us);

    // Wait for the second rising edge of TE
    // 3. Wait for TE to go LOW (end of the first pulse)
    start_wait_us = time_us_32();
    while (gpio_get(ILI_PIN_TE)) { // Wait for TE to go LOW
        if (time_us_32() - start_wait_us > TE_WAIT_TIMEOUT_US) {
            printf("TE Interval: Timeout waiting for second LOW.\n");
            return 0;
        }
        tight_loop_contents();
    }
    // 4. Wait for TE to go HIGH (second pulse start)
    start_wait_us = time_us_32();
    while (!gpio_get(ILI_PIN_TE)) { // Wait for TE to go HIGH
        if (time_us_32() - start_wait_us > TE_WAIT_TIMEOUT_US) {
            printf("TE Interval: Timeout waiting for second HIGH.\n");
            return 0;
        }
        tight_loop_contents();
    }
    t2_us = time_us_32();
    printf("TE Pulse 2 detected at %lu us\n", t2_us);

    uint32_t interval_us = t2_us - t1_us;
    printf("Measured TE Interval: %lu us (%.2f ms, %.2f Hz)\n", interval_us, (float)interval_us / 1000.0f,
           1000000.0f / interval_us);
    return interval_us;
}
