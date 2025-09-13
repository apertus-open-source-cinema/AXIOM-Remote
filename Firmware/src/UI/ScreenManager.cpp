#include "ScreenManager.h"

#include <cstdio>
#include <memory>
#include <stdexcept>

#include "AppContext.h"

ScreenManager::ScreenManager(AppContext* context) : _appContext(context)
{
}

void ScreenManager::AddScreen(ScreenID id, std::unique_ptr<BaseScreen> screen, int row, int col)
{
    if (!screen)
    {
        return;
    }

    BaseScreen* instancePtr = screen.get(); // Get raw pointer before moving ownership
    m_screens.push_back(std::move(screen));
    m_screenMap[id] = {row, col, instancePtr};
}

void ScreenManager::Initialize(lv_obj_t* parent, ScreenID initialScreen)
{
    if (!parent)
    {
        printf("ScreenManager: Invalid parent object provided.");
    }
    if (m_screens.empty())
    {
        return;
    }

    // 1. Create the Tileview
    m_tileView = lv_tileview_create(parent);
    lv_obj_remove_style_all(m_tileView);                   // Optional: remove default styles if needed
    lv_obj_set_size(m_tileView, lv_pct(100), lv_pct(100)); // Fill parent

    // 2. Create tiles and populate them using the added screens
    for (const auto& pair : m_screenMap)
    {
        ScreenID id = pair.first;
        const ScreenInfo& info = pair.second;

        // Add a tile for this screen. Assume horizontal layout for now (row 0). Use LV_DIR_NONE if no swipe needed.
        lv_obj_t* tile = lv_tileview_add_tile(m_tileView, info.col, info.row, LV_DIR_HOR); // Or LV_DIR_ALL etc.

        if (!tile)
        {
            printf("Failed to add tile for screen ID %d\n", (int)id);
            continue;
        }

        // Call the screen's Create method, passing the tile as its parent
        if (info.instance)
        {
            info.instance->Create(tile);
        }
    }

    // 3. Set the initial tile
    if (m_screenMap.count(initialScreen))
    {
        const auto& info = m_screenMap[initialScreen];
        lv_tileview_set_tile_by_index(m_tileView, info.col, info.row, LV_ANIM_OFF);
        m_currentScreen = info.instance;
        if (m_currentScreen)
            m_currentScreen->OnEnter(); // Notify initial screen
    }
    else
    {
        // Set to the first screen added if initial is invalid
        const auto& firstPair = *m_screenMap.begin();
        lv_tileview_set_tile_by_index(m_tileView, firstPair.second.col, firstPair.second.row, LV_ANIM_OFF);
        m_currentScreen = firstPair.second.instance;
        if (m_currentScreen)
            m_currentScreen->OnEnter();
        printf("Initial screen ID not found, defaulting to first screen.");
    }

    // 4. Add event handler to track tile changes
    lv_obj_add_event_cb(m_tileView, TileViewEventHandler, LV_EVENT_VALUE_CHANGED, this);

    printf("ScreenManager Initialized with %d screens\n", m_screens.size());
}

void ScreenManager::SwitchToScreen(ScreenID id, bool animate)
{
    if (!m_tileView)
        return;

    if (m_screenMap.count(id))
    {
        const auto& info = m_screenMap[id];
        printf("Switching to screen ID %d (Tile %d, %d)", (int)id, info.col, info.row);
        lv_tileview_set_tile_by_index(m_tileView, info.col, info.row, animate ? LV_ANIM_ON : LV_ANIM_OFF);
        // Note: Actual update of m_currentScreen happens in the event handler
    }
    else
    {
        printf("SwitchToScreen: Screen ID %d not found.", (int)id);
    }
}

void ScreenManager::RouteAction(ButtonID id, ButtonState state)
{
    if (m_currentScreen)
    {
        // Pass the action to the currently active screen's handler
        if (!m_currentScreen->HandleAction(id, state))
        {
            // Optional: Handle actions globally if not handled by the current screen
            printf("Action %d not handled by current screen.", (int)id);
        }
    }
    else
    {
        printf("RouteAction: No current screen active.");
    }
}

// --- Tileview Event Handling ---

/*static*/ void ScreenManager::TileViewEventHandler(lv_event_t* event)
{
    ScreenManager* instance = static_cast<ScreenManager*>(lv_event_get_user_data(event));
    if (instance && lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED)
    {
        instance->HandleTileChange(event);
    }
}

void ScreenManager::HandleTileChange(lv_event_t* event)
{
    lv_obj_t* currentTile = lv_tileview_get_tile_active(m_tileView);
    if (!currentTile)
        return;

    BaseScreen* newScreen = nullptr;
    // Find which screen instance corresponds to this tile object
    for (const auto& pair : m_screenMap)
    {
        if (pair.second.instance && pair.second.instance->GetRootObject() == currentTile)
        {
            newScreen = pair.second.instance;
            break;
        }
    }

    if (newScreen != m_currentScreen)
    {
        printf("Screen changed via TileView event.");
        if (m_currentScreen)
        {
            m_currentScreen->OnExit(); // Notify previous screen
        }
        m_currentScreen = newScreen;
        if (m_currentScreen)
        {
            m_currentScreen->OnEnter(); // Notify new screen
        }
    }
}
