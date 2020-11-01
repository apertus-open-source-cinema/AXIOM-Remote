#ifndef CENTRALDB_H
#define CENTRALDB_H

#include "Attribute.h"
#include "CentralDBObserver.h"
#include "Utils.h"

#include <cstdint>

class CentralDBObserver;

class CentralDB
{
    static const size_t MAX_NUM_OBSERVERS = 16; // TODO: adjust accordingly

    struct ObserverAttributePair
    {
        CentralDBObserver* observer;
        Attribute::Id attributeId;
    };

    CentralDBObserver* _observers[MAX_NUM_OBSERVERS];
    uint8_t _observerCount;
    Attribute _attributes[(size_t)Attribute::Id::NUM_ATTRIBUTES];

    Attribute* GetAttribute(const Attribute::Id& attributeId);
    const Attribute* GetAttribute(const Attribute::Id& attributeId) const;
    const void* GetValue(const Attribute::Id& attributeId, const Attribute::Type& type) const;
    void SetValue(const Attribute::Id& attributeId, const Attribute::Type& type, const void* value);

  public:
    CentralDB();

    void Attach(CentralDBObserver* const observer);

    // Attribute value getters. Getting the value for a type inapplicable for the attribute returns the default value
    // for that type
    bool GetBoolean(const Attribute::Id& attributeId) const;
    int32_t GetInt32(const Attribute::Id& attributeId) const;
    uint32_t GetUint32(const Attribute::Id& attributeId) const;
    float GetFloat(const Attribute::Id& attributeId) const;
    const char* GetString(const Attribute::Id& attributeId) const;

    // Attribute value setters. Setting the value for a type inapplicable for the attribute has no effect
    void SetBoolean(const Attribute::Id& attributeId, const bool value);
    void SetInt32(const Attribute::Id& attributeId, const int32_t value);
    void SetUint32(const Attribute::Id& attributeId, const uint32_t value);
    void SetFloat(const Attribute::Id& attributeId, const float value);
    void SetString(const Attribute::Id& attributeId, const char* value);

    void Notify(const Attribute::Id& attributeId) const;
};

#endif /* CENTRALDB_H */
