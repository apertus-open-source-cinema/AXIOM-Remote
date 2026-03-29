#pragma once

#include "AppContext.h"
#include "ButtonDefinitions.h"
#include <lvgl.h>

class BaseScreen
{
  protected:
    lv_obj_t* _screenRoot = nullptr;
    AppContext* _appContext;

    lv_obj_t* _topContainer    = nullptr;
    lv_obj_t* _leftContainer   = nullptr;
    lv_obj_t* _centerContainer = nullptr;
    lv_obj_t* _rightContainer  = nullptr;
    lv_obj_t* _bottomContainer = nullptr;

  public:
    explicit BaseScreen(AppContext* context) : _appContext(context)
    {
    }
    virtual ~BaseScreen() = default;

    /**
     * @brief Creates the LVGL widgets for this screen.
     * Called by the ScreenManager.
     * @param parent The parent object (typically the tile view tile) to create widgets on.
     */
    virtual void Create(lv_obj_t* parent);

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
     * @brief Handles knob rotation events.
     * Override in derived classes to handle knob input relevant to that screen.
     * @param delta The amount of rotation (positive for clockwise, negative for counter-clockwise).
     * @return true if the knob event was handled, false otherwise.
     */
    virtual bool HandleKnob(int8_t delta)
    {
        (void)delta;  // Mark as unused in base class
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

    AppContext* GetContext() const
    {
        return _appContext;
    }
};
