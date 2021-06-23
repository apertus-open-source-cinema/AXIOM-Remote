#include "catch2/catch.hpp"
#include <iostream>
#include "../Firmware/CentralDB/CentralDB.h"

static int num_callback_invocations = 0;

void dummy_callback(const CentralDB& /*db*/)
{
    num_callback_invocations++;
}

TEST_CASE("Observer callback invocation")
{
    CentralDB db;

    CentralDBObserver observer(Attribute::ID::WHITE_BALANCE, dummy_callback);

    db.Attach(&observer);

    const int initial_invocations = num_callback_invocations;
    db.SetUint32(Attribute::ID::WHITE_BALANCE, 123);
    const int final_invocations = num_callback_invocations;

    REQUIRE(final_invocations == initial_invocations + 1);
}

TEST_CASE("Multiple observers, single attribute")
{
    CentralDB db;

    CentralDBObserver observer1(Attribute::ID::WHITE_BALANCE, dummy_callback);
    CentralDBObserver observer2(Attribute::ID::WHITE_BALANCE, dummy_callback);
    CentralDBObserver observer3(Attribute::ID::WHITE_BALANCE, dummy_callback);

    db.Attach(&observer1);
    db.Attach(&observer2);
    db.Attach(&observer3);

    const int initial_invocations = num_callback_invocations;
    db.SetUint32(Attribute::ID::WHITE_BALANCE, 123);
    const int final_invocations = num_callback_invocations;

    REQUIRE(final_invocations == initial_invocations + 3);
}

TEST_CASE("Multiple observers, multiple attributes")
{
    CentralDB db;

    CentralDBObserver observer1(Attribute::ID::WHITE_BALANCE, dummy_callback);
    CentralDBObserver observer2(Attribute::ID::FRAME_RATE, dummy_callback);

    db.Attach(&observer1);
    db.Attach(&observer2);

    const int initial_invocations = num_callback_invocations;
    db.SetUint32(Attribute::ID::WHITE_BALANCE, 123);
    db.SetFloat(Attribute::ID::FRAME_RATE, 456.0f);
    const int final_invocations = num_callback_invocations;

    REQUIRE(final_invocations == initial_invocations + 2);
}

TEST_CASE("String attribute")
{
    CentralDB db;

    CentralDBObserver observer(Attribute::ID::TEST_STRING, dummy_callback);

    db.Attach(&observer);

    const int initial_invocations = num_callback_invocations;

    const char saved_value[] = "dummy-string-value";
    db.SetString(Attribute::ID::TEST_STRING, saved_value);
    const char* retrieved_value = db.GetString(Attribute::ID::TEST_STRING);

    const int final_invocations = num_callback_invocations;

    REQUIRE(final_invocations == initial_invocations + 1);
    REQUIRE(std::string(retrieved_value) == std::string(saved_value));
}

TEST_CASE("Boolean attribute")
{
    CentralDB db;

    CentralDBObserver observer(Attribute::ID::TEST_BOOLEAN, dummy_callback);

    db.Attach(&observer);

    const int initial_invocations = num_callback_invocations;

    const bool saved_value = true;
    db.SetBoolean(Attribute::ID::TEST_BOOLEAN, saved_value);
    const bool retrieved_value = db.GetString(Attribute::ID::TEST_STRING);

    const int final_invocations = num_callback_invocations;

    REQUIRE(final_invocations == initial_invocations + 1);
    REQUIRE(retrieved_value == saved_value);
}

TEST_CASE("Int32 attribute")
{
    CentralDB db;

    CentralDBObserver observer(Attribute::ID::TEST_INT32, dummy_callback);

    db.Attach(&observer);

    const int initial_invocations = num_callback_invocations;

    const int32_t saved_value = -987;
    db.SetInt32(Attribute::ID::TEST_INT32, saved_value);
    const int32_t retrieved_value = db.GetInt32(Attribute::ID::TEST_INT32);

    const int final_invocations = num_callback_invocations;

    REQUIRE(final_invocations == initial_invocations + 1);
    REQUIRE(retrieved_value == saved_value);
}

TEST_CASE("Set/Get inapplicable type value")
{
    CentralDB db;

    CentralDBObserver observer(Attribute::ID::TEST_INT32, dummy_callback);

    db.Attach(&observer);

    const int initial_invocations = num_callback_invocations;

    const int32_t saved_valid_type_value = -987;
    db.SetInt32(Attribute::ID::TEST_INT32, saved_valid_type_value);
    db.SetFloat(Attribute::ID::TEST_INT32, saved_valid_type_value); // This shouldn't have any effect
    const int32_t retrieved_valid_type_value = db.GetInt32(Attribute::ID::TEST_INT32);
    const float retrieved_inapplicable_type_value = db.GetFloat(Attribute::ID::TEST_INT32);

    const int final_invocations = num_callback_invocations;

    REQUIRE(final_invocations ==
            initial_invocations + 1); // No invocations of the callback expected for the inapplicable type setter
    REQUIRE(retrieved_valid_type_value == saved_valid_type_value);
    REQUIRE(retrieved_inapplicable_type_value == *((float*)Attribute::DefaultValue(Attribute::Type::FLOAT)));
}
