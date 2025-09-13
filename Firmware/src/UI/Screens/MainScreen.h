#pragma once
#include "BaseScreen.h"

#include "ButtonDefinitions.h"
#include "UIIdentifiers.h"
#include "lvgl.h"

class AppContext;

class MainScreen : public BaseScreen
{
    // Pointers to LVGL widgets specific to this screen
    lv_obj_t* m_fpsValueLabel = nullptr;
    lv_obj_t* m_aGainValueLabel = nullptr;
    // ... other widget pointers ...

    lv_obj_t* _menuButton;

    // Declare styles needed specifically for this screen if not global
    // static lv_style_t style_...;

    // Declare event handlers specific to this screen's widgets
    // static void FpsButtonEventHandler(lv_event_t* event);

    // Helper to create the compound control blocks (can be reused from LvglUI)
    lv_obj_t* CreateControlBlock(lv_obj_t* parent, const char* label_text, const char* value_text = "...");
    lv_obj_t* CreateInvertedControlBlock(lv_obj_t* parent, const char* label_text, const char* value_text = "...");

  public:
    // Constructor might take AppContext or other dependencies
    MainScreen(AppContext* context);
    ~MainScreen() override = default;

    void Create(lv_obj_t* parent) override;
    bool HandleAction(ButtonID id, ButtonState state) override;
    void OnEnter() override;
    void OnExit() override;

    // Method to update display based on AppContext (or internal logic)
    void UpdateDisplay();
};