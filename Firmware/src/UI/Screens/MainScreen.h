#pragma once
#include "BaseScreen.h"

#include "ButtonDefinitions.h"

class AppContext;

class MainScreen : public BaseScreen
{
    lv_obj_t* m_fpsValueLabel   = nullptr;
    lv_obj_t* m_aGainValueLabel = nullptr;

    lv_obj_t* _menuButton = nullptr;
    lv_obj_t* _debugButton = nullptr;
    lv_obj_t* _fpsBlock = nullptr;
    lv_obj_t* _aGainBlock = nullptr;
    lv_obj_t* _dGainBlock = nullptr;
    lv_obj_t* _shutterBlock = nullptr;
    lv_obj_t* _wbBlock = nullptr;

    lv_obj_t* CreateControlBlock(lv_obj_t* parent, const char* label_text, const char* value_text = "...");
    lv_obj_t* CreateInvertedControlBlock(lv_obj_t* parent, const char* label_text, const char* value_text = "...");

  public:
    MainScreen(AppContext* context);
    ~MainScreen() override = default;

    void Create(lv_obj_t* parent) override;
    bool HandleAction(ButtonID id, ButtonState state) override;
    void OnEnter() override;
    void OnExit() override;

    void UpdateDisplay();
};
