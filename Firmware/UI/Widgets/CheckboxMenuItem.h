#ifndef CHECKBOXMENUITEM_H
#define CHECKBOXMENUITEM_H

#include "MenuItem.h"
#include "../../../Firmware/Media/Icons/checkboxfalse_icon.h"
#include "../../../Firmware/Media/Icons/checkboxtrue_icon.h"

class IPainter;

class CheckBoxMenuItem : public MenuItem
{
    bool _checkboxValue = false;

  public:
    CheckBoxMenuItem(CentralDB* centralDB = nullptr, const char* label = "", bool checked = false) :
        MenuItem(centralDB, label), _checkboxValue(checked)
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

        //Conditional statement for Checkbox Draw Fuction using ternary operator 
        auto checkbox = (_checkboxValue) ? checkboxtrue_icon : checkboxfalse_icon;
        painter->DrawIcon(&checkbox, _x + 235 , _y + _verticalLabelOffset - 17, _currentTextColor);
        

    }

    void ExecuteAction(IMenuSystem* menuSystem) override
    {
        // toggle checkbox
        _checkboxValue = !_checkboxValue;
    }
};

#endif /* CHECKBOXMENUITEM_H */
