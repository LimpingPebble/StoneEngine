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
	using Error = std::exception;

	using SuccessCallback = std::function<void(const Params &)>;
	using FailureCallback = std::function<void(const Error &)>;

	using SyncRequestHandler = std::function<Result(const Params &)>;
	using AsyncRequestHandler = std::function<void(const Params &, const std::promise<Result> &)>;
	using RequestHandler = std::function<void(const Params &, const SuccessCallback &, const FailureCallback &)>;

	using NotificationSignal = Signal<const Params &>;

	JsonRpcDispatcher() = default;
	JsonRpcDispatcher(const JsonRpcDispatcher &other) = default;

	virtual ~JsonRpcDispatcher() = default;

	bool registerRequestHandler(const Method &method, const SyncRequestHandler &syncHandler);
	bool registerRequestHandler(const Method &method, const AsyncRequestHandler &asyncHandler);
	bool registerRequestHandler(const Method &method, const RequestHandler &handler);

	bool hasRequestHandler(const Method &method) const;

	NotificationSignal &getNotificationSignal(const Method &method);

	bool handleString(const std::string &message, std::ostream &output);
	bool handleStream(std::istream &stream, std::ostream &ouutput);
	bool handleJsonArray(const Json::Array &message, std::ostream &output);
	bool handleJsonObject(const Json::Object &message, std::ostream &output);

	bool handleRequest(Id id, const Method &method, const Params &params, std::ostream &output);
	bool handleNotification(const Method &method, const Params &params);

private:
	std::unordered_map<Method, RequestHandler> _requestHandlers;
	std::unordered_map<Method, std::unique_ptr<NotificationSignal>> _notificationSignals;
};

} // namespace Stone::Network
