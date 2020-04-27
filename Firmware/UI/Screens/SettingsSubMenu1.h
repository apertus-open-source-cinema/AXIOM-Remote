#ifndef SETTINGSSUBMENU1_H
#define SETTINGSSUBMENU1_H

#include <cstring>

#include "IMenu.h"
#include "../Painter/Painter.h"
#include "../Widgets/MenuItem.h"

//#include "../Widgets/MainPageButton.h"

#include "../ButtonDefinitions.h"

#include "../../GlobalSettings.h"

#include "../../../Bootloader/Periphery/USB/IUSBDevice.h"

//#include <Helpers.h>

class SettingsSubMenu1 : public IMenu
{
    char const* _label;
    char const* _menuBreadcrumbs;

    uint8_t _menuItemsCount;
    uint8_t _menuSelectionIndex;

    uint8_t _maxVisibleItems;

    uint8_t _parameterSelectionIndex;
    uint8_t _parameterMenuActive;
    uint8_t _menuOffset;

    // Color Defintions
    uint16_t _menuBackgroundColor;
    uint16_t _menuItemColor;
    uint16_t _menuDimmedItemColor;
    uint16_t _menuDisabledItemColor;
    uint16_t _menuSelectedItemColor;
    uint16_t _menuHightlightedItemColor;
    uint16_t _menuTextColor;
    uint16_t _menuDisabledTextColor;
    uint16_t _menuSelectedTextColor;

    MenuItem _menuItems[4] = {MenuItem("< Up"), MenuItem("Option 1"), MenuItem("Option 2"), MenuItem("Option 3")};

  public:
    // TODO: Add assignment of menu system to IMenu
    explicit SettingsSubMenu1(IUSBDevice* cdcDevice) :
        IMenu(cdcDevice), _menuItemsCount(4), _menuSelectionIndex(0), _maxVisibleItems(7)
    {
        // UNUSED(cdcDevice);
        //_usbDevice = cdcDevice;

        _label = "Menu";
        _menuBreadcrumbs = "Menu > SubMenu 1";

        // Added for testing
        _menuItems[0].SetMenuType(MenuItemType::MENU_ITEM_TYPE_BACKLINK);
        _menuItems[0].SetTargetScreen(AvailableScreens::SettingsMenu);

        // Color defintions
        _menuBackgroundColor = RGB565(180, 180, 180);
        _menuItemColor = (uint16_t)Color565::White;
        _menuSelectedItemColor = RGB565(255, 128, 0);
        _menuDimmedItemColor = RGB565(247, 251, 247);
        _menuSelectedTextColor = RGB565(255, 255, 255);
        _menuHightlightedItemColor = RGB565(0, 128, 255);
        _menuTextColor = (uint16_t)Color565::Black;
        _menuDisabledTextColor = RGB565(40, 40, 40);
        _menuDisabledItemColor = RGB565(180, 180, 180);

        // init menu selection indexes
        _parameterMenuActive = 0;
        _parameterSelectionIndex = 0;
        _menuOffset = 0;

        // Default selection is first item
        _menuItems[_menuSelectionIndex].SetHighlighted(true);

        /*
                _menuItems[2] = new MenuItem();
                _menuItems[2].SetLabel("Test Item 3");

                _menuButton.SetHandler(&MenuButtonHandler);

                _analogGainButton.SetHandler(&AnalogGainButtonHandler);
                _digitalGainButton.SetHandler(&DigitalGainButtonHandler);*/
    }

    void SetLabel(char* value)
    {
        _label = value;
    }

    const char* GetLabel()
    {
        return _label;
    }

    int8_t GetMenuItemsCount()
    {
        return _menuItemsCount;
    }

  protected:
    void Draw(IPainter* painter) override
    {
        painter->SetFont(Font::FreeSans9pt7b);
        DrawHeader(painter);

        // draw menu items
        DrawMenuItems(painter);

        // draw scroll bar indicator only if there are more than 7 menu items
        if (_menuItemsCount > 7)
        {
            DrawScrollIndicator(painter);
        }
    }

    void DrawHeader(IPainter* painter)
    {
        // draw header background
        painter->DrawFillRectangle(0, 0, GlobalSettings::LCDWidth, 28, _menuItemColor);

        // draw header bread crumbs
        painter->SetFont(Font::FreeSans9pt7b);
        painter->DrawText(5, 20, _menuBreadcrumbs, _menuTextColor, TextAlign::TEXT_ALIGN_LEFT, 0);

        // two header separation lines
        painter->DrawLine(0, 29, GlobalSettings::LCDWidth - 1, 29, _menuSelectedItemColor);

        painter->DrawLine(0, 30, GlobalSettings::LCDWidth - 1, 30, _menuBackgroundColor);
    }

    void DrawMenuItems(IPainter* painter)
    {
        int8_t displaySelectionIndex = _menuSelectionIndex - _menuOffset;

        // the _menuOffset is added to the item index and defines which item is the first one shown on screen

        // scrolling up from the first item
        if (displaySelectionIndex < 0)
        {
            _menuOffset -= 1;
        }

        // scrolling down from the last item
        if (displaySelectionIndex >= 7)
        {
            _menuOffset += 1;
        }

        // only up to 7 menu items fit on screen at once
        uint8_t displayItemsCount = LimitRange(_menuItemsCount, 0, 7);

        for (uint8_t itemIndex = 0; itemIndex < displayItemsCount; itemIndex++)
        {
            MenuItem currentMenuItem = _menuItems[itemIndex + _menuOffset];

            // TODO: move this to the update() function
            SetValue(currentMenuItem);

            uint16_t y = 31 + itemIndex * 30;
            currentMenuItem.SetDimensions(31, y, GlobalSettings::LCDWidth - 30, 29);

            currentMenuItem.Draw(painter);
        }
    }

