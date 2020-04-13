#ifndef PAINTERDECORATOR_H
#define PAINTERDECORATOR_H

#include <stdint.h>

#include "Painter.h"

template <class T>
class PainterDecorator : public T
{
    T* _decoratedPainter;

  public:
    PainterDecorator(T* painter) : _decoratedPainter(painter)
    {
    }

    T& GetPainter()
    {
        return *_decoratedPainter;
    }
};

#endif // PAINTERDECORATOR_H
