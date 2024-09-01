// Copyright 2024 Stone-Engine

#pragma once

#include <exception>
#include <string>

namespace Stone::Core {

class FileLoadingError : public std::exception {
public:
	const std::string filepath;
	const std::string reason;

	FileLoadingError(std::string filepath, std::string reason);
	const char *what() const throw();

private:
	std::string _whatMsg;
};

} // namespace Stone::Core
