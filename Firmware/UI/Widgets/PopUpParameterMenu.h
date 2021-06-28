#ifndef POPUPPARAMETERMENU_H
#define POPUPPARAMETERMENU_H

#include "IWidget.h"
#include "ParameterMenuItem.h"
#include "../Painter/IPainter.h"
#include "../../GlobalSettings.h"
#include "../Color565.h"

class PopUpParameterMenu : public IWidget
{
    uint16_t _backgroundColor;
    uint16_t _TextColor;
    uint16_t _borderColor;
    uint16_t _highlightColor;
    int8_t _highlightIndex;       // which item in parameter men is currently highlighted
    int8_t _pressedIndex;         // which item in parameter men is currently pressed
    uint8_t _previousChoiceIndex; // this item is the "old" selected choice but remains in effcet until the parameter
                                  // menu select has been completed
    uint8_t _choiceCount;         // number of parameter menu entries
    uint8_t _menuItemIndex; // this number indicates which menu item the parameter menu was launched from and affects
                            // location/choice positions
    uint8_t _borderwidth;
    uint8_t _horizontalTextMargin;
    bool init;

    // use fixed number of maximum 7 choices for the popup menu as more would not fit on the screen at a time
    ParameterMenuItem _parameterMenuItem[7] = {
        ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("..."),
        ParameterMenuItem("..."), ParameterMenuItem("..."), ParameterMenuItem("...")};

  public:
    PopUpParameterMenu(uint16_t x, uint16_t y);

    void SetHighlighted(uint8_t highlightindex);

    void SetChoices(const char* choicelabels[], uint8_t choices);

    int8_t GetHighlightIndex();

    int8_t GetPressedIndex();
    void SetPressedIndex(uint8_t index);

    void CalculateDimensions(IPainter* painter);

    void SetPosition(uint16_t x, uint16_t y);

    virtual void Draw(IPainter* painter) override;

    void UnPressAllMenuItems();
    void UnhighlightAllMenuItems();

    void ClearPreviousChoice();
};

#endif // POPUPPARAMETERMENU_H
