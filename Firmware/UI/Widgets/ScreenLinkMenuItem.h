#ifndef SCREENLINKMENUITEM_H
#define SCREENLINKMENUITEM_H

#include "MenuItem.h"
#include "../MenuDefinitions.h"

class IPainter;

class ScreenLinkMenuItem : public MenuItem
{
    AvailableScreens _targetScreen;

  public:
    ScreenLinkMenuItem(const char* label = "", AvailableScreens targetscreen = AvailableScreens::MainPage,
                       bool drawarrow = true) :
        MenuItem(const_cast<char*>(label)),
        _targetScreen(targetscreen)
    {
        if (drawarrow)
        {
            _value = ">";
        }
    }

    void SetTargetScreen(AvailableScreens targetscreen)
    {
        _targetScreen = targetscreen;
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
        menuSystem->SetCurrentScreen(_targetScreen);
    }
};

#endif /* SCREENLINKMENUITEM_H */
