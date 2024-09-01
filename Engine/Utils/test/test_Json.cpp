#include "Utils/Json.hpp"

#include <gtest/gtest.h>

using namespace Stone;

TEST(Json, ParseEmptyObject) {
	std::string jsonString = "{}";

	auto json = Json::Value::parse(jsonString);

	ASSERT_TRUE(json->is<Json::Object>());
	ASSERT_TRUE(json->get<Json::Object>().empty());
}

TEST(Json, ParseSimpleObject) {
	std::string jsonString = R"({"name": "John", "age": 30, "isStudent": false})";

	auto json = Json::Value::parse(jsonString);

	ASSERT_TRUE(json->is<Json::Object>());

	Json::Object obj = json->get<Json::Object>();
	ASSERT_TRUE(obj["name"]->is<std::string>());
	ASSERT_EQ(obj["name"]->get<std::string>(), "John");

	ASSERT_TRUE(obj["age"]->is<double>());
	ASSERT_EQ(obj["age"]->get<double>(), 30);

	ASSERT_TRUE(obj["isStudent"]->is<bool>());
	ASSERT_EQ(obj["isStudent"]->get<bool>(), false);
}

TEST(Json, ParseArray) {
	std::string jsonString = R"([1, "two", true, null])";

	auto json = Json::Value::parse(jsonString);

	ASSERT_TRUE(json->is<Json::Array>());

	Json::Array arr = json->get<Json::Array>();
	ASSERT_EQ(arr.size(), 4);

	ASSERT_TRUE(arr[0]->is<double>());
	ASSERT_EQ(arr[0]->get<double>(), 1);

	ASSERT_TRUE(arr[1]->is<std::string>());
	ASSERT_EQ(arr[1]->get<std::string>(), "two");

	ASSERT_TRUE(arr[2]->is<bool>());
	ASSERT_EQ(arr[2]->get<bool>(), true);

	ASSERT_TRUE(arr[3]->isNull());
}

TEST(Json, ParseNestedObject) {
	std::string jsonString = R"({"person": {"name": "John", "age": 30}})";

	auto json = Json::Value::parse(jsonString);

	ASSERT_TRUE(json->is<Json::Object>());

	Json::Object obj = json->get<Json::Object>();
	ASSERT_TRUE(obj["person"]->is<Json::Object>());

	Json::Object personObj = obj["person"]->get<Json::Object>();
	ASSERT_TRUE(personObj["name"]->is<std::string>());
	ASSERT_EQ(personObj["name"]->get<std::string>(), "John");

	ASSERT_TRUE(personObj["age"]->is<double>());
	ASSERT_EQ(personObj["age"]->get<double>(), 30);
}

TEST(Json, MalformedJsonThrowsException) {
	std::string jsonString = R"({"name": "John")"; // Missing closing brace

	EXPECT_THROW({ auto json = Json::Value::parse(jsonString); }, std::runtime_error);
}

TEST(Json, MalformedJsonThrowsException2) {
	std::string jsonString = R"({"name": John})"; // No quotes around string

	EXPECT_THROW({ auto json = Json::Value::parse(jsonString); }, std::runtime_error);
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
		obj["name"] = std::make_shared<Json::Value>(std::string("John"));
		obj["age"] = std::make_shared<Json::Value>(30.0);
		obj["isStudent"] = std::make_shared<Json::Value>(false);

		auto value = std::make_shared<Json::Value>(obj);

		result = value->serialize();
	}

	auto json = Json::Value::parse(result);

	ASSERT_TRUE(json->is<Json::Object>());

	auto obj = json->get<Json::Object>();

	ASSERT_TRUE(obj["name"]->is<std::string>());
	ASSERT_EQ(obj["name"]->get<std::string>(), "John");

	ASSERT_TRUE(obj["age"]->is<double>());
	ASSERT_EQ(obj["age"]->get<double>(), 30);

	ASSERT_TRUE(obj["isStudent"]->is<bool>());
	ASSERT_EQ(obj["isStudent"]->get<bool>(), false);
}

