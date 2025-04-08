// Copyright 2024 Stone-Engine

#include "Network/Dispatcher/JsonRpcDispatcher.hpp"

#include <sstream>

#define MAX_REQUEST_LOOP 1000000000
#define ERROR_HANDLER_THROWN "Unknown error occurred"
#define ERROR_FAILED_TO_SEND "Failed to send request"
#define ERROR_INVALID_JSON "Failed to parse JSON"
#define ERROR_INVALID_MESSAGE "Invalid JSON-RPC message format"

namespace Stone::Network {

bool JsonRpcDispatcher::registerRequestHandler(const Method &method, const SyncRequestHandler &syncHandler) {
	return registerRequestHandler(
		method, [syncHandler](const Params &params, const SuccessCallback &success, const FailureCallback &failure) {
			try {
				success(syncHandler(params));
			} catch (const Error &err) {
				failure(err);
			} catch (const std::exception &e) {
				failure(e.what());
			} catch (...) {
				failure(ERROR_HANDLER_THROWN);
			}
		});
}

bool JsonRpcDispatcher::registerRequestHandler(const Method &method, const AsyncRequestHandler &asyncHandler) {
	return registerRequestHandler(
		method, [asyncHandler](const Params &params, const SuccessCallback &success, const FailureCallback &failure) {
			std::promise<Result> promise;
			std::future<Result> future = promise.get_future();
			asyncHandler(params, promise);
			future.wait();
			try {
				success(future.get());
			} catch (const Error &err) {
				failure(err);
			} catch (const std::exception &e) {
				failure(e.what());
			} catch (...) {
				failure(ERROR_HANDLER_THROWN);
			}
		});
}

bool JsonRpcDispatcher::registerRequestHandler(const Method &method, const RequestHandler &handler) {
	auto it = _requestHandlers.find(method);
	if (it != _requestHandlers.end()) {
		return false; // Handler already registered
	}
	_requestHandlers[method] = handler;
	return true;
}

bool JsonRpcDispatcher::hasRequestHandler(const Method &method) const {
	return _requestHandlers.find(method) != _requestHandlers.end();
}

JsonRpcDispatcher::NotificationSignal &JsonRpcDispatcher::getNotificationSignal(const Method &method) {
	if (auto it = _notificationSignals.find(method); it != _notificationSignals.end()) {
		return *it->second;
	}
	auto it = _notificationSignals.emplace(method, std::make_unique<NotificationSignal>());
	return *it.first->second;
}

bool JsonRpcDispatcher::hasNotificationSignal(const Method &method) const {
	return _notificationSignals.find(method) != _notificationSignals.end();
}

bool JsonRpcDispatcher::sendRequest(std::ostream &output, const Method &method, const Params &params,
									const ResponseCallbacks &callbacks, float timeout) {
	_nextId++;
	if (_nextId >= MAX_REQUEST_LOOP)
		_nextId = 1;
	Json::Value request = Json::object({
		{	 JSONRPC_ID, Json::number(_nextId)},
		{JSONRPC_METHOD,	 Json::string(method)},
	});
	if (!params.isNull())
		request[JSONRPC_PARAMS] = params;
	_pendingRequests[_nextId] = {
		callbacks,
		std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<long long>(timeout * 1000)) //
	};
	output << request;
	if (output.fail() || output.bad()) {
		handleResponseError(_nextId, ERROR_FAILED_TO_SEND);
		return false;
	}
	return true;
}

bool JsonRpcDispatcher::sendNotification(std::ostream &output, const Method &method, const Params &params) {
	output << Json::object({
		{JSONRPC_METHOD, Json::string(method)},
		{JSONRPC_PARAMS,				 params},
	});
	return !(output.fail() || output.bad());
}

bool JsonRpcDispatcher::handleString(const std::string &message, std::ostream &output) {
	std::stringstream stream(message);
	return handleStream(stream, output);
}

bool JsonRpcDispatcher::handleStream(std::istream &stream, std::ostream &output) {
	Json::Value jsonValue;
	try {
		Json::parseStream(stream, jsonValue);
	} catch (const std::exception &e) {
		if (_sendErrorMessage) {
			output << Json::object({
				{ JSONRPC_ERROR,		ERROR_INVALID_JSON},
				{JSONRPC_PARAMS, Json::string(e.what())},
			});
		}
		return false;
	}
	if (jsonValue.is<Json::Object>()) {
		return handleJsonObject(jsonValue.get<Json::Object>(), output);
	} else if (jsonValue.is<Json::Array>()) {
		return handleJsonArray(jsonValue.get<Json::Array>(), output);
	}

	if (_sendErrorMessage) {
		output << Json::object({
			{ JSONRPC_ERROR, ERROR_INVALID_MESSAGE},
			{JSONRPC_PARAMS,			 jsonValue},
		});
	}

	return false;
}

