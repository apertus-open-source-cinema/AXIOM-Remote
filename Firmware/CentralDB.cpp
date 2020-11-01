#include "CentralDB.h"

#include <algorithm>
#include "CentralDB.h"

#include <algorithm>
#include <array>

CentralDB::CentralDB() :
    _observers{}, _observerCount{0}, _attributes{{Attribute::Id::FRAME_RATE, Attribute::Type::FLOAT},
                                                 {Attribute::Id::APERTURE, Attribute::Type::FLOAT},
                                                 {Attribute::Id::WHITE_BALANCE, Attribute::Type::UINT32},
                                                 {Attribute::Id::TEST_STRING, Attribute::Type::STRING},
                                                 {Attribute::Id::TEST_BOOLEAN, Attribute::Type::BOOLEAN},
                                                 {Attribute::Id::TEST_INT32, Attribute::Type::INT32}}
    // TODO: add any missing attributes
{
}

void CentralDB::Attach(CentralDBObserver* const observer)
{
    if (observer && _observerCount < MAX_NUM_OBSERVERS)
    {
        // Only add observers of attributes that exist in the DB
        if (GetAttribute(observer->ObservedAttribute()))
        {
            _observers[_observerCount] = observer;
            _observerCount++;
        }
    }
}

Attribute* CentralDB::GetAttribute(const Attribute::Id& attributeId)
{
    return std::find_if(std::begin(_attributes), std::end(_attributes),
                        [&attributeId](const Attribute& attribute) { return attribute.GetId() == attributeId; });
}

const Attribute* CentralDB::GetAttribute(const Attribute::Id& attributeId) const
{
    return std::find_if(std::begin(_attributes), std::end(_attributes),
                        [&attributeId](const Attribute& attribute) { return attribute.GetId() == attributeId; });
}

const void* CentralDB::GetValue(const Attribute::Id& attributeId, const Attribute::Type& type) const
{
    const Attribute* attribute = GetAttribute(attributeId);

    return attribute ? attribute->GetValue(type) : Attribute::DefaultValue(type);
}

void CentralDB::SetValue(const Attribute::Id& attributeId, const Attribute::Type& type, const void* value)
{
    auto attribute = GetAttribute(attributeId);

    if (attribute && attribute->SetValue(type, value))
    {
        Notify(attributeId);
    }
}

bool CentralDB::GetBoolean(const Attribute::Id& attributeId) const
{
    return *((bool*)GetValue(attributeId, Attribute::Type::BOOLEAN));
}

int32_t CentralDB::GetInt32(const Attribute::Id& attributeId) const
{
    return *((int32_t*)GetValue(attributeId, Attribute::Type::INT32));
}

uint32_t CentralDB::GetUint32(const Attribute::Id& attributeId) const
{
    return *((uint32_t*)GetValue(attributeId, Attribute::Type::UINT32));
}

float CentralDB::GetFloat(const Attribute::Id& attributeId) const
{
    return *((float*)GetValue(attributeId, Attribute::Type::FLOAT));
}

const char* CentralDB::GetString(const Attribute::Id& attributeId) const
{
    return (char*)GetValue(attributeId, Attribute::Type::STRING);
}

void CentralDB::SetBoolean(const Attribute::Id& attributeId, const bool value)
{
    SetValue(attributeId, Attribute::Type::BOOLEAN, (void*)(&value));
}

void CentralDB::SetInt32(const Attribute::Id& attributeId, const int32_t value)
{
    SetValue(attributeId, Attribute::Type::INT32, (void*)(&value));
}

void CentralDB::SetUint32(const Attribute::Id& attributeId, const uint32_t value)
{
    SetValue(attributeId, Attribute::Type::UINT32, (void*)(&value));
}

void CentralDB::SetFloat(const Attribute::Id& attributeId, const float value)
{
    SetValue(attributeId, Attribute::Type::FLOAT, (void*)(&value));
}

void CentralDB::SetString(const Attribute::Id& attributeId, const char* value)
{
    SetValue(attributeId, Attribute::Type::STRING, (void*)(value));
}

void CentralDB::Notify(const Attribute::Id& attributeId) const
{
    for (uint8_t i = 0; i < _observerCount; ++i)
    {
        if (attributeId == _observers[i]->ObservedAttribute())
        {
            _observers[i]->Update(*this);
        }
    }
}
