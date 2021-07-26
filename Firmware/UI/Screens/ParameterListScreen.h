#ifndef PARAMETERLISTSCREEN_H
#define PARAMETERLISTSCREEN_H

#include "IScreen.h"
#include "../Widgets/PushButton.h"
#include "../Widgets/ImageButton.h"
#include "../ButtonDefinitions.h"
#include "../MenuDefinitions.h"
#include "../IMenuSystem.h"
#include "../../GlobalSettings.h"

class IPainter;
class IUSBDevice;

class ParameterListScreen : public IScreen
{
    const char* _header;
    PushButton _cancelButton;
    PushButton _setButton;

    // Colors
    uint16_t _backgroundColor;
    uint16_t _textColor;
    uint16_t _highlightColor;
    uint16_t _highlightTextColor;
    uint16_t _backgroundPressedColor;
    uint16_t _textPressedColor;

    int8_t _highlightIndex;       // which item in parameter men is currently highlighted
    int8_t _pressedIndex;         // which item in parameter men is currently pressed
    uint8_t _previousOptionIndex; // this item is the "old" selected choice but remains in effect until the parameter
                                  // menu select has been completed
    uint8_t _optionCount;         // number of parameter menu entries
    const char* _optionLabels[64];
    uint8_t _optionLineHeight;

  public:
    explicit ParameterListScreen(IUSBDevice* usbDevice);

    void SetOptions(const char* optionlabels[], uint8_t optioncount);

    void SetSetButtonPressed(bool pressed);

    void SetCancelButtonPressed(bool pressed);

    void SetHighlighted(uint8_t highlightindex);

    void SetPressed(uint8_t pressedindex);

    void UnpressAll();

    void UpdateChoice(uint8_t choiceindex);

    uint8_t GetHighlightIndex();

    void SetHeader(const char* value);
    const char* GetHeader() const;

    void Draw(IPainter* painter) override;

    static void CancelButtonHandler(void* sender);
    static void SetButtonHandler(void* sender);

    void DrawHeader(IPainter* painter);
};

#endif // PARAMETERLISTSCREEN_H
