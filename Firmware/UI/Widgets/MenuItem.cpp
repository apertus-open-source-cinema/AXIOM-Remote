#include "MenuItem.h"

MenuItem::MenuItem(CentralDB* centralDB, const char* label, bool disabled, const char* value, bool hidden, bool pressed,
                   bool highlighted, MenuItemType type) :
    _db(centralDB),
    _disabled(disabled), _hidden(hidden), _pressed(pressed), _highlighted(highlighted), _label(label), _value(value),
    _type(type), _backgroundColor((uint16_t)Color565::White), _backgroundHighlightColor(utils::RGB565(255, 128, 0)),
    _backgroundPressedColor(utils::RGB565(0, 128, 255)), _backgroundDisabledColor(utils::RGB565(180, 180, 180)),
    _textColor((uint16_t)Color565::Black), _textHighlightColor((uint16_t)Color565::White),
    _textPressedColor((uint16_t)Color565::White), _textDisabledColor(utils::RGB565(180, 180, 180)),
    _currentBackgroundColor(_backgroundColor), _currentTextColor(_textColor), _verticalLabelOffset(20),
    _handlerPtr(nullptr)
{
    _x = 0;
    _y = 0;
    _width = 50;
    _height = 20;
}

void MenuItem::SetDisabled(bool disabled)
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

void MenuItem::SetHandler(void (*handler)(void*, CentralDB*))
{
    _handlerPtr = handler;
}

void MenuItem::Activate(void* sender)
{
    _handlerPtr(sender, _db);
}

void MenuItem::attachObserver()
{
    /* if (_db != nullptr)
     {
         _db->attach(&_observer);
     }*/
}

bool MenuItem::IsDisabled()
{
    return _disabled;
}

void MenuItem::SetHidden(bool hide)
{
    _hidden = hide;
}

bool MenuItem::IsHidden()
{
    return _hidden;
}

void MenuItem::SetPressed(bool pressed)
{
    if (_disabled)
    {
        return;
    }

    _pressed = pressed;

    if (_type == MenuItemType::MENU_ITEM_TYPE_READONLY)
    {
        _currentBackgroundColor = _backgroundColor;
    } else if (pressed)
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

bool MenuItem::IsPressed()
{
    return _pressed;
}

void MenuItem::SetHighlighted(bool highlighted)
{
    _highlighted = highlighted;

    if (_type == MenuItemType::MENU_ITEM_TYPE_READONLY)
    {
        _currentTextColor = _textDisabledColor;
        _currentBackgroundColor = _backgroundColor;
    } else if (highlighted)
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

bool MenuItem::IsHighlighted()
{
    return _highlighted;
}

void MenuItem::SetLabel(const char* value)
{
    _label = value;
}

char const* MenuItem::GetLabel()
{
    return _label;
}

void MenuItem::SetValue(char const* value)
{
    _value = value;
}

char const* MenuItem::GetValue()
{
    return _value;
}

void MenuItem::SetMenuType(MenuItemType type)
{
    _type = type;
    if (type == MenuItemType::MENU_ITEM_TYPE_READONLY)
    {
        _currentTextColor = _textDisabledColor;
    }
}

MenuItemType MenuItem::GetMenuType()
{
    return _type;
}

/*void SetDimensions(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    _x = x;
    _y = y;

    _width = width;
    _height = height;
}*/

void MenuItem::SetY(uint16_t y)
{
    _y = y;
}

void MenuItem::Draw(IPainter* painter)
{
    // Draw background
    if (_disabled && !(_highlighted))
    {
        painter->DrawStripedRectangle(_x, _y, _width, _height, 0xE71C, 0xD69A, 3, 7);
    } else if (_disabled && _highlighted)
    {
        painter->DrawStripedRectangle(_x, _y, _width, _height, 0xE71C, 0xD69A, 3, 7);
        painter->DrawFillRectangle(_x, _y, 4, _height, _backgroundHighlightColor);
        painter->DrawFillRectangle(GlobalSettings::LCDWidth - 20, _y, 4, _height, _backgroundHighlightColor);
    } else if (_type == MenuItemType::MENU_ITEM_TYPE_READONLY && _highlighted)
    {
        painter->DrawFillRectangle(_x, _y, _width, _height, _currentBackgroundColor);
        painter->DrawFillRectangle(_x, _y, 4, _height, _backgroundHighlightColor);
        painter->DrawFillRectangle(GlobalSettings::LCDWidth - 20, _y, 4, _height, _backgroundHighlightColor);

    } else
    {
        painter->DrawFillRectangle(_x, _y, _width, _height, _currentBackgroundColor);
    }

    // Label
    painter->DrawText(_x + 5, _y + _verticalLabelOffset, _label, _currentTextColor, TextAlign::TEXT_ALIGN_LEFT, 0);

    // value
    if (_value != nullptr)
    {
        painter->DrawText(_x + 180, _y + _verticalLabelOffset, _value, _currentTextColor, TextAlign::TEXT_ALIGN_RIGHT,
                          80);
    }
}

void MenuItem::ExecuteAction(IMenuSystem* menuSystem)
{
}

void MenuItem::update()
{
}