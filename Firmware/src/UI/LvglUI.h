#pragma once

#include <lvgl.h>
#include <string>

#include <ButtonDefinitions.h>

#include "AppContext.h"
#include "ScreenManager.h"
#include "Screens/MainScreen.h"

class PlatformInterface
{

  public:
    virtual void DisplayFlush(int* color_p, int x, int y, int w, int h) = 0;
    // virtual void readInput(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) = 0;
    // virtual void startTimer(int ms, void (*callback)(void *)) = 0;
    virtual ~PlatformInterface()
    {
    }
};

class LvglUI
{
  public:
    // enum class ButtonID
    // {
    //     BUTTON_1,
    //     BUTTON_2,
    //     BUTTON_3
    // };

    LvglUI();
    ~LvglUI();

    // Non-copyable/movable...

    void UpdateStatusLabel(const std::string& text);

    void TriggerButtonEvent(ButtonID id, ButtonState state);

  private:
    lv_obj_t* m_statusLabel = nullptr;
    lv_obj_t* m_counterButtonLabel = nullptr;

    // Static Event Callback
    static void CounterButtonEventCallbackStatic(lv_event_t* event); // Renamed param

    // Instance Event Handler
    void CounterButtonEventCallbackImpl(lv_event_t* event); // Renamed param

    // Helper to create the UI
    void CreateWidgets(lv_obj_t* parentScreen); // Renamed param

    void MainPage(lv_obj_t* parentScreen);

    std::unique_ptr<AppContext> _appContext;
    std::unique_ptr<MainScreen> _mainScreen;

    void InitializeScreens();

    // TODO: Move to the dedicated screen later
    lv_obj_t* _menuButton = nullptr;

    std::unique_ptr<ScreenManager> _screenManager;
};