/*
 * File:   IButton.h
 * Author: dev
 *
 * Created on 06 August 2019, 22:08
 */

#ifndef IBUTTON_H
#define IBUTTON_H

#include "IWidget.h"

enum class ButtonState : uint8_t
{
    Default = 0,
    Highlighted = 1,
    Disabled = 2,
};

class IButton : public IWidget
{
    // void* -> sender, e.g. MainPage
    void (*_handlerPtr)(void*);

    // Might want to move these three into implementers instead,
    // to allow for correct sizes

    uint16_t* _colors;
    uint8_t _colorsPerState; // Should really be const.
    uint8_t _stateCount;     // See above.

  protected:
    ButtonState _currentState;
    virtual uint16_t* GetStatePtr() = 0;

  public:
    // TODO: add startState to constructor?
    IButton(uint8_t stateCount, uint8_t colorsPerState, uint16_t* colorPointer, uint16_t x = 0, uint16_t y = 0,
            uint16_t width = 0, uint16_t height = 0) :
        IWidget(x, y, width, height),
        _handlerPtr(nullptr), _currentState(ButtonState::Default), _colors(colorPointer),
        _colorsPerState(colorsPerState), _stateCount(colorsPerState)
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

    // index: every derived class is supposed to define its own enum for colorMeanings

    uint8_t Index(ButtonState state, uint8_t index)
    {
        return static_cast<uint8_t>(state) * _colorsPerState + index;
    }
    void SetColor(ButtonState state, uint8_t index, uint16_t color)
    {
        _colors[static_cast<uint8_t>(state) * _colorsPerState + index] = color;
    }

    uint16_t GetColor(ButtonState state, uint8_t index)
    {
        return GetStatePtr()[static_cast<uint8_t>(state) * _colorsPerState + index];
    }

    uint16_t GetCurrentColor(uint8_t index)
    {
        return GetColor(_currentState, index);
    }

    void SetState(ButtonState state)
    {
        _currentState = state;
    }

    ButtonState GetState()
    {
        return _currentState;
    }
};

#endif /* IBUTTON_H */
