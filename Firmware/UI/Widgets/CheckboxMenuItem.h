#ifndef CHECKBOXMENUITEM_H
#define CHECKBOXMENUITEM_H

#include "MenuItem.h"
/*
#include "../Color565.h"
#include "../../GlobalSettings.h"*/

class IPainter;

class CheckBoxMenuItem : public MenuItem
{
    bool _checkboxValue = false;

  public:
    CheckBoxMenuItem()
    {
        _type = MenuItemType::MENU_ITEM_TYPE_CHECKBOX;
    }

    void SetValue(bool value)
    {
        _checkboxValue = value;
    }

    bool GetValue()
    {
        return _checkboxValue;
    }

    void Draw(IPainter* painter) override
    {
        // Draw background
        if (_disabled)
        {
            painter->DrawStripedRectangle(_x, _y, _width, _height, 0xE71C, 0xD69A, 5, 12);
        } else
        {
            painter->DrawFillRectangle(_x, _y, _width, _height, _currentBackgroundColor);
        }

        // Label
        painter->DrawText(_x + 5, _y + _verticalLabelOffset, _label, _currentTextColor, TextAlign::TEXT_ALIGN_LEFT, 0);

        // value
        if (_checkboxValue)
        {
            painter->DrawText(_x + 180, _y + _verticalLabelOffset, "on", _currentTextColor, TextAlign::TEXT_ALIGN_RIGHT,
                              80);
        } else
        {
            painter->DrawText(_x + 180, _y + _verticalLabelOffset, "off", _currentTextColor,
                              TextAlign::TEXT_ALIGN_RIGHT, 80);
        }
    }

    void ExecuteAction(IMenuSystem* menuSystem) override
    {
        // toggle checkbox
        _checkboxValue = !_checkboxValue;
    }
};

#endif /* CHECKBOXMENUITEM_H */
