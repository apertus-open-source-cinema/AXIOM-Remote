#pragma once

#include <ILvglPlatformDriver.h>

#include <cstdint>
#include <vector>

// Forward declare OpenGL type or include header
using GLuint = unsigned int;
// #include <glad/gl.h> // Or your preferred GL header

// --- Configuration ---
namespace DesktopConfig {
constexpr int DefaultFbWidth = 320;
constexpr int DefaultFbHeight = 240;
constexpr int FbBpp = 4;                     // RGBA8888 assumed
using FramebufferColorFormat = lv_color16_t; // Match LV_COLOR_DEPTH 32
constexpr size_t DrawBufSize = DefaultFbWidth * DefaultFbHeight;
} // namespace DesktopConfig

class DesktopGLDriver : public ILvglPlatformDriver {
public:
  DesktopGLDriver(int width = DesktopConfig::DefaultFbWidth,
                  int height = DesktopConfig::DefaultFbHeight);
  ~DesktopGLDriver() override;

  // --- Interface Implementation ---
  bool Initialize() override;
  lv_coord_t GetHorRes() const override { return m_fbWidth; }
  lv_coord_t GetVerRes() const override { return m_fbHeight; }

  // --- Desktop-Specific Methods ---
  GLuint GetTextureId() const;

  // Renamed base method for clarity if needed
  // void UpdateDisplayContext() override { /* Default if no params needed */ }

  // Non-copyable/movable
  DesktopGLDriver(const DesktopGLDriver &) = delete;
  DesktopGLDriver &operator=(const DesktopGLDriver &) = delete;
  DesktopGLDriver(DesktopGLDriver &&) = delete;
  DesktopGLDriver &operator=(DesktopGLDriver &&) = delete;

  static void FlushCallbackStatic(lv_display_t *pDrv, const lv_area_t *pArea,
                                  uint8_t *pixmap);
  static DesktopConfig::FramebufferColorFormat
      s_drawBuf1[DesktopConfig::DrawBufSize];

  lv_display_t *GetLvDisplay() const override { return nullptr; }

  lv_indev_t *GetLvInputDevice() const override { return nullptr; };

private:
  int m_fbWidth;
  int m_fbHeight;
  GLuint m_glTextureId = 0;
  std::vector<uint8_t> m_framebuffer; // RAM buffer for pixels

// Static buffers for LVGL draw operations (using s_ prefix)
#ifdef LV_USE_DOUBLE_BUFFER
  static DesktopConfig::FramebufferColorFormat
      s_drawBuf2[DesktopConfig::DrawBufSize];
#endif
  lv_draw_buf_t m_dispDrawBuf{}; // Descriptor for the draw buffers

  // Helper methods for resource management
  bool InitializeFramebuffer();
  bool InitializeTexture(); // Requires active GL context

  // Static Callbacks (Trampolines to instance methods)
  static void InputReadCallbackStatic(lv_indev_t *pDrv, lv_indev_data_t *pData);

  // Instance implementations of the callbacks
  void FlushCallbackImpl(const lv_area_t *pArea, uint8_t *pColorP);
  void InputReadCallbackImpl(lv_indev_data_t *pData);
};