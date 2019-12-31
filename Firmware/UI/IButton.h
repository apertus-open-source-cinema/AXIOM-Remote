/*
 * File:   IButton.h
 * Author: dev
 *
 * Created on 06 August 2019, 22:08
 */

#ifndef IBUTTON_H
#define IBUTTON_H

#include "IWidget.h"

class IButton : public IWidget {
    // void* -> sender, e.g. MainMenu
    void (*_handlerPtr)(void*);

  public:
    IButton() : _handlerPtr(nullptr)
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
