#ifndef MENUITEM_H
#define MENUITEM_H

#include "IButton.h"
#include "../Painter.h"

#include "../Color565.h"

enum class MenuItemType
{
    MENU_ITEM_TYPE_SUBMENU,
    MENU_ITEM_TYPE_PAGELINK,
    MENU_ITEM_TYPE_BACKLINK,
    MENU_ITEM_TYPE_READONLY,
    MENU_ITEM_TYPE_NUMERIC,
    MENU_ITEM_TYPE_DROPDOWN
};

class MenuItem : public IWidget
{
    bool _disabled;
    bool _hidden;
    bool _selected;
    bool _highlighted;
    char* _label;
    MenuItemType _type;

  public:
    MenuItem(bool disabled = false, bool hidden = false, bool selected = false, bool highlighted = false,
             const char* label = "...", MenuItemType type = MenuItemType::MENU_ITEM_TYPE_NUMERIC) :
        _disabled(disabled),
        _hidden(hidden), _selected(selected), _highlighted(highlighted), _label(const_cast<char*>(label)), _type(type)
    {
    }

    void SetDisabled(bool disabled)
    {
        _disabled = disabled;
    }

    bool IsDisabled()
    {
        return _disabled;
    }

    void SetHidden(bool hide)
    {
        _hidden = hide;
    }

    bool IsHidden()
    {
        return _hidden;
    }

    void SetSelected(bool selected)
    {
        _selected = selected;
    }

    bool IsSelected()
    {
        return _selected;
    }

    void SetHighlighted(bool highlighted)
    {
        _highlighted = highlighted;
    }

    bool IsHighlighted()
    {
        return _highlighted;
    }

    void SetLabel(const char* value)
    {
        _label = const_cast<char*>(value);
    }

    char* GetLabel()
    {
        return _label;
    }

    void SetMenuType(MenuItemType type)
    {
        _type = type;
    }

    MenuItemType GetMenuType()
    {
        return _type;
    }
};

#endif /* MENUITEM_H */
