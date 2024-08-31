// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Core/Assets/Resource.hpp"

#include <unordered_map>

namespace Stone::Core::Assets {

class Bundle : public Object {

public:

	Bundle(const Bundle &other) = delete;

	explicit Bundle(std::string rootDirectory = "./");

	~Bundle() override = default;

    const char *getClassName() const override;

    std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

    template<typename ResourceType, typename... Args>
	std::shared_ptr<ResourceType> loadResource(const std::string &filepath, Args... args)
    {
        auto it = _resources.find(filepath);
        if (it != _resources.end()) {
            return std::dynamic_pointer_cast<ResourceType>(it->second);
        }
        auto thisBundle = std::dynamic_pointer_cast<Bundle>(shared_from_this());
        auto resource = std::make_shared<ResourceType>(thisBundle, filepath, args...);
        _resources[filepath] = resource;
        return resource;
    }

	std::shared_ptr<Resource> getResource(const std::string &filepath) const;

    const std::string& getRootDirectory() const;

protected:
	/**
	 * @brief The root directory of the bundle
	 */
	std::string _rootDirectory;

	/**
	 * @brief The map of resources indexed by their filename shortned path
	 */
	std::unordered_map<std::string, std::shared_ptr<Resource>> _resources;
};

} // namespace Stone::Core::Assets
