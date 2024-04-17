// Copyright 2024 Stone-Engine

#include "Object.hpp"

namespace STN
{

    Object::Object()
    : std::enable_shared_from_this<Object>()
    {
        static uint32_t id = 0;
        _id = id++;
    }

    Object::Object(const Object &other)
    : std::enable_shared_from_this<Object>()
    {
        _id = other._id;
    }

    Object::~Object()
    {
    }

    uint32_t Object::getId() const
    {
        return _id;
    }

    std::string Object::getClassName() const
    {
        return "Object";
    }

}