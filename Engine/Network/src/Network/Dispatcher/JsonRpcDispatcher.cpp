// Copyright 2024 Stone-Engine

#include "Network/Dispatcher/JsonRpcDispatcher.hpp"

#include <sstream>

#define MAX_REQUEST_LOOP 1000000000

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
				failure("Unknown error occurred");
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
				failure("Unknown error occurred");
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

bool JsonRpcDispatcher::sendRequest(std::ostream &output, const Method &method, const Params &params,
									const ResponseCallbacks &callbacks) {
	_nextId++;
	if (_nextId >= MAX_REQUEST_LOOP)
		_nextId = 1;
	Json::Value request = Json::object();
	Json::Object &requestObject(request.get<Json::Object>());
	requestObject[JSONRPC_ID] = Json::number(_nextId);
	requestObject[JSONRPC_METHOD] = Json::string(method);
	if (!params.isNull())
		requestObject[JSONRPC_PARAMS] = params;
	_pendingRequests[_nextId] = callbacks;
	output << request;
	if (output.fail() || output.bad()) {
		_pendingRequests.erase(_nextId);
		return false;
	}
	return true;
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
		output << "Failed to parse JSON: " << e.what() << std::endl;
		return false;
	}
	if (jsonValue.is<Json::Object>()) {
		handleJsonObject(jsonValue.get<Json::Object>(), output);
		return true;
	} else if (jsonValue.is<Json::Array>()) {
		handleJsonArray(jsonValue.get<Json::Array>(), output);
		return true;
	}
	output << "Invalid JSON-RPC message format" << std::endl;
	return false;
}

bool JsonRpcDispatcher::handleJsonArray(const Json::Array &message, std::ostream &output) {
	for (const auto &item : message) {
		if (item.is<Json::Object>()) {
			handleJsonObject(item.get<Json::Object>(), output);
		} else {
			output << "Invalid JSON-RPC message format" << std::endl;
			return false;
		}
	}
	return true;
}

bool JsonRpcDispatcher::handleJsonObject(const Json::Object &message, std::ostream &output) {
	const auto &idPtr = message.find(JSONRPC_ID);
	const bool hasId = idPtr != message.end() && idPtr->second.is<double>();

	const auto &methodPtr = message.find(JSONRPC_METHOD);
	const bool hasMethod = methodPtr != message.end() && methodPtr->second.is<std::string>();

	if (hasMethod) {
		const auto &params = message.find(JSONRPC_PARAMS);

		if (hasId)
			return handleRequest(idPtr->second.get<double>(), methodPtr->second.get<std::string>(),
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
				return handleResponseSuccess(idPtr->second.get<double>(), resultPtr->second);
			} else if (!hasResult && hasError) {
				return handleResponseError(idPtr->second.get<double>(), errorPtr->second.get<std::string>());
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
				Json::Value response = Json::object();
				Json::Object &responseObject(response.get<Json::Object>());
				responseObject[JSONRPC_ID] = Json::number(id);
				responseObject[JSONRPC_RESULT] = result;
				output << response;
			},
			[&output, id](const Error &error) {
				Json::Value response = Json::object();
				Json::Object &responseObject(response.get<Json::Object>());
				responseObject[JSONRPC_ID] = Json::number(id);
				responseObject[JSONRPC_ERROR] = Json::string(error);
				output << response;
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
		it->second.first(result);
		_pendingRequests.erase(it);
		return true;
	}
	return false;
}

bool JsonRpcDispatcher::handleResponseError(Id id, const Error &error) {
	auto it = _pendingRequests.find(id);
	if (it != _pendingRequests.end()) {
		it->second.second(error);
		_pendingRequests.erase(it);
		return true;
	}
	return false;
}

} // namespace Stone::Network
