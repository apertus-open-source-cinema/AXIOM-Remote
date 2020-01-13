#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "IMenu.h"
#include "../Painter.h"
#include "../Widgets/MenuItem.h"

//#include "../Widgets/MainPageButton.h"

#include "../ButtonDefinitions.h"

#include "../../GlobalSettings.h"

#include "../../../Bootloader/Periphery/USB/IUSBDevice.h"

#include <Helpers.h>

class SettingsMenu : public IMenu
{
    IUSBDevice* _usbDevice;

    char* _label;
    char* _menuBreadcrumbs;
    MenuItem _menuItems[3];
    uint8_t _menuSelectionIndex;
    uint8_t _menuItemsCount;
    uint8_t _parameterSelectionIndex;
    uint8_t _parameterMenuActive;
    uint8_t _menuOffset;

    // Color Defintions
    Color565 _menuBackgroundColor;
    Color565 _menuItemColor;
    Color565 _menuDimmedItemColor;
    Color565 _menuDisabledItemColor;
    Color565 _menuSelectedItemColor;
    Color565 _menuHightlightedItemColor;
    Color565 _menuTextColor;
    Color565 _menuDisabledTextColor;
    Color565 _menuSelectedTextColor;

  public:
    explicit SettingsMenu(IUSBDevice* cdcDevice)
    {
        UNUSED(cdcDevice);

        _label = "Settings Menu";
        _menuItemsCount = 3;
        _menuSelectionIndex = 0;

        _menuItems[0] = new MenuItem();
        _menuItems[0].SetLabel("Test Item 1");

        _menuItems[1] = new MenuItem();
        _menuItems[1].SetDisabled(true);
        _menuItems[1].SetLabel("Test Item 2");

        // Color defintions
        _menuBackgroundColor = Color565::LightGrey;
        _menuItemColor = Color565::White;
        _menuSelectedTextColor = Color565::White;

        // init menu selection indexes
        _parameterMenuActive = 0;
        _parameterSelectionIndex = 0;
        _menuOffset = 0;

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

    char* GetLabel()
    {
        return _label;
    }

    int8_t GetMenuItemsCount()
    {
        return _menuItemsCount;
    }

  protected:
    void Draw(Painter* painter) override
    {

        // clear the screen
        painter->DrawFillRectangle(0, 0, GlobalSettings::LCDWidth, GlobalSettings::LCDHeight, _menuBackgroundColor);

        // draw header background
        painter->DrawFillRectangle(0, GlobalSettings::LCDHeight - 28, GlobalSettings::LCDWidth, 28, _menuItemColor);

        /*
        // draw header bread crumbs
        draw_string(5, FRAMEBUFFER_HEIGHT - 22, menu_breadcrumbs, menu_text_color, menu_text_color, Font::FreeSans9pt7b,
                    TextAlign::TEXT_ALIGN_LEFT, 0);
*/
        // two header separation lines
        painter->DrawLine(0, GlobalSettings::LCDHeight - 29, GlobalSettings::LCDWidth - 1,
                          GlobalSettings::LCDHeight - 29, _menuSelectedTextColor);

        painter->DrawLine(0, GlobalSettings::LCDHeight - 30, GlobalSettings::LCDWidth - 1,
                          GlobalSettings::LCDHeight - 30, _menuBackgroundColor);

        for (int8_t i = 0; i < _menuItemsCount; i++)
        {
            //_menuItems[i].Draw(painter);
            painter->DrawFillRectangle(0, GlobalSettings::LCDHeight - 30 - i * 30, 100, 25, _menuItemColor);
            painter->DrawFillRectangle(0, GlobalSettings::LCDHeight - 30 - i * 30, 100, 25, _menuItemColor);
        }

        // todo fix me: just example values for now
        // DrawScrollIndicator(1, 2, &painter);
    }

    void DrawScrollIndicator(uint8_t current_menu_item_screen_count, uint8_t current_menu_item_count, Painter* painter)
    {
        // maximum height is the screen without header area
        uint8_t srollbar_max_framebuffer_height = GlobalSettings::LCDHeight - 30;

        // height of the scroll indicator is defined by the ratio of number of items on screen vs total number of item.
        // if there are 7 items on screen of total 14 items the scroll indicator shall be 50% of the scrollbar height
        uint8_t scrollbarheight = srollbar_max_framebuffer_height *
                                  (float)((float)current_menu_item_screen_count / (float)(current_menu_item_count));

        //
        uint8_t scrollbaroffset = ((current_menu_item_count - current_menu_item_screen_count) - _menuOffset) *
                                  ((srollbar_max_framebuffer_height - scrollbarheight) /
                                   (current_menu_item_count - current_menu_item_screen_count));

        // Background
        painter->DrawFillRectangle(GlobalSettings::LCDWidth - 16, 0, 16, GlobalSettings::LCDHeight - 30,
                                   _menuTextColor);

        // Thin Line
        painter->DrawFillRectangle(GlobalSettings::LCDWidth - 9, 0, 4, GlobalSettings::LCDHeight - 30, _menuTextColor);

        // Thick Line
        painter->DrawFillRectangle(GlobalSettings::LCDWidth - 13, scrollbaroffset, 12, scrollbarheight, _menuTextColor);
    }

    void DrawMenuItem(uint16_t x, uint16_t y, uint8_t menu_main_item_index, Painter* painter)
    {
        uint16_t yoffset_label_from_base = 7;

        // don't draw empty items
        if (_label == NULL)
        {
            return;
        }

        char* value;

        if (_menuItems[menu_main_item_index].GetMenuType() == MenuItemType::MENU_ITEM_TYPE_SUBMENU)
        {
            // submenu items are indicated by a ">" sign
            sprintf(value, ">");
        } else if (_menuItems[menu_main_item_index].GetMenuType() == MenuItemType::MENU_ITEM_TYPE_PAGELINK)
        {
            // page links have no value icon displayed
            sprintf(value, "");
        } else if (_menuItems[menu_main_item_index].GetMenuType() == MenuItemType::MENU_ITEM_TYPE_BACKLINK)
        {
            // backlinks point to a menu higher in the menu hierarchy
            sprintf(value, "");
        } else
        {
            /* if (_menuItems[menu_main_item_index].current_value_ptr != NULL)
             {
                 // strcpy(value, (*main_menu[menu_index].menu_item[menu_main_item_index].current_value_ptr)(menu_index,
                 // menu_main_item_index));

                 // sprintf(value, "%s",
                 // (*main_menu[menu_index].menu_item[menu_main_item_index].current_value_ptr)(menu_index,
                 // menu_main_item_index));
             } else
             {*/
            sprintf(value, "%d", _menuItems[menu_main_item_index].GetValue());
            // }
        }

        // is the current line highlighted and not disabled?
        if (_menuItems[menu_main_item_index].IsHighlighted() && !_menuItems[menu_main_item_index].IsDisabled())
        {
            painter->DrawFillRectangle(x, y, GlobalSettings::LCDWidth - x, 29, _menuHightlightedItemColor);

            // label
            painter->DrawText(x + 5, y + yoffset_label_from_base, _menuItems[menu_main_item_index].GetLabel(),
                              _menuSelectedTextColor, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_LEFT, 0);

            // value
            painter->DrawText(x + 180, y + yoffset_label_from_base, value, _menuSelectedTextColor, Font::FreeSans9pt7b,
                              TextAlign::TEXT_ALIGN_LEFT, 80);
            return;
        }

        // is a parameter menu active currently and the item is disabled?
        if (_parameterMenuActive && _menuItems[menu_main_item_index].IsDisabled())
        {
            painter->DrawFillRectangle(x, y, GlobalSettings::LCDWidth - x, 29, _menuDimmedItemColor);

            // label
            painter->DrawText(x + 5, y + yoffset_label_from_base, _menuItems[menu_main_item_index].GetLabel(),
                              _menuDisabledTextColor, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_LEFT, 0);

            // value
            painter->DrawText(x + 180, y + yoffset_label_from_base, value, _menuDisabledTextColor, Font::FreeSans9pt7b,
                              TextAlign::TEXT_ALIGN_LEFT, 80);
            return;
        }

        // is a parameter menu active currently?
        if (_parameterMenuActive)
        {
            painter->DrawFillRectangle(x, y, GlobalSettings::LCDWidth - x, 29, _menuDimmedItemColor);

            // label
            painter->DrawText(x + 5, y + yoffset_label_from_base, _menuItems[menu_main_item_index].GetLabel(),
                              _menuTextColor, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_LEFT, 0);

            // value
            painter->DrawText(x + 180, y + yoffset_label_from_base, value, _menuTextColor, Font::FreeSans9pt7b,
                              TextAlign::TEXT_ALIGN_RIGHT, 80);
            return;
        }

        // is the current line selected and disabled?
        if (_menuItems[menu_main_item_index].IsSelected() && _menuItems[menu_main_item_index].IsDisabled())
        {
            painter->DrawFillRectangle(x, y, GlobalSettings::LCDWidth - x, 29, _menuDisabledItemColor);

            // disabled indicators on the side
            painter->DrawFillRectangle(30, y, 4, 29, _menuSelectedTextColor);
            painter->DrawFillRectangle(GlobalSettings::LCDWidth - 16 - 4, y, 4, 29, _menuSelectedTextColor);

            // label
            painter->DrawText(x + 5, y + yoffset_label_from_base, _menuItems[menu_main_item_index].GetLabel(),
                              _menuDisabledTextColor, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_LEFT, 0);

            // value
            painter->DrawText(x + 180, y + yoffset_label_from_base, value, _menuDisabledTextColor, Font::FreeSans9pt7b,
                              TextAlign::TEXT_ALIGN_RIGHT, 80);
            return;
        }

        // is the current line disabled?
        if (_menuItems[menu_main_item_index].IsDisabled())
        {
            painter->DrawFillRectangle(x, y, GlobalSettings::LCDWidth - x, 29, _menuDisabledTextColor);

            // label
            painter->DrawText(x + 5, y + yoffset_label_from_base, _menuItems[menu_main_item_index].GetLabel(),
                              _menuDisabledTextColor, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_LEFT, 0);

            // value
            painter->DrawText(x + 180, y + yoffset_label_from_base, value, _menuDisabledTextColor, Font::FreeSans9pt7b,
                              TextAlign::TEXT_ALIGN_RIGHT, 80);
            return;
        }

        // is the current line selected (cursor)?
        if (_menuItems[menu_main_item_index].IsSelected())
        {
            painter->DrawFillRectangle(x, y, GlobalSettings::LCDWidth - x, 29, _menuSelectedTextColor);

            // value
            painter->DrawText(x + 5, y + yoffset_label_from_base, _menuItems[menu_main_item_index].GetLabel(),
                              _menuSelectedTextColor, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_LEFT, 0);

            // label
            painter->DrawText(x + 180, y + yoffset_label_from_base, value, _menuSelectedTextColor, Font::FreeSans9pt7b,
                              TextAlign::TEXT_ALIGN_RIGHT, 80);
            return;
        }

        // if nothing of the above applies simply draw the line item normally
        painter->DrawFillRectangle(x, y, GlobalSettings::LCDWidth - x, 29, _menuItemColor);

        // label
        painter->DrawText(x + 5, y + yoffset_label_from_base, _menuItems[menu_main_item_index].GetLabel(),
                          _menuTextColor, Font::FreeSans9pt7b, TextAlign::TEXT_ALIGN_LEFT, 0);

        // value
        painter->DrawText(x + 180, y + yoffset_label_from_base, value, _menuTextColor, Font::FreeSans9pt7b,
                          TextAlign::TEXT_ALIGN_RIGHT, 80);
    }

    void UnselectAllMenuItems()
    {
        uint8_t b;
        for (b = 0; b < _menuItemsCount; b++)
        {
            _menuItems[b].SetSelected(false);
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

    void Update(Button button, IMenuSystem* menuSystem) override
    {
        UNUSED(button);
        UNUSED(menuSystem);
    }
};

#endif // SETTINGSMENU_H
