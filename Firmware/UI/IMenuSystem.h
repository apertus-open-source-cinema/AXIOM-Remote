#ifndef IMENUSYSTEM_H
#define IMENUSYSTEM_H

#include "MenuDefinitions.h"

class IMenuSystem
{
  public:
    virtual void SetCurrentScreen(AvailableScreens screen)
    {
    }
};

#endif // IMENUSYSTEM_H