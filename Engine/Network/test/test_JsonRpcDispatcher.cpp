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

TEST(JsonRpcDispatcher, SendRequestWithParams) {
	JsonRpcDispatcher dispatcher;
	std::stringstream out;
	Json::Value outJson;

	dispatcher.sendRequest(out, "setValue", Json::number(12),
						   {[](const Json::Value &result) { (void)result; },
							[](const std::string &error) {
								(void)error;
							}});

	ASSERT_NO_THROW(out >> outJson);
	ASSERT_TRUE(outJson.is<Json::Object>());
	ASSERT_NE(outJson.get<Json::Object>().find("id"), outJson.get<Json::Object>().end());
	ASSERT_NE(outJson.get<Json::Object>().find("method"), outJson.get<Json::Object>().end());
	ASSERT_NE(outJson.get<Json::Object>().find("params"), outJson.get<Json::Object>().end());

	EXPECT_EQ(outJson.get<Json::Object>()["method"], Json::string("setValue"));
	EXPECT_EQ(outJson.get<Json::Object>()["params"], Json::number(12));

	int firstId = outJson.get<Json::Object>()["id"].get<double>();

	dispatcher.sendRequest(out, "getValue", Json::null(),
						   {[](const Json::Value &result) { (void)result; },
							[](const std::string &error) {
								(void)error;
							}});

	ASSERT_NO_THROW(out >> outJson);
	ASSERT_TRUE(outJson.is<Json::Object>());
	ASSERT_NE(outJson.get<Json::Object>().find("id"), outJson.get<Json::Object>().end());
	ASSERT_NE(outJson.get<Json::Object>().find("method"), outJson.get<Json::Object>().end());

	EXPECT_EQ(outJson.get<Json::Object>()["method"], Json::string("getValue"));

	int secondId = outJson.get<Json::Object>()["id"].get<double>();
	EXPECT_NE(firstId, secondId);
}

TEST(JsonRpcDispatcher, SendRequestWithParamsAndReceiveResponse) {
	JsonRpcDispatcher dispatcher;
	std::stringstream out;
	Json::Value outJson;

	int zeValue = 0;
	std::string receivedError = "";

	{
		EXPECT_TRUE(dispatcher.sendRequest( //
			out, "getValue", Json::null(),
			{[&zeValue](const Json::Value &result) {
				 if (result.is<double>()) {
					 zeValue = result.get<double>();
				 }
			 },
			 [&receivedError](const std::string &error) {
				 receivedError = error;
			 }}));

		ASSERT_NO_THROW(out >> outJson);
		ASSERT_TRUE(outJson.is<Json::Object>());
		ASSERT_NE(outJson.get<Json::Object>().find("id"), outJson.get<Json::Object>().end());
		ASSERT_NE(outJson.get<Json::Object>().find("method"), outJson.get<Json::Object>().end());

		ASSERT_TRUE(outJson.get<Json::Object>()["id"].is<double>());
		EXPECT_EQ(outJson.get<Json::Object>()["method"], Json::string("getValue"));

		int requestId = outJson.get<Json::Object>()["id"].get<double>();

		EXPECT_EQ(zeValue, 0);

		auto response = Json::Object({
			{	 "id", Json::number(requestId)},
			{"result",		   Json::number(12)},
		});
		EXPECT_TRUE(dispatcher.handleJsonObject(response, out));

		EXPECT_EQ(zeValue, 12);
		EXPECT_EQ(receivedError, "");

		zeValue = 0;

		auto secondResponse = Json::Object({
			{	 "id", Json::number(requestId)},
			{"result",		   Json::number(17)},
		});

		EXPECT_FALSE(dispatcher.handleJsonObject(secondResponse, out));

		EXPECT_NE(zeValue, 17);
	}

	{
		zeValue = 10;

		EXPECT_TRUE(dispatcher.sendRequest( //
			out, "getValue", Json::null(),
			{[&zeValue](const Json::Value &result) {
				 if (result.is<double>()) {
					 zeValue = result.get<double>();
				 }
			 },
			 [&zeValue, &receivedError](const std::string &error) {
				 //
				 zeValue = 0;
				 receivedError = error;
			 }}));

		ASSERT_NO_THROW(out >> outJson);
		ASSERT_TRUE(outJson.is<Json::Object>());
		ASSERT_NE(outJson.get<Json::Object>().find("id"), outJson.get<Json::Object>().end());
		ASSERT_NE(outJson.get<Json::Object>().find("method"), outJson.get<Json::Object>().end());

		ASSERT_TRUE(outJson.get<Json::Object>()["id"].is<double>());
		EXPECT_EQ(outJson.get<Json::Object>()["method"], Json::string("getValue"));

		int requestId = outJson.get<Json::Object>()["id"].get<double>();

		auto response = Json::Object({
			{	 "id",			   Json::number(requestId)},
			{"error", Json::string("pas envie cette fois")},
		});
		EXPECT_TRUE(dispatcher.handleJsonObject(response, out));

		EXPECT_EQ(zeValue, 0);
		EXPECT_EQ(receivedError, "pas envie cette fois");
	}
}

TEST(JsonRpcDispatcher, HandleRequestWithTimeout) {
	JsonRpcDispatcher dispatcher;
	std::stringstream out;

	bool errorReceived = false;

	dispatcher.sendRequest( //
		out, "getValue", Json::null(),
		{[](const Json::Value &result) { (void)result; },
		 [&errorReceived](const std::string &error) {
			 (void)error;
			 errorReceived = true;
		 }},
		0.005f);
	// Timeout after 5ms

	dispatcher.cleanupTimedOutPendingRequests();
	EXPECT_FALSE(errorReceived);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	dispatcher.cleanupTimedOutPendingRequests();
	EXPECT_FALSE(errorReceived);

	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	dispatcher.cleanupTimedOutPendingRequests();
	EXPECT_TRUE(errorReceived);
}
