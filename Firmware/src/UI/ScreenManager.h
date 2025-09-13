#pragma once
#include <map>
#include <memory>
#include <vector>

#include <lvgl.h>

#include "ButtonDefinitions.h"
#include "Screens/BaseScreen.h"
#include "UIIdentifiers.h"

// #include "UIIdentifiers.hpp" // ScreenID and UIAction::Identifier

// Forward declare AppContext if needed for actions
class AppContext;

class ScreenManager
{
    struct ScreenInfo
    {
        int row;
        int col;
        BaseScreen* instance;
    };

  private:
    lv_obj_t* m_tileView = nullptr;                     // The main tileview object
    std::vector<std::unique_ptr<BaseScreen>> m_screens; // Owns the screen objects
    // Map ScreenID enum to its tile coordinates (row, col) and instance pointer
    std::map<ScreenID, ScreenInfo> m_screenMap;
    BaseScreen* m_currentScreen = nullptr; // Pointer to the currently active screen instance

    AppContext* _appContext; // Pointer to shared app state/logic (optional, depends on design)

    // Tileview event handler
    static void TileViewEventHandler(lv_event_t* event);
    void HandleTileChange(lv_event_t* event); // Instance method

  public:
    ScreenManager(AppContext* context); // Pass dependencies
    ~ScreenManager() = default;

    /**
     * @brief Creates the Tileview and initializes all registered screens.
     * @param parent The LVGL object to create the Tileview on (e.g., lv_scr_act()).
     * @param initialScreen The ID of the screen to show first.
     */
    void Initialize(lv_obj_t* parent, ScreenID initialScreen);

    /**
     * @brief Programmatically switch to a specific screen.
     * @param id The ID of the screen to switch to.
     * @param animate Use LVGL animation (LV_ANIM_ON) or switch instantly (LV_ANIM_OFF).
     */
    void SwitchToScreen(ScreenID id, bool animate = true);

    /**
     * @brief Routes an action to the currently active screen.
     * @param actionId The action that occurred.
     */
    void RouteAction(ButtonID id, ButtonState state);

    // Add screens BEFORE calling Initialize
    void AddScreen(ScreenID id, std::unique_ptr<BaseScreen> screen, int row, int col);
};