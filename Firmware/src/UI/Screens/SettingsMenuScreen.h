#pragma once
#include "BaseScreen.h"

#include "ButtonDefinitions.h"
#include "UIIdentifiers.h"
#include "lvgl.h"

class AppContext;

class SettingsMenuScreen : public BaseScreen
{

  public:
    SettingsMenuScreen(AppContext* context);
    ~SettingsMenuScreen() override = default;

    void Create(lv_obj_t* parent) override;
    bool HandleAction(ButtonID id, ButtonState state) override;
    void OnEnter() override;
    void OnExit() override;

    void UpdateDisplay();
};