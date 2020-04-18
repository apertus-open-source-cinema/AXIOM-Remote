/* 
 * File:   AnalogGainMenu.h
 * Author: dev
 *
 * Created on 09 August 2019, 16:38
 */

#ifndef ANALOGGAINMENU_H
#define	ANALOGGAINMENU_H

#include "IMenu.h"

class AnalogGainMenu : public IMenu
{
public:

    AnalogGainMenu()
    {

    }

    void Draw(IPainter* painter) override
    {
//        painter->DrawImage(apertus_logo.pixel_data, 58, 89, apertus_logo.width, apertus_logo.height);
//
//        for(uint8_t index = 0; index < 6; index++)
//        {
//            IWidget* widget = _widgetArray[index];
//            if(widget == nullptr)
//            {
//                return;
//            }
//
//            widget->Draw(painter);
//        }
    }
};


#endif	/* ANALOGGAINMENU_H */

