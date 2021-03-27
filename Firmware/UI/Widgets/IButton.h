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
    static constexpr uint8_t _colorsPerState = 4;
    static constexpr uint8_t _stateCount = 3;
    uint16_t _colors[_stateCount][_colorsPerState];
    ButtonState _currentState;

  public:
    // TODO: add startState to constructor?
    IButton(uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) :
        IWidget(x, y, width, height), _handlerPtr(nullptr), _currentState(ButtonState::Default)
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

    // index: every subclass is supposed to define own enum for colorMeanings
    void SetColor(ButtonState state, uint8_t index, uint16_t color)
    {
        _colors[static_cast<uint8_t>(state)][index] = color;
    }

    uint16_t GetColor(ButtonState state, uint8_t index)
    {
        return _colors[static_cast<uint8_t>(state)][index];
    }

    uint16_t GetCurrentColor(uint8_t index)
    {
        return _colors[static_cast<uint8_t>(_currentState)][index];
    }

    void SetState(ButtonState state)
    {
        // TODO: checks to see if valid transition, transition function?
        _currentState = state;
    }
};

#endif /* IBUTTON_H */
