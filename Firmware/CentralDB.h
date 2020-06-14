#ifndef CENTRALDB_H
#define CENTRALDB_H

#include "Attribute.h"
#include "CentralDBObserver.h"
#include "Utils.h"

#include <cstdint>
#include <cstring>

class CentralDBObserver;

class CentralDB
{
  private:
    struct ObserverAttributePair
    {
        CentralDBObserver* observer;
        Attribute::ID attributeId;
    };
    static const int MAX_OBSERVER_ATTRIBUTE_PAIRS = 16;

    Attribute _attributes[size_t(Attribute::ID::NUM_ATTRIBUTES)];
    ObserverAttributePair _observerAttributePairs[MAX_OBSERVER_ATTRIBUTE_PAIRS];
    uint8_t _observerAttributePairsCount;

  public:
    CentralDB();

    void Attach(const Attribute::ID attributeId, CentralDBObserver* const observer);
    const void* GetValue(Attribute::ID attributeId) const;
    void SetValue(const Attribute::ID attributeId, const void* value);
    void Notify(const Attribute::ID attributeId) const;
};

#endif /* CENTRALDB_H */
