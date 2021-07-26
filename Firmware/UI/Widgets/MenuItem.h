#ifndef MENUITEM_H
#define MENUITEM_H

#include "IWidget.h"
#include "../Painter/Painter.h"
#include "../IMenuSystem.h"

#include "../Color565.h"
#include "../../GlobalSettings.h"
#include "../../CentralDB/CentralDB.h"
#include "../../CentralDB/CentralDBObserver.h"

class IPainter;

enum class MenuItemType
{
    MENU_ITEM_TYPE_READONLY,
    MENU_ITEM_TYPE_NUMERIC,
    MENU_ITEM_TYPE_DROPDOWN,
    MENU_ITEM_TYPE_LIST,
    MENU_ITEM_TYPE_CHECKBOX,
    MENU_ITEM_TYPE_LINK,
    MENU_ITEM_TYPE_NONE
};

class MenuItem : public IWidget
{
  protected:
    CentralDB* _db;

    bool _disabled;
    bool _hidden;
    bool _pressed;
    bool _highlighted;
    const char* _label;
    const char* _value;
    MenuItemType _type;

    uint16_t _backgroundColor;
    uint16_t _backgroundHighlightColor;
    uint16_t _backgroundPressedColor;
    // uint16_t _backgroundDimmedColor;
    uint16_t _backgroundDisabledColor;

    uint16_t _textColor;
    uint16_t _textHighlightColor;
    uint16_t _textPressedColor;
    // uint16_t _textDimmedColor;
    uint16_t _textDisabledColor;

    uint16_t _currentBackgroundColor;
    uint16_t _currentTextColor;

    uint8_t _verticalLabelOffset;

    void (*_handlerPtr)(void*, CentralDB*);

  public:
    MenuItem(CentralDB* centralDB = nullptr, const char* label = "...", bool disabled = false,
             const char* value = nullptr, bool hidden = false, bool pressed = false, bool highlighted = false,
             MenuItemType type = MenuItemType::MENU_ITEM_TYPE_NONE);

    virtual void SetHandler(void (*handler)(void*, CentralDB*));

    void Activate(void* sender);

    void SetDisabled(bool disabled);
    bool IsDisabled() const;

    void SetHidden(bool hide);
    bool IsHidden() const;

    void SetPressed(bool pressed);
    bool IsPressed() const;

    void SetHighlighted(bool highlighted);
    bool IsHighlighted() const;

    void SetLabel(const char* value);
    const char* GetLabel() const;

    void SetValue(const char* value);
    const char* GetValue() const;

    void SetMenuType(MenuItemType type);
    MenuItemType GetMenuType() const;

    void SetY(uint16_t y);

    void Draw(IPainter* painter) override;

    virtual void ExecuteAction(IMenuSystem* menuSystem);
};

#endif /* MENUITEM_H */
