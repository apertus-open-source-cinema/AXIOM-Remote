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
    explicit NumericValueScreen(IUSBDevice* usbDevice) :
        IScreen(usbDevice), _header("Parameter Menu"), _cancelButton("Cancel"), _liveButton("Live"), _setButton("Set"),
        _value(0), _stepSize(1), _suffix("")
    {
        //_cancelButton.SetHandler(&CancelButtonHandler);
        _bottomButtonBar.SetButton(ButtonPosition::Left, &_cancelButton);

        // This is the primary button in this menu
        //_setButton.SetHandler(&SetButtonHandler);
        _setButton.SetBackgroundColor((uint16_t)Color565::AXIOM_Orange);
        _bottomButtonBar.SetButton(ButtonPosition::Right, &_setButton);

        _bottomButtonBar.SetButton(ButtonPosition::Center, &_liveButton);

        _backgroundColor = (uint16_t)Color565::White;
        _textColor = (uint16_t)Color565::Black;
        _highlightColor = (uint16_t)Color565::AXIOM_Orange;
        _highlightTextColor = (uint16_t)Color565::White;
    }

    void SetRange(int16_t min, int16_t max)
    {
        _minRange = min;
        _maxRange = max;
    }

    void SetValue(int16_t value)
    {
        _value = value;
    }

    int16_t GetValue()
    {
        return _value;
    }

    void SetSuffix(const char* suffix)
    {
        _suffix = suffix;
    }

    void SetStepSize(uint16_t value)
    {
        _stepSize = value;
    }

    void SetHeader(const char* value)
    {
        _header = value;
    }

    char const* GetHeader()
    {
        return _header;
    }

    void IncreaseValueStep()
    {
        _value += _stepSize;
        if (_value > _maxRange)
        {
            _value = _maxRange;
        }
    }

    void DecreaseValueStep()
    {
        _value -= _stepSize;
        if (_value < _minRange)
        {
            _value = _minRange;
        }
    }

    void ToggleLiveSet(bool toggle)
    {
        _liveButton.SetChecked(toggle);
    }

    void ToggleLiveSet()
    {
        _liveButton.SetChecked(!_liveButton.GetChecked());
    }

    void SetSetButtonPressed(bool pressed)
    {
        _setButton.SetHighlighted(pressed);
    }

    void SetCancelButtonPressed(bool pressed)
    {
        _cancelButton.SetHighlighted(pressed);
    }

    void Draw(IPainter* painter) override
    {
        painter->Fill(_backgroundColor);
        DrawHeader(painter);

        // draw value
        painter->SetFont(Font::FreeSans24pt7b);
        uint8_t heightcenter = GlobalSettings::LCDHeight / 2;
        uint8_t fontcenter = heightcenter + painter->GetCurrentFontHeight() / 2;
        char valuestring[8];
        sprintf(valuestring, "%d%s", _value, _suffix);
        painter->DrawText(0, fontcenter, valuestring, _textColor, TextAlign::TEXT_ALIGN_CENTER,
                          GlobalSettings::LCDWidth);

        // center line
        painter->DrawLine(5, fontcenter + 5, GlobalSettings::LCDWidth - 5, fontcenter + 5, _textColor);
        painter->DrawLine(5, fontcenter + 6, GlobalSettings::LCDWidth - 5, fontcenter + 6, _textColor);

        // value position indicator
        uint16_t indicatorposition;
        indicatorposition =
            ((float)(_value - _minRange) / (float)(_maxRange - _minRange)) * (GlobalSettings::LCDWidth - 10);
        painter->DrawFillRectangle(5 + indicatorposition - 3, fontcenter + 5, 6, 10, _textColor);

        // draw min Range
        char minvaluestring[8];
        painter->SetFont(Font::FreeSans9pt7b);
        sprintf(minvaluestring, "%d", _minRange);
        painter->DrawText(5, fontcenter, minvaluestring, _textColor, TextAlign::TEXT_ALIGN_LEFT, 0);
        painter->DrawLine(5, fontcenter, 5, fontcenter + 10, _textColor);

        // draw max Range
        char maxvaluestring[8];
        sprintf(maxvaluestring, "%d", _maxRange);

        painter->DrawText(GlobalSettings::LCDWidth - 50, fontcenter, maxvaluestring, _textColor,
                          TextAlign::TEXT_ALIGN_RIGHT, 45);
        painter->DrawLine(GlobalSettings::LCDWidth - 5, fontcenter, GlobalSettings::LCDWidth - 5, fontcenter + 10,
                          _textColor);

        DrawBottomButtonBar(painter);
    }

    static void CancelButtonHandler(void* sender);
    static void SetButtonHandler(void* sender);

    void DrawHeader(IPainter* painter)
    {
        // header background
        painter->DrawFillRectangle(0, 0, GlobalSettings::LCDWidth, 30, (uint16_t)Color565::Black);

        //  header title
        painter->SetFont(Font::FreeSans12pt7b);
        painter->DrawText(0, 22, _header, (uint16_t)Color565::White, TextAlign::TEXT_ALIGN_CENTER,
                          GlobalSettings::LCDWidth);

        // header separation lines
        painter->DrawLine(0, 29, GlobalSettings::LCDWidth - 1, 29, (uint16_t)Color565::AXIOM_Orange);
    }
};

#endif // NUMERICVALUESCREEN_H
