// Copyright 2024 Stone-Engine

#pragma once

#include <string>

/**
 * @brief Macro to use inside an abstract node class to make the engine reconize this class as a node.
 */
#define STONE_ABSTRACT_NODE(NewClassName)                                                                              \
	static const std::string className;                                                                                \
	virtual const char *getClassName() const override;

/**
 * @brief Macro to use inside a node class to make the engine reconize this class as a node.
 */
#define STONE_NODE(NewClassName) STONE_ABSTRACT_NODE(NewClassName)

/* Cpp Implementations */

#include "Utils/DynamicObjectFactory.hpp"

#define STONE_NODE_BASE_IMPLEMENTATION(NewClassName)                                                                   \
	const char *NewClassName::getClassName() const {                                                                   \
		return NewClassName::className.c_str();                                                                        \
	}

/**
 * @brief Macro to use inside the cpp implementation of a node class that have used the STONE_NODE macro in its class.
 */
#define STONE_NODE_IMPLEMENTATION(NewClassName)                                                                        \
	STONE_NODE_BASE_IMPLEMENTATION(NewClassName)                                                                       \
                                                                                                                       \
	const std::string NewClassName::className = [] {                                                                   \
		auto constructor = [](const std::string &nodeName) {                                                           \
			return std::make_shared<NewClassName>(nodeName);                                                           \
		};                                                                                                             \
		Stone::DynamicObjectFactory<Stone::Scene::Node, const std::string &>::getInstance().add(#NewClassName,         \
																								constructor);          \
		return #NewClassName;                                                                                          \
	}();

/**
 * @brief Macro to use inside the cpp implementation of an abstract node class that have used the STONE_ABSTRACT_NODE
 * macro in its class.
 */
#define STONE_ABSTRACT_NODE_IMPLEMENTATION(NewClassName)                                                               \
	STONE_NODE_BASE_IMPLEMENTATION(NewClassName)                                                                       \
                                                                                                                       \
	const std::string NewClassName::className = #NewClassName;
