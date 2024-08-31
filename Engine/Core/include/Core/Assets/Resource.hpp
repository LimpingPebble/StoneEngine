// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"

namespace Stone::Core::Assets {

class Bundle;

/**
 * @brief Represents a resource.
 */
class Resource : public Object {

public:

    Resource(const std::shared_ptr<Bundle>& bundle, const std::string &filepath);

    virtual void unloadData() = 0;
    virtual void loadData(bool force = false) = 0;

    std::shared_ptr<Bundle> getBundle() const;

    const std::string &getFilename() const;

    const std::string &getSubDirectory() const;

    std::string getFullPath() const;

protected:

    /**
     * @brief The workspace that contains the resource
     */
    std::weak_ptr<Bundle> _bundle;

	/**
	 * @brief The filename of the asset without it's path
	 */
	std::string _filename;

	/**
	 * @brief The directory path of the asset
	 */
	std::string _subdirectory;

    friend class Bundle;

};

} // namespace Stone::Core
