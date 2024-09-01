#include "Utils/Json.hpp"

#include <gtest/gtest.h>

using namespace Stone;

TEST(Json, ParseEmptyObject) {
	std::string jsonString = "{}";

	auto json = Json::Value::parse(jsonString);

	ASSERT_EQ(json->type, Json::Value::Type::Object);
	ASSERT_TRUE(json->asObject().empty());
}

TEST(Json, ParseSimpleObject) {
	std::string jsonString = R"({"name": "John", "age": 30, "isStudent": false})";

	auto json = Json::Value::parse(jsonString);

	ASSERT_EQ(json->type, Json::Value::Type::Object);

	Json::Object obj = json->asObject();
	ASSERT_EQ(obj["name"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj["name"]->asString(), "John");

	ASSERT_EQ(obj["age"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj["age"]->asNumber(), 30);

	ASSERT_EQ(obj["isStudent"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj["isStudent"]->asBool(), false);
}

TEST(Json, ParseArray) {
	std::string jsonString = R"([1, "two", true, null])";

	auto json = Json::Value::parse(jsonString);

	ASSERT_EQ(json->type, Json::Value::Type::Array);

	Json::Array arr = std::get<Json::Array>(json->value);
	ASSERT_EQ(arr.size(), 4);

	ASSERT_EQ(arr[0]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(arr[0]->asNumber(), 1);

	ASSERT_EQ(arr[1]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(arr[1]->asString(), "two");

	ASSERT_EQ(arr[2]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(arr[2]->asBool(), true);

	ASSERT_EQ(arr[3]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(arr[3]->asNull(), nullptr);
}

TEST(Json, ParseNestedObject) {
	std::string jsonString = R"({"person": {"name": "John", "age": 30}})";

	auto json = Json::Value::parse(jsonString);

	ASSERT_EQ(json->type, Json::Value::Type::Object);

	Json::Object obj = std::get<Json::Object>(json->value);
	ASSERT_EQ(obj["person"]->type, Json::Value::Type::Object);

	Json::Object personObj = std::get<Json::Object>(obj["person"]->value);
	ASSERT_EQ(personObj["name"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(personObj["name"]->asString(), "John");

	ASSERT_EQ(personObj["age"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(personObj["age"]->asNumber(), 30);
}

TEST(Json, MalformedJsonThrowsException) {
	std::string jsonString = R"({"name": "John")"; // Missing closing brace

	Json::Parser parser(jsonString);
	EXPECT_THROW({ auto json = parser.parse(); }, std::runtime_error);
}

TEST(Json, MalformedJsonThrowsException2) {
	std::string jsonString = R"({"name": John})"; // No quotes around string

	Json::Parser parser(jsonString);
	EXPECT_THROW({ auto json = parser.parse(); }, std::runtime_error);
}

/*
 * Serializer tests
 */

TEST(JsonSerializer, SerializeEmptyObject) {

	Json::Object obj;
	auto value = std::make_shared<Json::Value>(obj);

	std::string result = value->serialize();
	ASSERT_EQ(result, "{}");
}

TEST(JsonSerializer, SerializeSimpleObject) {

	std::string result;

	{
		Json::Object obj;
		obj["name"] = std::make_shared<Json::Value>(Json::Primitive("John"));
		obj["age"] = std::make_shared<Json::Value>(Json::Primitive(30.0));
		obj["isStudent"] = std::make_shared<Json::Value>(Json::Primitive(false));

		auto value = std::make_shared<Json::Value>(obj);

		result = value->serialize();
	}

	auto json = Json::Value::parse(result);

	ASSERT_EQ(json->type, Json::Value::Type::Object);

	Json::Object obj = std::get<Json::Object>(json->value);
	ASSERT_EQ(obj["name"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj["name"]->asString(), "John");

	ASSERT_EQ(obj["age"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj["age"]->asNumber(), 30);

	ASSERT_EQ(obj["isStudent"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj["isStudent"]->asBool(), false);
}

TEST(JsonSerializer, SerializeArray) {

	std::string result;

	{
		Json::Array arr = {std::make_shared<Json::Value>(Json::Primitive(1.0)),
						   std::make_shared<Json::Value>(Json::Primitive("two")),
						   std::make_shared<Json::Value>(Json::Primitive(true)),
						   std::make_shared<Json::Value>(Json::Primitive(nullptr))};

		auto value = std::make_shared<Json::Value>(arr);

		result = value->serialize();
	}

	auto json = Json::Value::parse(result);

	ASSERT_EQ(json->type, Json::Value::Type::Array);

	Json::Array obj = std::get<Json::Array>(json->value);
	ASSERT_EQ(obj.size(), 4);

	ASSERT_EQ(obj[0]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj[0]->asNumber(), 1);

	ASSERT_EQ(obj[1]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj[1]->asString(), "two");

	ASSERT_EQ(obj[2]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj[2]->asBool(), true);

	ASSERT_EQ(obj[3]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(obj[3]->asNull(), nullptr);
}

TEST(JsonSerializer, SerializeNestedObject) {

	std::string result;

	{
		Json::Object personObj;
		personObj["name"] = std::make_shared<Json::Value>(Json::Primitive("John"));
		personObj["age"] = std::make_shared<Json::Value>(Json::Primitive(30.0));

		Json::Object obj;
		obj["person"] = std::make_shared<Json::Value>(personObj);

		auto value = std::make_shared<Json::Value>(obj);

		result = value->serialize();
	}

	auto json = Json::Value::parse(result);

	ASSERT_EQ(json->type, Json::Value::Type::Object);

	auto obj = json->asObject();

    ASSERT_EQ(obj["person"]->type, Json::Value::Type::Object);

    auto personObj = obj["person"]->asObject();

	ASSERT_EQ(personObj["name"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(personObj["name"]->asString(), "John");

	ASSERT_EQ(personObj["age"]->type, Json::Value::Type::Primitive);
	ASSERT_EQ(personObj["age"]->asNumber(), 30);
}

TEST(JsonSerializer, SerializeComplexObject) {

	std::string result;
	{
		Json::Object addressObj;
		addressObj["city"] = std::make_shared<Json::Value>(Json::Primitive("New York"));
		addressObj["zip"] = std::make_shared<Json::Value>(Json::Primitive("10001"));

		Json::Object obj;
		obj["name"] = std::make_shared<Json::Value>(Json::Primitive("John"));
		obj["age"] = std::make_shared<Json::Value>(Json::Primitive(30.0));
		obj["isStudent"] = std::make_shared<Json::Value>(Json::Primitive(false));
		obj["scores"] = std::make_shared<Json::Value>(Json::Array{
			std::make_shared<Json::Value>(Json::Primitive(85.5)), std::make_shared<Json::Value>(Json::Primitive(92.0)),
			std::make_shared<Json::Value>(Json::Primitive(78.5))});
		obj["address"] = std::make_shared<Json::Value>(addressObj);

		auto value = std::make_shared<Json::Value>(obj);

		result = value->serialize();
	}

    auto json = Json::Value::parse(result);

    ASSERT_EQ(json->type, Json::Value::Type::Object);

    auto obj = json->asObject();
    ASSERT_EQ(obj["name"]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(obj["name"]->asString(), "John");

    ASSERT_EQ(obj["age"]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(obj["age"]->asNumber(), 30);

    ASSERT_EQ(obj["isStudent"]->type, Json::Value::Type::Primitive);
    ASSERT_EQ(obj["isStudent"]->asBool(), false);

    ASSERT_EQ(obj["scores"]->type, Json::Value::Type::Array);
    auto scores = obj["scores"]->asArray();
    ASSERT_EQ(scores.size(), 3);
    ASSERT_EQ(scores[0]->asNumber(), 85.5);
    ASSERT_EQ(scores[1]->asNumber(), 92);
    ASSERT_EQ(scores[2]->asNumber(), 78.5);

    ASSERT_EQ(obj["address"]->type, Json::Value::Type::Object);
    auto address = obj["address"]->asObject();
    ASSERT_EQ(address["city"]->asString(), "New York");
    ASSERT_EQ(address["zip"]->asString(), "10001");

}
