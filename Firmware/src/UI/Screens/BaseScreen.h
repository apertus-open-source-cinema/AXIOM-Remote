#pragma once

#include "../Theme.h"
#include "AppContext.h"
#include "ButtonDefinitions.h"
#include "UIIdentifiers.h"
#include <lvgl.h>

class BaseScreen
{
  protected:
    lv_obj_t* _screenRoot = nullptr; // The root object for this screen (the tile object)
    AppContext* _appContext;          // Pointer to shared application context

  public:
    explicit BaseScreen(AppContext* context) : _appContext(context)
    {
    }
    virtual ~BaseScreen() = default; // Virtual destructor is essential!

    /**
     * @brief Creates the LVGL widgets for this screen.
     * Called by the ScreenManager.
     * @param parent The parent object (typically the tile view tile) to create widgets on.
     */
    virtual void Create(lv_obj_t* parent)
    {
        _screenRoot = parent; // Store the root tile object
        // Derived classes will implement widget creation here

        lv_obj_set_style_bg_color(parent, lv_color_hex(0x808080), 0); // Medium gray background
        lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);
    }

    /**
     * @brief Handles an action triggered by hardware/simulation buttons.
     * Override in derived classes to handle actions relevant to that screen.
     * @param actionId The identifier of the action that occurred.
     * @return true if the action was handled, false otherwise (allows bubbling up if needed).
     */
    virtual bool HandleAction(ButtonID id, ButtonState state)
    {
        (void)id;     // Mark as unused in base class
        return false; // Base implementation handles nothing
    }

    /**
     * @brief Called by ScreenManager when this screen becomes active (visible).
     * Override for setup specific to entering the screen.
     */
    virtual void OnEnter()
    {
    }

    /**
     * @brief Called by ScreenManager when this screen becomes inactive (hidden).
     * Override for cleanup specific to exiting the screen.
     */
    virtual void OnExit()
    {
    }

    // Getter for the root object (optional, might not be needed externally)
    lv_obj_t* GetRootObject() const
    {
        return _screenRoot;
    }
};
