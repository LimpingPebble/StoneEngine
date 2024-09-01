// Copyright 2024 Stone-Engine

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Stone::Core {

class Object : public std::enable_shared_from_this<Object> {
public:
	Object();
	Object(const Object &other);

	virtual ~Object() = default;

	uint32_t getId() const;

	virtual const char *getClassName() const;

	virtual std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const;

protected:
	uint32_t _id;
};

} // namespace Stone::Core

std::ostream &operator<<(std::ostream &os, const Stone::Core::Object &obj);
