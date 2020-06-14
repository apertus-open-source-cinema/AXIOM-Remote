#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Utils.h"

#include <cstdint>
#include <cstring>

class Attribute
{
  public:
    enum class ID
    {
        UNKNOWN = 0,
        WHITE_BALANCE,
        /* TODO: add more attributes */
        NUM_ATTRIBUTES
    };

    enum class Type
    {
        UNKNOWN = 0,
        INT32,
        UINT32,
        FLOAT,
        STRING
    };

  private:
    const static int MAX_STRING_VALUE_LENGTH = 32;
    const static int MAX_STRING_VALUE_SIZE = MAX_STRING_VALUE_LENGTH + 1;

    const ID _id;
    const Type _type;

    union {
        int32_t int32_val;
        uint32_t uint32_val;
        float float_val;
        char str_val[MAX_STRING_VALUE_SIZE];
    } _value;

  public:
    explicit Attribute(const ID id, const Type& type) : _id(id), _type(type)
    {
    }

    const void* GetValue() const
    {
        switch (_type)
        {
        case Type::INT32:
            return &(_value.int32_val);
        case Type::UINT32:
            return &(_value.uint32_val);
        case Type::FLOAT:
            return &(_value.float_val);
        case Type::STRING:
            return _value.str_val;
        case Type::UNKNOWN:
        default:
            return nullptr;
        }
    }

    void SetValue(const void* value)
    {
        switch (_type)
        {
        case Type::INT32:
            _value.int32_val = *((int32_t*)value);
            break;
        case Type::UINT32:
            _value.uint32_val = *((uint32_t*)value);
            break;
        case Type::FLOAT:
            _value.float_val = *((float*)value);
        case Type::STRING:
            utils::CopyString(_value.str_val, (const char*)value, MAX_STRING_VALUE_LENGTH);
            break;
        case Type::UNKNOWN:
        default:
            return;
        }
    }

    ID GetId() const
    {
        return _id;
    }
};

#endif // ATTRIBUTE_H