bool JsonRpcDispatcher::handleJsonArray(const Json::Array &message, std::ostream &output) {
	for (const auto &item : message) {
		if (item.is<Json::Object>()) {
			handleJsonObject(item.get<Json::Object>(), output);
		} else {
			if (_sendErrorMessage) {
				output << Json::object({
					{ JSONRPC_ERROR, ERROR_INVALID_MESSAGE},
					{JSONRPC_PARAMS,					 item},
				});
			}
		}
	}
	return true;
}

bool JsonRpcDispatcher::handleJsonObject(const Json::Object &message, std::ostream &output) {
	const auto &idPtr = message.find(JSONRPC_ID);
	const bool hasId = idPtr != message.end() && idPtr->second.is<double>();
	const Id id = hasId ? static_cast<Id>(idPtr->second.get<double>()) : 0;

	const auto &methodPtr = message.find(JSONRPC_METHOD);
	const bool hasMethod = methodPtr != message.end() && methodPtr->second.is<std::string>();

	if (hasMethod) {
		const auto &params = message.find(JSONRPC_PARAMS);

		if (hasId)
			return handleRequest(id, methodPtr->second.get<std::string>(),
								 params == message.end() ? Json::null() : params->second, output);
		else
			return handleNotification(methodPtr->second.get<std::string>(),
									  params == message.end() ? Json::null() : params->second);
	} else {
		if (hasId) {
			const auto &resultPtr = message.find(JSONRPC_RESULT);
			const bool hasResult = resultPtr != message.end() && !resultPtr->second.isNull();

			const auto &errorPtr = message.find(JSONRPC_ERROR);
			const bool hasError = errorPtr != message.end() && errorPtr->second.is<std::string>();

			if (hasResult && !hasError) {
				return handleResponseSuccess(id, resultPtr->second);
			} else if (!hasResult && hasError) {
				return handleResponseError(id, errorPtr->second.get<std::string>());
			}
			if (_sendErrorMessage) {
				output << Json::object({
					{ JSONRPC_ERROR, ERROR_INVALID_MESSAGE},
					{JSONRPC_PARAMS,				 message},
				});
			}
		}
	}
	return false;
}

bool JsonRpcDispatcher::handleRequest(Id id, const Method &method, const Params &params, std::ostream &output) {
	auto it = _requestHandlers.find(method);
	if (it != _requestHandlers.end()) {
		it->second(
			params,
			[&output, id](const Result &result) {
				output << Json::object({
					{	 JSONRPC_ID, Json::number(id)},
					{JSONRPC_RESULT,			 result},
				});
			},
			[&output, id](const Error &error) {
				output << Json::object({
					{	 JSONRPC_ID, Json::number(id)},
					{JSONRPC_ERROR,			error},
				});
			});
		return true;
	}
	return false;
}

bool JsonRpcDispatcher::handleNotification(const Method &method, const Params &params) {
	bool used = false;
	auto it = _notificationSignals.find(method);
	if (it != _notificationSignals.end()) {
		(*it->second)(params);
		used = true;
	}
	auto handlerIt = _requestHandlers.find(method);
	if (handlerIt != _requestHandlers.end()) {
		handlerIt->second(params, [](const Result &) {}, [](const Error &) {});
		used = true;
	}
	return used;
}

bool JsonRpcDispatcher::handleResponseSuccess(Id id, const Result &result) {
	auto it = _pendingRequests.find(id);
	if (it != _pendingRequests.end()) {
		it->second.callbacks.first(result);
		_pendingRequests.erase(it);
		return true;
	}
	return false;
}

bool JsonRpcDispatcher::handleResponseError(Id id, const Error &error) {
	auto it = _pendingRequests.find(id);
	if (it != _pendingRequests.end()) {
		it->second.callbacks.second(error);
		_pendingRequests.erase(it);
		return true;
	}
	return false;
}

void JsonRpcDispatcher::cleanup() {
	cleanupTimedOutPendingRequests();
	cleanupEmptyNotificationSignals();
}

void JsonRpcDispatcher::cleanupTimedOutPendingRequests() {
	for (auto it = _pendingRequests.begin(); it != _pendingRequests.end();) {
		if (it->second.expiration < std::chrono::steady_clock::now()) {
			it->second.callbacks.second("request timed out");
			it = _pendingRequests.erase(it);
		} else {
			++it;
		}
	}
}

void JsonRpcDispatcher::cleanupEmptyNotificationSignals() {
	for (auto it = _notificationSignals.begin(); it != _notificationSignals.end();) {
		if (!it->second->isBound()) {
			it = _notificationSignals.erase(it);
		} else {
			++it;
		}
	}
}


} // namespace Stone::Network
