#include "CentralDBObserver.h"

#include <utility>

CentralDBObserver::CentralDBObserver(const Attribute::ID attribureID, std::function<void(const CentralDB&)> callback) :
    _observedAttributeId{attribureID}, _callback{std::move(callback)}
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
