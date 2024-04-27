// Copyright 2024 Stone-Engine

#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace Stone {

namespace Core {

class Object : public std::enable_shared_from_this<Object> {
public:
	Object();
	Object(const Object &other);

	virtual ~Object();

	uint32_t getId() const;

	virtual const char *getClassName() const;

	virtual std::ostream &writeToStream(std::ostream &stream, bool closing_bracer = true) const;

protected:
	uint32_t _id;
};

} // namespace Core

} // namespace Stone

std::ostream &operator<<(std::ostream &os, const Stone::Core::Object &obj);
