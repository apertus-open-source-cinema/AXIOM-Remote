#ifndef NUMERICMENUITEM_H
#define NUMERICMENUITEM_H

#include "MenuItem.h"
#include "../MenuDefinitions.h"

#include <stdio.h>

class IPainter;

class NumericMenuItem : public MenuItem
{
    int16_t _value;
    int16_t _previousValue; // this item is the "old" selected choice but remains in effcet until the new choice
                            // has been made

    int16_t _minRange;
    int16_t _maxRange;
    int16_t _stepSize;

    char const* _suffix;

  public:
    NumericMenuItem(CentralDB* centralDB = nullptr, const char* label = "", int16_t value = 0, int16_t minRange = 0,
                    int16_t maxRange = 100, int16_t stepsize = 1, const char* suffix = "") :
        MenuItem(centralDB, label),
        _value(value), _previousValue(0), _minRange(minRange), _maxRange(maxRange), _stepSize(stepsize), _suffix(suffix)
    {
        _type = MenuItemType::MENU_ITEM_TYPE_NUMERIC;
    }

    void SetValue(int16_t value)
    {
        if ((value >= _minRange) & (value <= _maxRange))
        {
            _value = value;
            _previousValue = value;
        }
    }

    void SetSuffix(const char* value)
    {
        _suffix = value;
    }

    char const* GetSuffix()
    {
        return _suffix;
    }
    void SetStepSize(int16_t newstepsize)
    {
        _stepSize = newstepsize;
    }

    int16_t GetStepSize()
    {
        return _stepSize;
    }

    int16_t GetMaxRange()
    {
        return _maxRange;
    }

    int16_t GetMinRange()
    {
        return _minRange;
    }

    int16_t GetValue()
    {
        return _value;
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
        char valuetext[8];
        sprintf(valuetext, "%d%s", _value, _suffix);
        painter->DrawText(_x + 180, _y + _verticalLabelOffset, valuetext, _currentTextColor,
                          TextAlign::TEXT_ALIGN_RIGHT, 80);
    }

    virtual void ExecuteAction(IMenuSystem* menuSystem) override
    {
    }
};

#endif /* NUMERICMENUITEM_H */
