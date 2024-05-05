// Copyright 2024 Stone-Engine

#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

namespace Stone {

/**
 * @brief A factory class for creating dynamic objects of given type.
 *
 * This class provides a mechanism for creating string typed objects at runtime.
 * It uses a map to store constructors for different types of objects.
 * The factory can be accessed through the getInstance() method, which returns a reference to the singleton instance of
 * the factory. Objects can be created by calling the create() method and passing the type and constructor arguments.
 * New constructors can be added to the factory using the add() method.
 *
 * @tparam T The type of objects to be created.
 * @tparam Args The types of constructor arguments.
 */
template <class T, class... Args>
class DynamicObjectFactory {
public:
	using Constructor = std::function<std::shared_ptr<T>(Args...)>;

	DynamicObjectFactory() = default;

	/**
	 * @brief Get the singleton instance of the factory.
	 *
	 * @return DynamicObjectFactory& The reference to the singleton instance.
	 */
	static DynamicObjectFactory &getInstance() {
		static DynamicObjectFactory instance;
		return instance;
	}

	/**
	 * @brief Create an object of type T.
	 *
	 * @param type The type of the object to create.
	 * @param arg The constructor arguments.
	 * @return std::shared_ptr<T> The created object, or nullptr if the type is not registered.
	 */
	std::shared_ptr<T> create(const std::string &type, Args... arg) {
		auto it = _constructors.find(type);
		if (it != _constructors.end()) {
			return it->second(arg...);
		}
		return nullptr;
	}

	/**
	 * @brief Add a constructor for a specific type.
	 *
	 * @param type The type of the object.
	 * @param constructor The constructor function.
	 */
	void add(const std::string &type, Constructor constructor) {
		_constructors[type] = constructor;
	}

protected:
	std::unordered_map<std::string, Constructor> _constructors;
};

} // namespace Stone
