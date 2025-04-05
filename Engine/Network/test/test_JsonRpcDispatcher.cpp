#include "Network/Dispatcher/JsonRpcDispatcher.hpp"

#include <gtest/gtest.h>

using namespace Stone::Network;

TEST(JsonRpcDispatcher, HandleRequestThatIgnoreParams) {
	JsonRpcDispatcher dispatcher;
	std::stringstream out;
	Json::Value outJson;

	int zeValue = 0;

	dispatcher.registerRequestHandler("incValue", [&zeValue](const Json::Value &params) {
		(void)params;
		zeValue++;
		return Json::null();
	});

	EXPECT_EQ(zeValue, 0);

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue"})", out));
	EXPECT_EQ(zeValue, 1);
	EXPECT_STREQ(out.str().c_str(), "");

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue", "params": null})", out));
	EXPECT_EQ(zeValue, 2);
	EXPECT_STREQ(out.str().c_str(), "");

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue", "params": {}})", out));
	EXPECT_EQ(zeValue, 3);
	EXPECT_STREQ(out.str().c_str(), "");

	EXPECT_TRUE(dispatcher.handleString(R"({"id": 0, "method": "incValue", "params": {}})", out));
	EXPECT_EQ(zeValue, 4);
	EXPECT_STRNE(out.str().c_str(), "");

	out = std::stringstream();
	EXPECT_STREQ(out.str().c_str(), "");

	EXPECT_TRUE(dispatcher.handleString(R"({"id": 1, "method": "incValue", "params": {}})", out));
	EXPECT_EQ(zeValue, 5);
	EXPECT_STRNE(out.str().c_str(), "");
}

TEST(JsonRpcDispatcher, HandleRequestUsingNumberParam) {
	JsonRpcDispatcher dispatcher;
	std::stringstream out;
	Json::Value outJson;

	int zeValue = 0;

	dispatcher.registerRequestHandler("incValue", [&zeValue](const Json::Value &params) {
		if (!params.is<double>())
			throw std::runtime_error("invalid params type");
		zeValue += params.get<double>();
		return Json::number(zeValue);
	});

	EXPECT_EQ(zeValue, 0);

	EXPECT_TRUE(dispatcher.handleString(R"({"id": 0, "method": "incValue"})", out));
	ASSERT_EQ(zeValue, 0);
	EXPECT_STRNE(out.str().c_str(), "");
	ASSERT_NO_THROW(out >> outJson);
	ASSERT_TRUE(outJson.is<Json::Object>());
	EXPECT_STREQ(outJson.get<Json::Object>()["error"].get<std::string>().c_str(), "invalid params type");
	out = std::stringstream();
	EXPECT_STREQ(out.str().c_str(), "");

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue", "params": 12})", out));
	EXPECT_EQ(zeValue, 12);
	EXPECT_STREQ(out.str().c_str(), "");

	EXPECT_TRUE(dispatcher.handleString(R"({"id": 1, "method": "incValue", "params": 6})", out));
	EXPECT_EQ(zeValue, 18);
	EXPECT_STRNE(out.str().c_str(), "");
	ASSERT_NO_THROW(out >> outJson);
	ASSERT_TRUE(outJson.is<Json::Object>());
	EXPECT_EQ(outJson.get<Json::Object>()["id"].get<double>(), 1);
	EXPECT_EQ(outJson.get<Json::Object>()["result"].get<double>(), 18);
	EXPECT_EQ(outJson.get<Json::Object>().find("error"), outJson.get<Json::Object>().end());
}


TEST(JsonRpcDispatcher, HandleNotificationThatIgnoreParams) {
	JsonRpcDispatcher dispatcher;
	std::stringstream out;

	int zeValue = 0;
	auto incValue = [&zeValue](const Json::Value &params) {
		(void)params;
		zeValue++;
	};
	Stone::Slot<const Json::Value &> incSlot(incValue);

	dispatcher.getNotificationSignal("incValue").bind(incSlot);

	EXPECT_EQ(zeValue, 0);

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue"})", out));
	EXPECT_EQ(zeValue, 1);

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue", "params": null})", out));
	EXPECT_EQ(zeValue, 2);

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue", "params": {}})", out));
	EXPECT_EQ(zeValue, 3);
}

TEST(JsonRpcDispatcher, HandleNotificationUsingParams) {
	JsonRpcDispatcher dispatcher;
	std::stringstream out;

	int zeValue = 0;
	auto incValue = [&zeValue](const Json::Value &params) {
		if (!params.is<double>())
			return;
		zeValue += params.get<double>();
	};
	Stone::Slot<const Json::Value &> incSlot(incValue);

	dispatcher.getNotificationSignal("incValue").bind(incSlot);

	EXPECT_EQ(zeValue, 0);

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue"})", out));
	EXPECT_EQ(zeValue, 0);

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue", "params": 2})", out));
	EXPECT_EQ(zeValue, 2);

	EXPECT_TRUE(dispatcher.handleString(R"({"method": "incValue", "params": [2]})", out));
	EXPECT_EQ(zeValue, 2);
}