    void DrawScrollIndicator(IPainter* painter)
    {
        // start the scrollbar below the header
        uint8_t scrollbarYOrigin = 31;

        // maximum height is the screen without header area
        uint8_t scrollbarHeight = GlobalSettings::LCDHeight - scrollbarYOrigin;

        // height of the scroll indicator is defined by the ratio of number of items on screen vs total number of item.
        // for example: if there are 7 items on screen of total 14 items the scroll indicator shall be 50% of the
        // scrollbar height
        uint8_t sliderHeight = scrollbarHeight * ((float)_maxVisibleItems / _menuItemsCount);

        // Calculate offset of scrollbar for 1 item
        float segmentOffset = (scrollbarHeight - sliderHeight) / (_menuItemsCount - _maxVisibleItems);

        uint8_t scrollbarYOffset = scrollbarYOrigin + (_menuOffset * segmentOffset);

        // Background
        painter->DrawFillRectangle(GlobalSettings::LCDWidth - 16, scrollbarYOrigin, 16,
                                   GlobalSettings::LCDHeight - scrollbarYOrigin, _menuItemColor);

        // Thin Line
        painter->DrawFillRectangle(GlobalSettings::LCDWidth - 10, scrollbarYOrigin, 4,
                                   GlobalSettings::LCDHeight - scrollbarYOrigin, _menuTextColor);

        // Draw slider as thick line
        painter->DrawFillRectangle(GlobalSettings::LCDWidth - 14, scrollbarYOffset, 12, sliderHeight, _menuTextColor);
    }

    void SetValue(MenuItem& menuItem)
    {
        switch (menuItem.GetMenuType())
        {
        case MenuItemType::MENU_ITEM_TYPE_SUBMENU:
            menuItem.SetValue(">");
            break;
        default:
            break;
            /*case MenuItemType::MENU_ITEM_TYPE_PAGELINK:
            case MenuItemType::MENU_ITEM_TYPE_BACKLINK:
                break;
            default:
                value = _menuItems[item_index].GetValue();*/
        }
    }

    void UnselectAllMenuItems()
    {
        uint8_t b;
        for (b = 0; b < _menuItemsCount; b++)
        {
            _menuItems[b].SetPressed(false);
        }
    }

    void UnhighlightAllMenuItems()
    {
        uint8_t b;
        for (b = 0; b < _menuItemsCount; b++)
        {
            _menuItems[b].SetHighlighted(false);
        }
    }

    void Update(Button button, int8_t knob, IMenuSystem* menuSystem) override
    {
        if (knob != 0)
        {
            // char debugText[32];
            // sprintf(debugText, "Knob: %d \r\n", knob);
            // _usbDevice->Send((uint8_t*)debugText, 32);

            // Remove highlighting from last selected item as new button event occured
            _menuItems[_menuSelectionIndex].SetHighlighted(false);

            _menuSelectionIndex -= knob;
            _menuSelectionIndex = LimitRange(_menuSelectionIndex, 0, _menuItemsCount - 1);
            _menuItems[_menuSelectionIndex].SetHighlighted(true);
        }

        switch (button)
        {
        case Button::BUTTON_3_UP:
            // Remove highlighting from last selected item as new button event occured
            _menuItems[_menuSelectionIndex].SetHighlighted(false);

            _menuSelectionIndex--;
            _menuSelectionIndex = LimitRange(_menuSelectionIndex, 0, _menuItemsCount - 1);
            _menuItems[_menuSelectionIndex].SetHighlighted(true);
            break;
        case Button::BUTTON_6_UP:
            // Remove highlighting from last selected item as new button event occured
            _menuItems[_menuSelectionIndex].SetHighlighted(false);

            _menuSelectionIndex++;
            _menuSelectionIndex = LimitRange(_menuSelectionIndex, 0, _menuItemsCount - 1);
            _menuItems[_menuSelectionIndex].SetHighlighted(true);
            break;
        case Button::BUTTON_5_UP:
            _menuItems[_menuSelectionIndex].SetPressed(false);
            _menuItems[_menuSelectionIndex].ExecuteAction(menuSystem);
            break;
        case Button::BUTTON_5_DOWN:
            _menuItems[_menuSelectionIndex].SetPressed(true);
            break;
        case Button::E_1_UP:
            _menuItems[_menuSelectionIndex].SetPressed(false);
            _menuItems[_menuSelectionIndex].ExecuteAction(menuSystem);
            break;
        case Button::E_1_DOWN:
            _menuItems[_menuSelectionIndex].SetPressed(true);
            break;
        default:
            break;
        }

        //_usbDevice->Send((uint8_t*)"Knob \r\n", 10);

        UNUSED(menuSystem);
    }
};

#endif // SETTINGSSUBMENU1_H
