// Copyright 2024 Stone-Engine

#pragma once

#include <string>

#define STONE_ABSTRACT_NODE(NewClassName) \
    static const std::string className;   \
    virtual const char *getClassName() const override;

#define STONE_NODE(NewClassName) \
    STONE_ABSTRACT_NODE(NewClassName)

/* Cpp Implementations */

#include "Utils/DynamicObjectFactory.hpp"

#define STONE_NODE_BASE_IMPLEMENTATION(NewClassName) \
    const char *NewClassName::getClassName() const   \
    {                                                \
        return NewClassName::className.c_str();      \
    }

#define STONE_NODE_IMPLEMENTATION(NewClassName)                        \
    STONE_NODE_BASE_IMPLEMENTATION(NewClassName)                       \
                                                                       \
    const std::string NewClassName::className = []                     \
    {                                                                  \
        auto constructor = [](const std::string &nodeName)             \
        {                                                              \
            return std::make_shared<NewClassName>(nodeName);           \
        };                                                             \
        DynamicObjectFactory<Node, const std::string &>::getInstance() \
            .add(#NewClassName, constructor);                          \
        return #NewClassName;                                          \
    }();

#define STONE_ABSTRACT_NODE_IMPLEMENTATION(NewClassName) \
    STONE_NODE_BASE_IMPLEMENTATION(NewClassName)         \
                                                         \
    const std::string NewClassName::className = #NewClassName;
