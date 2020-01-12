#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <stdint.h>

// Ugly hack to allow access for every module, needs rework, settings will get own memory section
class GlobalSettings
{
  public:
    static uint8_t brightnessPercentage;
    static uint16_t LCDWidth;
    static uint8_t LCDHeight;
};

#endif /* GLOBALSETTINGS_H */
