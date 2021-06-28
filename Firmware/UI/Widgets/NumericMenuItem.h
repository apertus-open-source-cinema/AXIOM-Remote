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
                    int16_t maxRange = 100, int16_t stepsize = 1, const char* suffix = "");

    void SetValue(int16_t value);

    void SetSuffix(const char* value);

    char const* GetSuffix();

    void SetStepSize(int16_t newstepsize);
    int16_t GetStepSize();

    int16_t GetMaxRange();
    int16_t GetMinRange();

    int16_t GetValue();

    void Draw(IPainter* painter) override;

    virtual void ExecuteAction(IMenuSystem* menuSystem) override;
};

#endif /* NUMERICMENUITEM_H */
