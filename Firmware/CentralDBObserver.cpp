#include "CentralDBObserver.h"

#include <utility>

// TODO: Investigate -> Clang-Tidy: Pass by value and use std::move
// NOTE: std::move is not available in XC32, reference: https://www.microchip.com/forums/m1089919.aspx
CentralDBObserver::CentralDBObserver(const Attribute::ID attribureID, std::function<void(const CentralDB&)> callback) :
    _observedAttributeId{attribureID}, _callback{callback}
{
}

Attribute::ID CentralDBObserver::ObservedAttribute() const
{
    return _observedAttributeId;
}

void CentralDBObserver::Update(const CentralDB& db) const
{
    _callback(db);
}
