// DesktopGLDriver.cpp

#include "DesktopGLDriver.h" // Should be "DesktopGLDriver.hpp" if using .hpp extension

#include <SDL3/SDL.h> // For logging, input
#include <algorithm>  // std::max, std::min
#include <cstring>    // std::memcpy, std::memset
#include <stdexcept>  // std::runtime_error, std::bad_alloc

// Include GL header here for implementation details
#include <glad/gl.h> // Or your preferred GL header

// Define static members
DesktopConfig::FramebufferColorFormat
    DesktopGLDriver::s_drawBuf1[DesktopConfig::DrawBufSize];
#ifdef LV_USE_DOUBLE_BUFFER
DesktopConfig::FramebufferColorFormat
    DesktopGLDriver::s_drawBuf2[DesktopConfig::DrawBufSize];
#endif

DesktopGLDriver::DesktopGLDriver(int width, int height)
    : m_fbWidth(width), m_fbHeight(height)
{
  if (!InitializeFramebuffer())
  {
    // Constructor throws, RAII ensures member destructors are called if needed
    throw std::runtime_error("Failed to initialize desktop framebuffer");
  }
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
              "DesktopGLDriver constructed (Framebuffer allocated).");
}

DesktopGLDriver::~DesktopGLDriver()
{
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "DesktopGLDriver destroyed.");
  if (m_glTextureId != 0)
  {
    // Ensure GL context is active here or use a separate cleanup method managed by main
    glDeleteTextures(1, &m_glTextureId);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "OpenGL Texture deleted.");
  }
}

// Initialize driver-specific resources (Texture needs GL context active)
bool DesktopGLDriver::Initialize()
{
  // Initialize OpenGL Texture (Requires active GL context)
  if (!InitializeTexture())
  {
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "Failed to initialize OpenGL texture in DesktopGLDriver::Initialize.");
    return false;
  }

  // Create Input Device
  // m_lvglInputDevice = lv_indev_create();
  // lv_indev_set_type(m_lvglInputDevice, LV_INDEV_TYPE_ENCODER);
  // lv_indev_set_read_cb(m_lvglInputDevice, InputReadCallbackStatic);
  // lv_indev_set_user_data(m_lvglInputDevice, this);
  // // Important: Add input device to the default group so it controls focused objects!
  // lv_group_t* g = lv_group_get_default();
  // if (g) {
  //     lv_indev_set_group(m_lvglInputDevice, g);
  // } else {
  //     SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "No default group found when initializing input device!");
  // }

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
              "DesktopGLDriver Initialized (Texture & Input ready).");
  return true;
}

GLuint DesktopGLDriver::GetTextureId() const { return m_glTextureId; }

// --- Helper Implementations ---
bool DesktopGLDriver::InitializeFramebuffer()
{
  try
  {
    // Resize using the correct bytes per pixel (2 for RGB565)
    m_framebuffer.resize(static_cast<size_t>(m_fbWidth) * m_fbHeight * DesktopConfig::FbBpp);
    std::memset(m_framebuffer.data(), 0, m_framebuffer.size());
    return true;
  }
  catch (const std::bad_alloc &e)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Framebuffer allocation failed: %s", e.what());
    return false;
  }
}

bool DesktopGLDriver::InitializeTexture()
{
  if (m_glTextureId != 0)
    return true; // Already initialized

  glGenTextures(1, &m_glTextureId);
  if (m_glTextureId == 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "glGenTextures failed.");
    return false;
  }
  glBindTexture(GL_TEXTURE_2D, m_glTextureId);
  // Use NEAREST filter for pixel-perfect rendering, or LINEAR for smoother scaling
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Allocate texture storage using correct RGB565 formats
  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_RGB565, // internalformat = GL_RGB565
               m_fbWidth, m_fbHeight, 0,
               GL_RGB,   // format = GL_RGB
               GL_UNSIGNED_SHORT_5_6_5,     // type = GL_UNSIGNED_SHORT_5_6_5
               nullptr);                        // No initial data
  glBindTexture(GL_TEXTURE_2D, 0);              // Unbind

  bool success = (glGetError() == GL_NO_ERROR);
  if (success)
  {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "OpenGL Texture Initialized (ID: %u).", m_glTextureId);
  }
  else
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "OpenGL error during texture initialization.");
    glDeleteTextures(1, &m_glTextureId); // Clean up failed texture
    m_glTextureId = 0;
  }
  return success;
}

// --- Static Callback Implementations ---

/* static */ void DesktopGLDriver::FlushCallbackStatic(lv_display_t *display,
                                                       const lv_area_t *area,
                                                       uint8_t *pixmap)
{
  // Get the driver instance from the display's user data (set in main)
  auto *instance = static_cast<DesktopGLDriver *>(lv_display_get_user_data(display));
  if (instance)
  {
    // Call the instance method to perform the flush logic
    instance->FlushCallbackImpl(area, pixmap);
  }
  // Inform LVGL that flushing is complete for this area (CRITICAL!)
  lv_display_flush_ready(display);
}

/* static */ void
DesktopGLDriver::InputReadCallbackStatic(lv_indev_t *inputDevice,
                                         lv_indev_data_t *inputData)
{
    auto *instance = static_cast<DesktopGLDriver *>(lv_indev_get_user_data(inputDevice));
    if (instance)
    {
        instance->InputReadCallbackImpl(inputData);
    }
}

