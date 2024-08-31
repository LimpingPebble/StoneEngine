// Copyright 2024 Stone-Engine

#include "Core/Assets/Bundle.hpp"

namespace Stone::Core::Assets {

Bundle::Bundle(std::string rootDirectory) : _rootDirectory(std::move(rootDirectory)) {
    if (_rootDirectory.empty()) {
        _rootDirectory = "./";
    } else if (_rootDirectory.back() != '/') {
        _rootDirectory += '/';
    }
}

const char *Bundle::getClassName() const {
    return "Bundle";
}

std::ostream &Bundle::writeToStream(std::ostream &stream, bool closing_bracer) const {
    Object::writeToStream(stream, false);
    stream << ",root_directory:\"" << _rootDirectory << "\"";
    if (closing_bracer) {
        stream << "}";
    }
    return stream;
}

std::shared_ptr<Resource> Bundle::getResource(const std::string &filepath) const {
	auto it = _resources.find(filepath);
	if (it != _resources.end()) {
		return it->second;
	}
	return nullptr;
}

const std::string &Bundle::getRootDirectory() const {
	return _rootDirectory;
}


} // namespace Stone::Core::Assets
