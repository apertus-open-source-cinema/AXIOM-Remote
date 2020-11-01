#ifndef IDEBUGPAINTER_H
#define IDEBUGPAINTER_H

#include <stdint.h>

#include "../FontDefinitions.h"

class IPainter;

class IDebugPainter
{
  public:
    // Images
    virtual void DrawIcon(IPainter* painter, const Icon* image, uint16_t x, uint16_t y, uint16_t color) = 0;

    // Text Related
    virtual void DrawText(IPainter* painter, uint16_t x, uint16_t y, const char* text, uint16_t color, TextAlign align,
                          uint16_t textblockwidth) = 0;
};

#endif /* IDEBUGPAINTER_H */
