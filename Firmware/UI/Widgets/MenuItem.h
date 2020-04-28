#ifndef MENUITEM_H
#define MENUITEM_H

#include "IWidget.h"

#include "../Color565.h"

class IPainter;

enum class MenuItemType
{
    MENU_ITEM_TYPE_SUBMENU,
    MENU_ITEM_TYPE_PAGELINK,
    MENU_ITEM_TYPE_BACKLINK,
    MENU_ITEM_TYPE_READONLY,
    MENU_ITEM_TYPE_NUMERIC,
    MENU_ITEM_TYPE_DROPDOWN
};

class MenuItem : public IWidget
{
    bool _disabled;
    bool _hidden;
    bool _pressed;
    bool _highlighted;
    char const* _label;
    char const* _value;
    MenuItemType _type;
    AvailableScreens _targetScreen;

    uint16_t _backgroundColor;
    uint16_t _backgroundHighlightColor;
    uint16_t _backgroundPressedColor;
    uint16_t _backgroundDimmedColor;
    uint16_t _backgroundDisabledColor;

    uint16_t _textColor;
    uint16_t _textHighlightColor;
    uint16_t _textPressedColor;
    uint16_t _textDimmedColor;
    uint16_t _textDisabledColor;

    uint16_t _currentBackgroundColor;
    uint16_t _currentTextColor;

    uint8_t _verticalLabelOffset;

  public:
    MenuItem(const char* label = "...", bool disabled = false, const char* value = nullptr, bool hidden = false,
             bool pressed = false, bool highlighted = false, MenuItemType type = MenuItemType::MENU_ITEM_TYPE_NUMERIC) :
        _disabled(disabled),
        _hidden(hidden), _pressed(pressed), _highlighted(highlighted), _label(const_cast<char*>(label)),
        _value(const_cast<char*>(value)), _type(type), _backgroundColor((uint16_t)Color565::White),
        _backgroundHighlightColor(RGB565(255, 128, 0)), _backgroundPressedColor(RGB565(0, 128, 255)),
        _backgroundDisabledColor(RGB565(180, 180, 180)), _textColor((uint16_t)Color565::Black),
        _textHighlightColor((uint16_t)Color565::White), _textPressedColor((uint16_t)Color565::White),
        _textDisabledColor(RGB565(180, 180, 180)), _currentBackgroundColor(_backgroundColor),
        _currentTextColor(_textColor), _verticalLabelOffset(20)
    {
        _x = 0;
        _y = 0;
        _width = 50;
        _height = 20;
    }

    void SetDisabled(bool disabled)
    {
        _disabled = disabled;

        if (disabled)
        {
            _currentBackgroundColor = _backgroundDisabledColor;
            _currentTextColor = _textDisabledColor;
        } else
        {
            // TODO: add more case handling here (what if highlighted, pressed, etc.)

            _currentBackgroundColor = _backgroundColor;
            _currentTextColor = _textColor;
        }
    }

    void SetTargetScreen(AvailableScreens targetscreen)
    {
        _targetScreen = targetscreen;
    }

    bool IsDisabled()
    {
        return _disabled;
    }

    void SetHidden(bool hide)
    {
        _hidden = hide;
    }

    bool IsHidden()
    {
        return _hidden;
    }

    void SetPressed(bool pressed)
    {
        if (_disabled)
        {
            return;
        }

        _pressed = pressed;

        if (pressed)
        {
            _currentBackgroundColor = _backgroundPressedColor;
        } else if (_highlighted)
        {
            _currentBackgroundColor = _backgroundHighlightColor;
        } else
        {
            _currentBackgroundColor = _backgroundColor;
        }
    }

    bool IsPressed()
    {
        return _pressed;
    }

    void SetHighlighted(bool highlighted)
    {
        _highlighted = highlighted;

        if (highlighted)
        {
            _currentBackgroundColor = _backgroundHighlightColor;
            _currentTextColor = _textHighlightColor;
        } else if (_disabled)
        {
            _currentBackgroundColor = _backgroundDisabledColor;
            _currentTextColor = _textDisabledColor;
        } else
        {
            _currentBackgroundColor = _backgroundColor;
            _currentTextColor = _textColor;
        }
    }

    bool IsHighlighted()
    {
        return _highlighted;
    }

    void SetLabel(const char* value)
    {
        _label = const_cast<char*>(value);
    }

    char const* GetLabel()
    {
        return _label;
    }

    void SetValue(char const* value)
    {
        _value = value;
    }

    char const* GetValue()
    {
        return _value;
    }

    void SetMenuType(MenuItemType type)
    {
        _type = type;
    }

    MenuItemType GetMenuType()
    {
        return _type;
    }

    void SetDimensions(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        _x = x;
        _y = y;

        _width = width;
        _height = height;
    }

    void SetY(uint16_t y)
    {
        _y = y;
    }

    void Draw(IPainter* painter) override
    {
        // Draw background
        if (_disabled)
        {
            painter->DrawStripedRectangle(_x, _y, _width, _height, 0x8A01, 0x5120, 5, 12);
        } else
        {
            painter->DrawFillRectangle(_x, _y, _width, _height, _currentBackgroundColor);
        }

        painter->DrawText(_x + 5, _y + _verticalLabelOffset, _label, _currentTextColor, TextAlign::TEXT_ALIGN_LEFT, 0);

        // value
        if (_value != nullptr)
        {
            painter->DrawText(_x + 180, _y + _verticalLabelOffset, _value, _currentTextColor,
                              TextAlign::TEXT_ALIGN_RIGHT, 80);
        }
    }

    void ExecuteAction(IMenuSystem* menuSystem)
    {
        switch (_type)
        {
        case MenuItemType::MENU_ITEM_TYPE_BACKLINK:
            menuSystem->SetCurrentScreen(_targetScreen);
            break;
        case MenuItemType::MENU_ITEM_TYPE_PAGELINK:
            menuSystem->SetCurrentScreen(_targetScreen);
            break;
        case MenuItemType::MENU_ITEM_TYPE_SUBMENU:
            menuSystem->SetCurrentScreen(_targetScreen);
            break;
        default:
            break;
        }
    }
};

#endif /* MENUITEM_H */
