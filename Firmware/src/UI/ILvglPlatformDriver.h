#pragma once

#include <lvgl.h>
// #include <src/misc/lv_types.h> // lv_coord_t is in lvgl.h

class ILvglPlatformDriver
{
public:
    // Virtual destructor is essential for polymorphism
    virtual ~ILvglPlatformDriver() = default;

    /**
     * @brief Initializes platform-specific hardware (display, input) and
     *        creates and registers the necessary LVGL display and input drivers.
     * @return true on successful initialization, false otherwise.
     */
    virtual bool Initialize() = 0; // Changed: No parameters

    /**
     * @brief Gets the horizontal resolution configured for this driver.
     * @return Horizontal resolution in pixels.
     */
    virtual lv_coord_t GetHorRes() const = 0;

    /**
     * @brief Gets the vertical resolution configured for this driver.
     * @return Vertical resolution in pixels.
     */
    virtual lv_coord_t GetVerRes() const = 0;

    /**
     * @brief Gets the LVGL display object created and managed by this driver.
     * @return Pointer to the lv_display_t object, or nullptr if not initialized.
     */
    virtual lv_display_t* GetLvDisplay() const = 0; // Added

    /**
     * @brief Gets the primary LVGL input device created and managed by this driver.
     * @return Pointer to the lv_indev_t object, or nullptr if not initialized or no input.
     */
    virtual lv_indev_t* GetLvInputDevice() const = 0; // Added (optional, return nullptr if no input)


    // --- Removed UpdateDisplayContext as it wasn't used ---
    // /**
    //  * @brief Optional method for the main application to provide runtime context updates.
    //  */
    // virtual void UpdateDisplayContext(/* parameters tailored to needs */)
    // {
    //     // Default implementation does nothing
    // }
};
