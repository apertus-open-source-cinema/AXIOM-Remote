#include "CentralDB.h"

#include <algorithm>
#include <array>

CentralDB::CentralDB() :
    _attributes{Attribute(Attribute::ID::UNKNOWN, Attribute::Type::UNKNOWN),
                Attribute(Attribute::ID::WHITE_BALANCE, Attribute::Type::STRING)},
    _observerAttributePairsCount{0}
{
}

void CentralDB::Attach(const Attribute::ID attributeId, CentralDBObserver* const observer)
{
    if (_observerAttributePairsCount < MAX_OBSERVER_ATTRIBUTE_PAIRS)
    {
        _observerAttributePairs[_observerAttributePairsCount].attributeId = attributeId;
        _observerAttributePairs[_observerAttributePairsCount].observer = observer;
        _observerAttributePairsCount++;
    }
}

const void* CentralDB::GetValue(const Attribute::ID attributeId) const
{
    auto attribute =
        std::find_if(std::begin(_attributes), std::end(_attributes),
                     [attributeId](const Attribute& attribute) { return attribute.GetId() == attributeId; });

    return attribute ? attribute->GetValue() : nullptr;
}

void CentralDB::SetValue(const Attribute::ID attributeId, const void* value)
{
    auto attribute =
        std::find_if(std::begin(_attributes), std::end(_attributes),
                     [attributeId](const Attribute& attribute) { return attribute.GetId() == attributeId; });

    if (attribute)
    {
        attribute->SetValue(value);
        Notify(attributeId);
    }
}

void CentralDB::Notify(const Attribute::ID attributeId) const
{
    auto observerAttributePair = std::find_if(
        std::begin(_observerAttributePairs), std::begin(_observerAttributePairs) + _observerAttributePairsCount,
        [attributeId](const ObserverAttributePair& pair) { return pair.attributeId == attributeId && pair.observer; });

    if (observerAttributePair)
    {
        observerAttributePair->observer->Update(*this);
    }
}