void DesktopGLDriver::InputReadCallbackImpl(lv_indev_data_t *data)
{
    data->enc_diff = m_encDiff;
    data->state = m_isEncPressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    
    // Clear the accumulated diff after reading, or just reset it if it was consumed?
    // Usually, we accumulate until read, then clear.
    m_encDiff = 0;
}

void DesktopGLDriver::UpdateInputState(ButtonID btn, ButtonState state, int16_t knobDiff)
{
    // Accumulate knob rotation
    m_encDiff += knobDiff;

    // Track button press state
    if (btn == ButtonID::KNOB) {
        m_isEncPressed = (state == ButtonState::Pressed);
    }
}

// --- Instance Callback Implementations ---

// void DesktopGLDriver::FlushCallbackImpl(const lv_area_t *areaToFlush,
//                                         uint8_t *pixmap)
// {
//   // Check if resources are ready
//   if (m_framebuffer.empty() || m_glTextureId == 0)
//   {
//       SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Flush callback called before resources ready.");
//       return;
//   }

//   // --- Step 1: Copy pixel data from LVGL buffer (pixmap) to intermediate RAM framebuffer (m_framebuffer) ---
//   int32_t w = lv_area_get_width(areaToFlush);
//   int32_t h = lv_area_get_height(areaToFlush);

//   // Calculate destination pointer in the byte buffer using bytes-per-pixel (FbBpp = 2)
//   uint8_t* destinationBuffer = m_framebuffer.data() +
//       (areaToFlush->y1 * m_fbWidth + areaToFlush->x1) * DesktopConfig::FbBpp;
//   // Source buffer pointer provided by LVGL
//   uint8_t* sourceBuffer = pixmap; // Already uint8_t* in v9

//   size_t destinationStrideBytes = static_cast<size_t>(m_fbWidth) * DesktopConfig::FbBpp;
//   size_t sourceStrideBytes = static_cast<size_t>(w) * DesktopConfig::FbBpp; // Source stride based on area width

//   // Copy data row by row if strides differ, otherwise copy as a block
//   if (sourceStrideBytes == destinationStrideBytes && w == m_fbWidth)
//   {
//     // If area width matches framebuffer width, copy the whole block
//     memcpy(destinationBuffer, sourceBuffer, sourceStrideBytes * h);
//   }
//   else
//   {
//     // If area width is smaller (partial update), copy row by row
//     for (int32_t y = 0; y < h; ++y)
//     {
//       memcpy(destinationBuffer + y * destinationStrideBytes, // Advance dest by full width stride
//              sourceBuffer + y * sourceStrideBytes,          // Advance src by area width stride
//              sourceStrideBytes);                            // Copy bytes for one row of the area
//     }
//   }

//   // --- Step 2: Upload the updated region from RAM framebuffer to OpenGL Texture ---
//   glBindTexture(GL_TEXTURE_2D, m_glTextureId);
//   // Set pixel store alignment appropriate for tightly packed uint16_t data
//   glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Often 1 or 2 for RGB565, 1 is safest

//   // Use glTexSubImage2D for efficient partial updates
//   glTexSubImage2D(GL_TEXTURE_2D, 0,
//                   areaToFlush->x1, areaToFlush->y1, w, h,
//                   GL_RGB,    // format = GL_RGB
//                   GL_UNSIGNED_SHORT_5_6_5,      // type = GL_UNSIGNED_SHORT_5_6_5
//                   // Pointer to the start of the *sub-region* in our intermediate buffer
//                   m_framebuffer.data() + (areaToFlush->y1 * m_fbWidth + areaToFlush->x1) * DesktopConfig::FbBpp);

//   glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Restore default OpenGL alignment (common practice)
//   glBindTexture(GL_TEXTURE_2D, 0);       // Unbind texture

//   // Optional: Check for GL errors after upload
//   // GLenum err = glGetError(); if (err != GL_NO_ERROR) { /* Log error */ }
// }

void DesktopGLDriver::FlushCallbackImpl(const lv_area_t* areaToFlush, uint8_t* pixmap)
{
    // In FULL mode, areaToFlush should ideally cover the whole screen.
    // We can use its width/height just in case, but expect full screen.
    int32_t w = lv_area_get_width(areaToFlush);
    int32_t h = lv_area_get_height(areaToFlush);

    if (m_glTextureId == 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Flush - No GL texture");
        return; // Check texture exists
    }
    if (!pixmap) {
         SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Flush - Null pixmap buffer");
         return; // Check buffer validity
    }


    glBindTexture(GL_TEXTURE_2D, m_glTextureId);
    // Set alignment for packed RGB565 data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Upload DIRECTLY from LVGL's buffer ('pixmap')
    // Use glTexSubImage2D to potentially update existing texture data efficiently
    // (even if updating the whole area, it avoids potential reallocation vs glTexImage2D)
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    areaToFlush->x1, areaToFlush->y1, w, h, // Use area coords/size
                    GL_RGB,    // GL_RGB
                    GL_UNSIGNED_SHORT_5_6_5,      // GL_UNSIGNED_SHORT_5_6_5
                    pixmap);                          // Direct pointer to LVGL's data

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Restore default alignment
    glBindTexture(GL_TEXTURE_2D, 0);       // Unbind

    // Optional: Check GL errors
    // GLenum err = glGetError(); if (err != GL_NO_ERROR) { /* Log error */ }
}

