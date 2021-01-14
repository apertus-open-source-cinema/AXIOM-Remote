#include "CentralDBObserver.h"

// TODO: Investigate -> Clang-Tidy: Pass by value and use std::move
// NOTE: std::move is not available in XC32, reference: https://www.microchip.com/forums/m1089919.aspx
CentralDBObserver::CentralDBObserver(const Attribute::ID attributeID, void (*callback)(const CentralDB&)) :
    _observedAttributeID(attributeID), _callback(callback)
{
}

Attribute::ID CentralDBObserver::ObservedAttribute() const
{
    return _observedAttributeID;
}

void CentralDBObserver::Update(const CentralDB& db) const
{
    _callback(db);
}
