// Copyright 2024 Stone-Engine

#pragma once

#include "Utils/Json.hpp"
#include "Utils/SigSlot.hpp"

#include <future>

#define JSONRPC_ID "id"
#define JSONRPC_METHOD "method"
#define JSONRPC_PARAMS "params"
#define JSONRPC_RESULT "result"
#define JSONRPC_ERROR "error"

namespace Stone::Network {

class JsonRpcDispatcher {
public:
	using Id = int;
	using Method = std::string;
	using Params = Json::Value;
	using Result = Json::Value;
	using Error = std::string;

	using SuccessCallback = std::function<void(const Result &)>;
	using FailureCallback = std::function<void(const Error &)>;

	using SyncRequestHandler = std::function<Result(const Params &)>;
	using AsyncRequestHandler = std::function<void(const Params &, const std::promise<Result> &)>;
	using RequestHandler = std::function<void(const Params &, const SuccessCallback &, const FailureCallback &)>;

	using NotificationSignal = Signal<const Params &>;

	using ResponseCallbacks = std::pair<SuccessCallback, FailureCallback>;

	JsonRpcDispatcher() = default;
	JsonRpcDispatcher(const JsonRpcDispatcher &other) = default;

	virtual ~JsonRpcDispatcher() = default;

	bool registerRequestHandler(const Method &method, const SyncRequestHandler &syncHandler);
	bool registerRequestHandler(const Method &method, const AsyncRequestHandler &asyncHandler);
	bool registerRequestHandler(const Method &method, const RequestHandler &handler);

	bool hasRequestHandler(const Method &method) const;

	NotificationSignal &getNotificationSignal(const Method &method);

	bool hasNotificationSignal(const Method &method) const;

	bool sendRequest(std::ostream &output, const Method &method, const Params &params,
					 const ResponseCallbacks &callbacks, float timeout = 10.0f);

	bool sendNotification(std::ostream &output, const Method &method, const Params &params);

	bool handleString(const std::string &message, std::ostream &output);
	bool handleStream(std::istream &stream, std::ostream &output);
	bool handleJsonArray(const Json::Array &message, std::ostream &output);
	bool handleJsonObject(const Json::Object &message, std::ostream &output);

	bool handleRequest(Id id, const Method &method, const Params &params, std::ostream &output);
	bool handleNotification(const Method &method, const Params &params);
	bool handleResponseSuccess(Id id, const Result &result);
	bool handleResponseError(Id id, const Error &error);

	void cleanup();
	void cleanupTimedOutPendingRequests();
	void cleanupEmptyNotificationSignals();

private:
	std::unordered_map<Method, RequestHandler> _requestHandlers;
	std::unordered_map<Method, std::unique_ptr<NotificationSignal>> _notificationSignals;

	struct PendingResponse {
		ResponseCallbacks callbacks;
		std::chrono::steady_clock::time_point expiration;
	};
	std::unordered_map<Id, PendingResponse> _pendingRequests;
	Id _nextId = 0;
};

} // namespace Stone::Network
