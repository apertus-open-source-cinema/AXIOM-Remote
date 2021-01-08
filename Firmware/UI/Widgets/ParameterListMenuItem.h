#ifndef PARAMETERLISTMENUITEM_H
#define PARAMETERLISTMENUITEM_H

#include "MenuItem.h"
#include "../MenuDefinitions.h"

class IPainter;

class ParameterListMenuItem : public MenuItem
{
    int8_t _highlightIndex;       // which item in parameter men is currently highlighted
    int8_t _pressedIndex;         // which item in parameter men is currently pressed
    uint8_t _previousOptionIndex; // this item is the "old" selected choice but remains in effcet until the parameter
                                  // menu select has been completed
    uint8_t _optionCount;         // number of parameter menu entries
    char const* _optionLabels[64];
    uint8_t _choiceIndex;

  public:
    ParameterListMenuItem(CentralDB* centralDB = nullptr, const char* label = "") : MenuItem(centralDB, label)
    {
        _type = MenuItemType::MENU_ITEM_TYPE_LIST;
    }

    void SetOptions(const char* optionlabels[], uint8_t options)
    {
        _optionCount = options;
        for (int8_t i = 0; i < options; i++)
        {
            _optionLabels[i] = optionlabels[i];
        }
    }

    char const* GetOption(uint8_t index)
    {
        return _optionLabels[index];
    }

    void UpdateChoice(uint8_t choiceindex)
    {
        if (choiceindex < _optionCount)
        {
            _choiceIndex = choiceindex;
            _value = _optionLabels[choiceindex];
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
        if (_value != nullptr)
        {
            painter->DrawText(_x + 180, _y + _verticalLabelOffset, _value, _currentTextColor,
                              TextAlign::TEXT_ALIGN_RIGHT, 80);
        }
    }

    virtual void ExecuteAction(IMenuSystem* menuSystem) override
    {
        // menuSystem->SetCurrentScreen(_targetScreen);
    }

    uint8_t GetOptionCount()
    {
        return _optionCount;
    }
};

#endif /* SCREENLINKMENUITEM_H */
