#include "NumericMenuItem.h"

NumericMenuItem::NumericMenuItem(CentralDB* centralDB, const char* label, int16_t value, int16_t minRange,
                                 int16_t maxRange, int16_t stepsize, const char* suffix) :
    MenuItem(centralDB, label),
    _value(value), _previousValue(0), _minRange(minRange), _maxRange(maxRange), _stepSize(stepsize), _suffix(suffix)
{
    _type = MenuItemType::MENU_ITEM_TYPE_NUMERIC;
}

void NumericMenuItem::SetValue(int16_t value)
{
    if ((value >= _minRange) & (value <= _maxRange))
    {
        _value = value;
        _previousValue = value;
    }
}

void NumericMenuItem::SetSuffix(const char* value)
{
    _suffix = value;
}

char const* NumericMenuItem::GetSuffix()
{
    return _suffix;
}
void NumericMenuItem::SetStepSize(int16_t newstepsize)
{
    _stepSize = newstepsize;
}

int16_t NumericMenuItem::GetStepSize()
{
    return _stepSize;
}

int16_t NumericMenuItem::GetMaxRange()
{
    return _maxRange;
}

int16_t NumericMenuItem::GetMinRange()
{
    return _minRange;
}

int16_t NumericMenuItem::GetValue()
{
    return _value;
}

void NumericMenuItem::Draw(IPainter* painter)
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
    painter->DrawText(_x + 180, _y + _verticalLabelOffset, valuetext, _currentTextColor, TextAlign::TEXT_ALIGN_RIGHT,
                      80);
}

void NumericMenuItem::ExecuteAction(IMenuSystem* menuSystem)
{
}