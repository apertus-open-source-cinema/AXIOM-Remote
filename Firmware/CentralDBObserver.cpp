#include "CentralDBObserver.h"

CentralDBObserver::CentralDBObserver(const Attribute::Id attributeId, void (*callback)(const CentralDB&)) :
    _observedAttributeId{attributeId}, _callback{callback}
{
}

Attribute::Id CentralDBObserver::ObservedAttribute() const
{
    return _observedAttributeId;
}

void CentralDBObserver::Update(const CentralDB& db) const
{
    _callback(db);
}
