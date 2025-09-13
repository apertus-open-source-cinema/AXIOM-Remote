#pragma once

#include "ILvglPlatformDriver.h"
#include "lvgl.h" // Include base LVGL header

namespace Rp2040Config {
constexpr int DisplayWidth = 240;
constexpr int DisplayHeight = 320;
// Example for LV_COLOR_DEPTH 16 (RGB565)
using DrawBufferColorFormat = lv_color16_t;
// Use a smaller buffer for partial rendering, e.g., 1/10th of the screen height
constexpr size_t DrawBufLines = DisplayHeight / 4;          // Adjust as needed
constexpr size_t DrawBufSize = DisplayWidth * DrawBufLines; // Size in pixels
// Define Hardware Pins & SPI/I2C settings here... (Already done in
// ILI9341Display.h)
} // namespace Rp2040Config

class Rp2040HALDriver : public ILvglPlatformDriver {
public:
  Rp2040HALDriver();
  ~Rp2040HALDriver() override;

  // --- Interface Implementation ---
  bool Initialize() override;
  lv_coord_t GetHorRes() const override { return Rp2040Config::DisplayWidth; }
  lv_coord_t GetVerRes() const override { return Rp2040Config::DisplayHeight; }
  lv_display_t *GetLvDisplay() const override { return m_lvglDisplay; }

  // --- ADD THIS OVERRIDE ---
  /**
   * @brief Gets the primary LVGL input device (returns nullptr as input is not
   * implemented).
   * @return Pointer to the lv_indev_t object (nullptr).
   */
  lv_indev_t *GetLvInputDevice() const override {
    return m_lvglInputDevice;
  } // Or return nullptr directly if m_lvglInputDevice is not added yet

  // Non-copyable/movable
  Rp2040HALDriver(const Rp2040HALDriver &) = delete;
  Rp2040HALDriver &operator=(const Rp2040HALDriver &) = delete;
  Rp2040HALDriver(Rp2040HALDriver &&) = delete;
  Rp2040HALDriver &operator=(Rp2040HALDriver &&) = delete;

private:
  // Static buffers for LVGL draw operations (must be static or global for DMA)
  static Rp2040Config::DrawBufferColorFormat
      s_drawBuf1[Rp2040Config::DrawBufSize];
  static Rp2040Config::DrawBufferColorFormat
      s_drawBuf2[Rp2040Config::DrawBufSize];

  // LVGL display driver structure (managed internally)
  lv_display_t *m_lvglDisplay = nullptr;
  lv_indev_t *m_lvglInputDevice =
      nullptr; // Add member to hold the input device (even if null for now)

  // Helper methods for hardware initialization
  bool InitializeDisplayHardware();
  bool InitializeInputHardware(); // Placeholder for touch/buttons

  // Static Callbacks (Trampolines to access the global display driver instance)
  static void FlushCallbackStatic(lv_display_t *pDisp, const lv_area_t *pArea,
                                  uint8_t *pixmap);
  static void InputReadCallbackStatic(lv_indev_t *pIndev,
                                      lv_indev_data_t *pData); // Placeholder
};
