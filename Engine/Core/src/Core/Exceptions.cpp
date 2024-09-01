// Copyright 2024 Stone-Engine

#include "Core/Exceptions.hpp"

namespace Stone::Core {

FileLoadingError::FileLoadingError(std::string filepath, std::string reason)
	: filepath(std::move(filepath)), reason(std::move(reason)) {
	_whatMsg = "FileLoadingError{file:\"" + this->filepath + "\",reason:\"" + this->reason + "\"}";
}

const char *FileLoadingError::what() const noexcept {
	return _whatMsg.c_str();
}

} // namespace Stone::Core
