#ifndef POPUPMENUITEM_H
#define POPUPMENUITEM_H

#include "MenuItem.h"
/*
#include "../Color565.h"
#include "../../GlobalSettings.h"*/

class IPainter;

class PopUpMenuItem : public MenuItem
{
    uint8_t _dropDownChoices;
    char const* _choiceLabels[7];
    uint8_t _choiceIndex;

  public:
    PopUpMenuItem(CentralDB* centralDB = nullptr, const char* label = "") : MenuItem(centralDB, label), _choiceIndex(0)
    {
        _type = MenuItemType::MENU_ITEM_TYPE_DROPDOWN;
    }

    void SetChoices(const char* choicelabels[], uint8_t choices)
    {
        _dropDownChoices = choices;
        for (int8_t i = 0; i < choices; i++)
        {
            _choiceLabels[i] = choicelabels[i];
        }
    }

    const char* GetChoice(uint8_t choiceindex)
    {
        return _choiceLabels[choiceindex];
    }

    uint8_t GetChoiceIndex()
    {
        return _choiceIndex;
    }

    uint8_t GetChoiceCount()
    {
        return _dropDownChoices;
    }

    void UpdateChoice(uint8_t choiceindex)
    {
        if (choiceindex < _dropDownChoices)
        {
            _value = _choiceLabels[choiceindex];
            _choiceIndex = choiceindex;
        }
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
        painter->DrawText(_x + 180, _y + _verticalLabelOffset, _value, _currentTextColor, TextAlign::TEXT_ALIGN_RIGHT,
                          80);
    }

    void ExecuteAction(IMenuSystem* menuSystem) override
    {
    }
};

#endif /* POPUPMENUITEM_H */
