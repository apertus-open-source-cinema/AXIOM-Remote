#ifndef MENUITEM_H
#define MENUITEM_H

#include "IWidget.h"
#include "../Painter.h"

#include "../Color565.h"

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
    bool _selected;
    bool _highlighted;
    char* _label;
    char* _value;
    MenuItemType _type;

    uint16_t _backgroundColor;
    uint16_t _backgroundHighlightColor;
    uint16_t _textColor;

    uint16_t _currentBackgroundColor;

    uint8_t _verticalLabelOffset;

  public:
    MenuItem(const char* label = "...", bool disabled = false, const char* value = nullptr, bool hidden = false,
             bool selected = false, bool highlighted = false,
             MenuItemType type = MenuItemType::MENU_ITEM_TYPE_NUMERIC) :
        _disabled(disabled),
        _hidden(hidden), _selected(selected), _highlighted(highlighted), _label(const_cast<char*>(label)),
        _value(const_cast<char*>(value)), _type(type), _backgroundColor((uint16_t)Color565::White),
        _backgroundHighlightColor(RGB565(0, 128, 255)), _textColor((uint16_t)Color565::Black),
        _currentBackgroundColor(_backgroundColor), _verticalLabelOffset(7)
    {
        _x = 0;
        _y = 0;
        _width = 50;
        _height = 20;
    }

    void SetDisabled(bool disabled)
    {
        _disabled = disabled;
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

    void SetSelected(bool selected)
    {
        _selected = selected;
    }

    bool IsSelected()
    {
        return _selected;
    }

    void SetHighlighted(bool highlighted)
    {
        _highlighted = highlighted;

        if (highlighted)
        {
            _currentBackgroundColor = _backgroundHighlightColor;
        } else
        {
            _currentBackgroundColor = _backgroundColor;
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

    char* GetLabel()
    {
        return _label;
    }

    void SetValue(char* value)
    {
        _value = value;
    }

    char* GetValue()
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

    void Draw(Painter* painter) override
    {
        // Draw background
        painter->DrawFillRectangle(_x, _y, _width, _height, _currentBackgroundColor);

        painter->DrawText(_x + 5, _y + _verticalLabelOffset, _label, _textColor, Font::FreeSans9pt7b,
                          TextAlign::TEXT_ALIGN_LEFT, 0);

        // value
        if(_value != nullptr)
        {
            painter->DrawText(_x + 180, _y + _verticalLabelOffset, _value, _textColor, Font::FreeSans9pt7b,
                          TextAlign::TEXT_ALIGN_RIGHT, 80);
        }        
    }
};

#endif /* MENUITEM_H */
