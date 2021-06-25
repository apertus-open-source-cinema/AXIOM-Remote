#ifndef MENUITEM_H
#define MENUITEM_H

#include "IWidget.h"
#include "../Painter/Painter.h"
#include "../IMenuSystem.h"

#include "../Color565.h"
#include "../../GlobalSettings.h"
#include "../../CentralDB/CentralDB.h"
#include "../../CentralDB/CentralDBObserver.h"

#include "Periphery/ILI9341/ILI9341Device.h"

class IPainter;

enum class MenuItemType
{
    MENU_ITEM_TYPE_READONLY,
    MENU_ITEM_TYPE_NUMERIC,
    MENU_ITEM_TYPE_DROPDOWN,
    MENU_ITEM_TYPE_LIST,
    MENU_ITEM_TYPE_CHECKBOX,
    MENU_ITEM_TYPE_LINK,
    MENU_ITEM_TYPE_NONE
};

class MenuItem : public IWidget
{
  protected:
    CentralDB* _db;

    bool _disabled;
    bool _hidden;
    bool _pressed;
    bool _highlighted;
    char const* _label;
    char const* _value;
    MenuItemType _type;

    uint16_t _backgroundColor;
    uint16_t _backgroundHighlightColor;
    uint16_t _backgroundPressedColor;
    // uint16_t _backgroundDimmedColor;
    uint16_t _backgroundDisabledColor;

    uint16_t _textColor;
    uint16_t _textHighlightColor;
    uint16_t _textPressedColor;
    // uint16_t _textDimmedColor;
    uint16_t _textDisabledColor;

    uint16_t _currentBackgroundColor;
    uint16_t _currentTextColor;

    uint8_t _verticalLabelOffset;

    void (*_handlerPtr)(void*, CentralDB*);

  public:
    MenuItem(CentralDB* centralDB = nullptr, const char* label = "...", bool disabled = false,
             const char* value = nullptr, bool hidden = false, bool pressed = false, bool highlighted = false,
             MenuItemType type = MenuItemType::MENU_ITEM_TYPE_NONE) :
        _db(centralDB),
        _disabled(disabled), _hidden(hidden), _pressed(pressed), _highlighted(highlighted), _label(label),
        _value(value), _type(type), _backgroundColor((uint16_t)Color565::White),
        _backgroundHighlightColor(utils::RGB565(255, 128, 0)), _backgroundPressedColor(utils::RGB565(0, 128, 255)),
        _backgroundDisabledColor(utils::RGB565(180, 180, 180)), _textColor((uint16_t)Color565::Black),
        _textHighlightColor((uint16_t)Color565::White), _textPressedColor((uint16_t)Color565::White),
        _textDisabledColor(utils::RGB565(180, 180, 180)), _currentBackgroundColor(_backgroundColor),
        _currentTextColor(_textColor), _verticalLabelOffset(20), _handlerPtr(nullptr)
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

    virtual void SetHandler(void (*handler)(void*, CentralDB*))
    {
        _handlerPtr = handler;
    }

    void Activate(void* sender)
    {
        _handlerPtr(sender, _db);
    }
    void attachObserver()
    {
        /* if (_db != nullptr)
         {
             _db->attach(&_observer);
         }*/
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

    bool IsPressed()
    {
        return _pressed;
    }

    void SetHighlighted(bool highlighted)
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

    bool IsHighlighted()
    {
        return _highlighted;
    }

    void SetLabel(const char* value)
    {
        _label = value;
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
        if (type == MenuItemType::MENU_ITEM_TYPE_READONLY)
        {
            _currentTextColor = _textDisabledColor;
        }
    }

    MenuItemType GetMenuType()
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

    void SetY(uint16_t y)
    {
        _y = y;
    }

    void Draw(IPainter* painter) override
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
            painter->DrawText(_x + 180, _y + _verticalLabelOffset, _value, _currentTextColor,
                              TextAlign::TEXT_ALIGN_RIGHT, 80);
        }
    }

    virtual void ExecuteAction(IMenuSystem* menuSystem)
    {
    }

    void update()
    {
    }
};

#endif /* MENUITEM_H */
