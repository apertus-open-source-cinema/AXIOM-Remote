#ifndef CENTRALDB_H
#define CENTRALDB_H

#include <cstdint>

#include "Attribute.h"
#include "CentralDBObserver.h"
#include "../Utils.h"

class CentralDBObserver;

class CentralDB
{
    static const size_t MAX_NUM_OBSERVERS = 16; // TODO: adjust accordingly

    struct ObserverAttributePair
    {
        CentralDBObserver* observer;
        Attribute::ID attributeID;
    };

    CentralDBObserver* _observers[MAX_NUM_OBSERVERS];
    uint8_t _observerCount;
    Attribute _attributes[(size_t)Attribute::ID::NUM_ATTRIBUTES];

    Attribute* GetAttribute(const Attribute::ID& attributeID);
    const Attribute* GetAttribute(const Attribute::ID& attributeID) const;
    const void* GetValue(const Attribute::ID& attributeID, const Attribute::Type& type) const;
    void SetValue(const Attribute::ID& attributeID, const Attribute::Type& type, const void* value);

  public:
    CentralDB();

    void Attach(CentralDBObserver* const observer);

    // Attribute value getters. Getting the value for a type inapplicable for the attribute returns the default value
    // for that type
    bool GetBoolean(const Attribute::ID& attributeID) const;
    int32_t GetInt32(const Attribute::ID& attributeID) const;
    uint32_t GetUint32(const Attribute::ID& attributeID) const;
    float GetFloat(const Attribute::ID& attributeID) const;
    const char* GetString(const Attribute::ID& attributeID) const;

    // Attribute value setters. Setting the value for a type inapplicable for the attribute has no effect
    void SetBoolean(const Attribute::ID& attributeID, const bool value);
    void SetInt32(const Attribute::ID& attributeID, const int32_t value);
    void SetUint32(const Attribute::ID& attributeID, const uint32_t value);
    void SetFloat(const Attribute::ID& attributeID, const float value);
    void SetString(const Attribute::ID& attributeID, const char* value);

    void Notify(const Attribute::ID& attributeID) const;
};

#endif /* CENTRALDB_H */
