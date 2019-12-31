/* 
 * File:   IWidget.h
 * Author: dev
 *
 * Created on 03 August 2019, 23:44
 */

#ifndef IWIDGET_H
#define	IWIDGET_H

#include "../Utils.h"

class Painter;

class IWidget
{
public:

    virtual void Draw(Painter* painter)
    {
        UNUSED(painter);
        
        // Default empty implementation to prevent overhead
    }
};

#endif	/* IWIDGET_H */

