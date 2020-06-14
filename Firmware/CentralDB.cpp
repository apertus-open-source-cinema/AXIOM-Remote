#include "CentralDB.h"

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
    for (auto& attribute : _attributes)
    {
        if (attribute.GetId() == attributeId)
        {
            return attribute.GetValue();
        }
    }
    return nullptr;
}

void CentralDB::SetValue(const Attribute::ID attributeId, const void* value)
{
    for (auto& attribute : _attributes)
    {
        if (attribute.GetId() == attributeId)
        {
            attribute.SetValue(value);
            Notify(attributeId);
            return;
        }
    }
}

void CentralDB::Notify(const Attribute::ID attributeId) const
{
    for (uint8_t i = 0; i < _observerAttributePairsCount; i++)
    {
        if (_observerAttributePairs[i].attributeId == attributeId && _observerAttributePairs[i].observer)
        {
            _observerAttributePairs[i].observer->Update(this);
        }
    }
}
