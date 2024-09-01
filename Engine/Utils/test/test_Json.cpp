#include "Utils/Json.hpp"

#include <gtest/gtest.h>

using namespace Stone;

TEST(Json, ParseEmptyObject) {
    std::string jsonString = "{}";

    Json::Parser parser(jsonString);
    auto json = parser.parse();

    ASSERT_EQ(json->type, Json::Value::Type::Object);
    ASSERT_TRUE(std::get<Json::Object>(json->value).empty());
}

TEST(Json, ParseSimpleObject) {
    std::string jsonString = R"({"name": "John", "age": 30, "isStudent": false})";
    Json::Parser parser(jsonString);
    auto json = parser.parse();

    ASSERT_EQ(json->type, Json::Value::Type::Object);

    Json::Object obj = std::get<Json::Object>(json->value);
    ASSERT_EQ(obj["name"]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<std::string>(std::get<Json::Primitive>(obj["name"]->value)), "John");

    ASSERT_EQ(obj["age"]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<double>(std::get<Json::Primitive>(obj["age"]->value)), 30);

    ASSERT_EQ(obj["isStudent"]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<bool>(std::get<Json::Primitive>(obj["isStudent"]->value)), false);
}

TEST(Json, ParseArray) {
    std::string jsonString = R"([1, "two", true, null])";
    Json::Parser parser(jsonString);
    auto json = parser.parse();

    ASSERT_EQ(json->type, Json::Value::Type::Array);

    Json::Array arr = std::get<Json::Array>(json->value);
    ASSERT_EQ(arr.size(), 4);

    ASSERT_EQ(arr[0]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<double>(std::get<Json::Primitive>(arr[0]->value)), 1);

    ASSERT_EQ(arr[1]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<std::string>(std::get<Json::Primitive>(arr[1]->value)), "two");

    ASSERT_EQ(arr[2]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<bool>(std::get<Json::Primitive>(arr[2]->value)), true);

    ASSERT_EQ(arr[3]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<std::nullptr_t>(std::get<Json::Primitive>(arr[3]->value)), nullptr);
}

TEST(Json, ParseNestedObject) {
    std::string jsonString = R"({"person": {"name": "John", "age": 30}})";
    Json::Parser parser(jsonString);
    auto json = parser.parse();

    ASSERT_EQ(json->type, Json::Value::Type::Object);

    Json::Object obj = std::get<Json::Object>(json->value);
    ASSERT_EQ(obj["person"]->type, Json::Value::Type::Object);

    Json::Object personObj = std::get<Json::Object>(obj["person"]->value);
    ASSERT_EQ(personObj["name"]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<std::string>(std::get<Json::Primitive>(personObj["name"]->value)), "John");

    ASSERT_EQ(personObj["age"]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(std::get<double>(std::get<Json::Primitive>(personObj["age"]->value)), 30);
}

TEST(Json, MalformedJsonThrowsException) {
    std::string jsonString = R"({"name": "John")";  // Missing closing brace

    Json::Parser parser(jsonString);
    EXPECT_THROW({
        auto json = parser.parse();
    }, std::runtime_error);
}

TEST(Json, MalformedJsonThrowsException2) {
    std::string jsonString = R"({"name": John})";  // No quotes around string

    Json::Parser parser(jsonString);
    EXPECT_THROW({
        auto json = parser.parse();
    }, std::runtime_error);
}