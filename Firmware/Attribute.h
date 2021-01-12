#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Utils.h"
#include <cstdint>

class Attribute
{
    friend class CentralDB;

  public:
    enum class ID : std::uint8_t
    {
        FRAME_RATE = 0,
        WHITE_BALANCE,
        APERTURE,
        REMOTE_LCD_BRIGHTNESS,
        TEST_STRING,
        TEST_BOOLEAN,
        TEST_INT32,
        /* TODO: adjust accordingly, i.e. add more attributes */
        NUM_ATTRIBUTES // DO NOT USE: referenced only to count the total number of attributes
    };

    enum class Type
    {
        BOOLEAN = 0,
        INT32,
        UINT32,
        FLOAT,
        STRING
    };

  private:
    static const int MAX_STRING_DATA_LENGTH = 32;
    static const int STRING_VALUE_SIZE = MAX_STRING_DATA_LENGTH + 1; // 32 for data, 1 for null terminator

    static const bool DEFAULT_VALUE_BOOLEAN;
    static const int32_t DEFAULT_VALUE_INT32;
    static const uint32_t DEFAULT_VALUE_UINT32;
    static const float DEFAULT_VALUE_FLOAT;
    static const char DEFAULT_VALUE_STRING[STRING_VALUE_SIZE];

    const ID _id;
    const Type _type;

    union {
        bool bool_val;
        int32_t int32_val;
        uint32_t uint32_val;
        float float_val;
        char str_val[STRING_VALUE_SIZE];
    } _value;

  public:
    Attribute(const ID& id, const Type& type) noexcept : _id(id), _type(type)
    {
        memset(_value.str_val, 0, STRING_VALUE_SIZE);
    }

    ID GetId() const noexcept
    {
        return _id;
    }

    const void* GetValue(const Type& type) const noexcept
    {
        if (type != _type)
        {
            return DefaultValue(type);
        }

        switch (_type)
        {
        case Type::BOOLEAN:
            return &(_value.bool_val);
        case Type::INT32:
            return &(_value.int32_val);
        case Type::UINT32:
            return &(_value.uint32_val);
        case Type::FLOAT:
            return &(_value.float_val);
        case Type::STRING:
            return _value.str_val;
        default:
            return nullptr;
        }
    }

    bool SetValue(const Type& type, const void* value) noexcept
    {
        if (type != _type)
        {
            return false;
        }

        switch (_type)
        {
        case Type::BOOLEAN:
            _value.bool_val = *((bool*)value);
            break;
        case Type::INT32:
            _value.int32_val = *((int32_t*)value);
            break;
        case Type::UINT32:
            _value.uint32_val = *((uint32_t*)value);
            break;
        case Type::FLOAT:
            _value.float_val = *((float*)value);
            break;
        case Type::STRING:
            utils::CopyString(_value.str_val, (const char*)value, MAX_STRING_DATA_LENGTH);
            break;
        default:
            return false;
        }

        return true;
    }

    static const void* DefaultValue(const Type& type) noexcept
    {
        switch (type)
        {
        case Type::BOOLEAN:
            return &DEFAULT_VALUE_BOOLEAN;
        case Type::INT32:
            return &DEFAULT_VALUE_INT32;
        case Type::UINT32:
            return &DEFAULT_VALUE_UINT32;
        case Type::FLOAT:
            return &DEFAULT_VALUE_FLOAT;
        case Type::STRING:
            return DEFAULT_VALUE_STRING;
        default:
            return nullptr;
        }
    }
};

#endif // ATTRIBUTE_H
