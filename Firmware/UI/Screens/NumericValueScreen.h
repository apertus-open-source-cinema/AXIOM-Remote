#ifndef NUMERICVALUESCREEN_H
#define NUMERICVALUESCREEN_H

#include "IScreen.h"
#include "../Widgets/PushButton.h"
#include "../Widgets/ImageButton.h"
#include "../ButtonDefinitions.h"
#include "../MenuDefinitions.h"
#include "../IMenuSystem.h"
#include "../../GlobalSettings.h"
#include "../Widgets/ToggleButton.h"

#include <stdio.h>

class IPainter;
class IUSBDevice;

class NumericValueScreen : public IScreen
{
    char const* _header;
    PushButton _cancelButton;
    ToggleButton _liveButton;
    PushButton _setButton;
    uint16_t _backgroundColor;
    uint16_t _textColor;
    uint16_t _highlightColor;
    uint16_t _highlightTextColor;
    int16_t _previousvalue; // this is the "old" selected value
    int16_t _value;
    int16_t _minRange;
    int16_t _maxRange;
    uint16_t _stepSize;
    char const* _suffix;

  public:
    explicit NumericValueScreen(IUSBDevice* usbDevice);

    void SetRange(int16_t min, int16_t max);

    void SetValue(int16_t value);

    int16_t GetValue();

    void SetSuffix(const char* suffix);

    void SetStepSize(uint16_t value);

    void SetHeader(const char* value);

    char const* GetHeader();

    void IncreaseValueStep();

    void DecreaseValueStep();

    void ToggleLiveSet(bool toggle);

    void ToggleLiveSet();

    void SetSetButtonPressed(bool pressed);

    void SetCancelButtonPressed(bool pressed);

    void Draw(IPainter* painter) override;

    static void CancelButtonHandler(void* sender);
    static void SetButtonHandler(void* sender);

    void DrawHeader(IPainter* painter);
};

#endif // NUMERICVALUESCREEN_H
