// Copyright 2024 Stone-Engine

#include "Core/Assets/Resource.hpp"

#include "Core/Assets/Bundle.hpp"

namespace Stone::Core::Assets {

Resource::Resource(const std::shared_ptr<Bundle> &bundle, const std::string &filepath)
	: _bundle(bundle), _filename(), _subdirectory() {

	std::size_t slashPos = filepath.find_last_of('/');
	if (slashPos != std::string::npos) {
		_subdirectory = filepath.substr(0, slashPos + 1);
		_filename = filepath.substr(slashPos + 1);
	} else {
		_subdirectory = "./";
		_filename = filepath;
	}
};

std::shared_ptr<Bundle> Resource::getBundle() const {
	return _bundle.lock();
}

const std::string &Resource::getFilename() const {
	return _filename;
}

const std::string &Resource::getSubDirectory() const {
	return _subdirectory;
}

std::string Resource::getFullPath() const {
	return getBundle()->getRootDirectory() + _subdirectory + "/" + _filename;
}

} // namespace Stone::Core::Assets