TEST(JsonSerializer, SerializeArray) {

	std::string result;

	{
		Json::Array arr = {std::make_shared<Json::Value>(1.0), std::make_shared<Json::Value>(std::string("two")),
						   std::make_shared<Json::Value>(true), std::make_shared<Json::Value>(nullptr)};

		auto value = std::make_shared<Json::Value>(arr);

		result = value->serialize();
	}

	auto json = Json::Value::parse(result);

	ASSERT_TRUE(json->is<Json::Array>());

	auto obj = json->get<Json::Array>();
	ASSERT_EQ(obj.size(), 4);

	ASSERT_TRUE(obj[0]->is<double>());
	ASSERT_EQ(obj[0]->get<double>(), 1);

	ASSERT_TRUE(obj[1]->is<std::string>());
	ASSERT_EQ(obj[1]->get<std::string>(), "two");

	ASSERT_TRUE(obj[2]->is<bool>());
	ASSERT_EQ(obj[2]->get<bool>(), true);

	ASSERT_TRUE(obj[3]->isNull());
}

TEST(JsonSerializer, SerializeNestedObject) {

	std::string result;

	{
		Json::Object personObj;
		personObj["name"] = std::make_shared<Json::Value>(std::string("John"));
		personObj["age"] = std::make_shared<Json::Value>(30.0);

		Json::Object obj;
		obj["person"] = std::make_shared<Json::Value>(personObj);

		auto value = std::make_shared<Json::Value>(obj);

		result = value->serialize();
	}

	auto json = Json::Value::parse(result);

	ASSERT_TRUE(json->is<Json::Object>());

	auto obj = json->get<Json::Object>();

	ASSERT_TRUE(obj["person"]->is<Json::Object>());

	auto personObj = obj["person"]->get<Json::Object>();

	ASSERT_TRUE(personObj["name"]->is<std::string>());
	ASSERT_EQ(personObj["name"]->get<std::string>(), "John");

	ASSERT_TRUE(personObj["age"]->is<double>());
	ASSERT_EQ(personObj["age"]->get<double>(), 30);
}

TEST(JsonSerializer, SerializeComplexObject) {

	std::string result;
	{
		Json::Object addressObj;
		addressObj["city"] = std::make_shared<Json::Value>(std::string("New York"));
		addressObj["zip"] = std::make_shared<Json::Value>(std::string("10001"));

		Json::Object obj;
		obj["name"] = std::make_shared<Json::Value>(std::string("John"));
		obj["age"] = std::make_shared<Json::Value>(30.0);
		obj["isStudent"] = std::make_shared<Json::Value>(false);
		obj["scores"] = std::make_shared<Json::Value>(Json::Array{
			std::make_shared<Json::Value>(85.5),
            std::make_shared<Json::Value>(92.0),
			std::make_shared<Json::Value>(78.5)
        });
		obj["address"] = std::make_shared<Json::Value>(addressObj);

		auto value = std::make_shared<Json::Value>(obj);

		result = value->serialize();
	}

	auto json = Json::Value::parse(result);

	ASSERT_TRUE(json->is<Json::Object>());

	auto obj = json->get<Json::Object>();
	ASSERT_TRUE(obj["name"]->is<std::string>());
	ASSERT_EQ(obj["name"]->get<std::string>(), "John");

	ASSERT_TRUE(obj["age"]->is<double>());
	ASSERT_EQ(obj["age"]->get<double>(), 30);

	ASSERT_TRUE(obj["isStudent"]->is<bool>());
	ASSERT_EQ(obj["isStudent"]->get<bool>(), false);

	ASSERT_TRUE(obj["scores"]->is<Json::Array>());
	auto scores = obj["scores"]->get<Json::Array>();
	ASSERT_EQ(scores.size(), 3);
	ASSERT_EQ(scores[0]->get<double>(), 85.5);
	ASSERT_EQ(scores[1]->get<double>(), 92);
	ASSERT_EQ(scores[2]->get<double>(), 78.5);

	ASSERT_TRUE(obj["address"]->is<Json::Object>());
	auto address = obj["address"]->get<Json::Object>();
	ASSERT_EQ(address["city"]->get<std::string>(), "New York");
	ASSERT_EQ(address["zip"]->get<std::string>(), "10001");
}
