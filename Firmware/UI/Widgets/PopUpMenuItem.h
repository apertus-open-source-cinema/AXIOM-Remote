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
    PopUpMenuItem(CentralDB* centralDB = nullptr, const char* label = "");

    void SetChoices(const char* choicelabels[], uint8_t choices);

    const char* GetChoice(uint8_t choiceindex);
    uint8_t GetChoiceIndex();

    uint8_t GetChoiceCount();

    void UpdateChoice(uint8_t choiceindex);
    void Draw(IPainter* painter) override;
    void ExecuteAction(IMenuSystem* menuSystem) override;
};

#endif /* POPUPMENUITEM_H */
