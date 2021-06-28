#ifndef PARAMETERMENUITEM_H
#define PARAMETERMENUITEM_H

#include "IWidget.h"

#include "../Color565.h"

class IPainter;
class IMenuSystem;

class ParameterMenuItem : public IWidget
{
    bool _disabled;
    bool _hidden;
    bool _pressed;
    bool _highlighted;
    bool _previousChoice;
    char const* _label;
    char const* _value;

    uint16_t _backgroundColor;
    uint16_t _backgroundHighlightColor;
    uint16_t _backgroundPressedColor;
    uint16_t _backgroundDisabledColor;

    uint16_t _textColor;
    uint16_t _textHighlightColor;
    uint16_t _textPressedColor;
    uint16_t _textDisabledColor;

    uint16_t _currentBackgroundColor;
    uint16_t _currentTextColor;

    uint8_t _verticalLabelOffset;
    uint8_t _horizontalTextMargin;

  public:
    ParameterMenuItem(const char* label = "...", bool disabled = false, const char* value = nullptr,
                      bool hidden = false, bool pressed = false, bool highlighted = false);

    void SetDisabled(bool disabled);

    bool IsDisabled();

    void SetHidden(bool hide);

    bool IsHidden();

    void SetPressed(bool pressed);

    bool IsPressed();

    void SetHighlighted(bool highlighted);

    bool IsHighlighted();

    void SetLabel(const char* value);

    char const* GetLabel();

    void SetValue(char const* value);

    char const* GetValue();

    void SetPreviousChoice(bool previousChoice);

    void SetDimensions(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    void SetHorizontalTextMargin(uint8_t horizontalTextMargin);
    void SetY(uint16_t y);

    void Draw(IPainter* painter) override;

    void ExecuteAction(IMenuSystem* menuSystem);
};

#endif /* PARAMETERMENUITEM_H */
