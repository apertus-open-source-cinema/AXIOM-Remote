/* 
 * File:   IMenu.h
 * Author: dev
 *
 * Created on 23 July 2019, 18:49
 */

#ifndef IMENU_H
#define	IMENU_H

//#include <Helpers.h>
#include <stdint.h>

#include <Helpers.h>

//uint16_t RGB565(uint8_t red, uint8_t green, uint8_t blue);

class Painter;
enum class Button;

class IMenu
{
public:

    virtual uint16_t GetBackgroundColor()
    {
        return RGB565(180, 180, 180);
    }

    virtual void Draw(Painter* painter)
    {

    }

    virtual void Update(Button button)
    {

    }
};

#endif	/* IMENU_H */

