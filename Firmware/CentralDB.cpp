#include "CentralDB.h"

void CentralDB::Notify()
{
    for (uint8_t i = 0; i < _viewscount; i++)
    {
        _views[i]->update();
    }
}