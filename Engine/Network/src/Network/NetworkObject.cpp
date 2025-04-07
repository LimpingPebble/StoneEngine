// Copyright 2024 Stone-Engine

#include "Network/NetworkObject.hpp"

namespace Stone::Network {


void NetworkObject::writeToJson(Json::Object &json) const {
	Core::Object::writeToJson(json);

	json["poolId"] = Json::number(_poolId);
}

ObjectPool<NetworkObject>::Id NetworkObject::getPoolId() const {
	return _poolId;
}

void NetworkObject::receiveData(const std::uint8_t *data, std::size_t size) {
	onReceivingData.broadcast(*this, data, size);
}

void NetworkObject::sendData(const std::uint8_t *data, std::size_t &size) const {
	onSendingData.broadcast(*this, data, size);
}


} // namespace Stone::Network
