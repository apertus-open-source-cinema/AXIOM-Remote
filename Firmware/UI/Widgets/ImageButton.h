#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include "IButton.h"
#include "../Painter/Painter.h"

#include "Icon.h"

enum class ButtonStyle : uint8_t
{
    Icon = 0,
    IconAndText = 1
};

class ImageButton : public IButton
{
    const Icon* _image;
    const char* _label;

    uint8_t _cornerRadius;
    bool _highlighted;

    // Color Defintions
    uint16_t _textColor;
    uint16_t _imageColor;
    uint16_t _backgroundColor;

    uint16_t _textHighlightColor;
    uint16_t _imageHighlightColor;
    uint16_t _backgroundHighlightColor;

    uint16_t _currentTextColor;
    uint16_t _currentImageColor;
    uint16_t _currentBackgroundColor;

    uint8_t _imagePositionX;
    uint8_t _textPositionX;
    uint8_t _textPositionY;
    uint8_t _totalWidth; // TODO: remove
    uint8_t _labelWidth;

    ButtonStyle _buttonStyle;

  public:
    explicit ImageButton(const Icon* icon, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0);

    void SetCornerRadius(uint8_t cornerRadius);

    void SetButtonStyle(ButtonStyle buttonstyle);

    ButtonStyle GetButtonStyle() const;

    void SetLabel(const char* value);

    const char* GetLabel() const;

    virtual void Draw(IPainter* painter) override;

    void SetBackgroundColor(uint16_t color);

    void SetImageColor(uint16_t color);

    void SetTextColor(uint16_t color);

    void SetHighlightBackgroundColor(uint16_t color);

    void SetHighlightImageColor(uint16_t color);

    void SetHighlightTextColor(uint16_t color);

    void SetHighlighted(bool highlighted);
};

#endif // IMAGEBUTTON_H
