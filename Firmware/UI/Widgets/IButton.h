/*
 * File:   IButton.h
 * Author: dev
 *
 * Created on 06 August 2019, 22:08
 */

#ifndef IBUTTON_H
#define IBUTTON_H

#include "IWidget.h"

class IButton : public IWidget
{
    // void* -> sender, e.g. MainPage
    void (*_handlerPtr)(void*);

  public:
    IButton(uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IWidget(x, y, width, height), _handlerPtr(nullptr)
    {
    }

    virtual void SetHandler(void (*handler)(void*))
    {
        _handlerPtr = handler;
    }

    void Activate(void* sender)
    {
        _handlerPtr(sender);
    }
};

#endif /* IBUTTON_H */
