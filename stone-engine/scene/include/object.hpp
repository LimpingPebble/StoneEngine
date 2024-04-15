// Copyright 2024 Stone-Engine

#pragma once

#include <memory>

namespace STN
{

    class Object : public std::enable_shared_from_this<Object>
    {
    public:
        Object();
        Object(const Object &other);

        virtual ~Object();

        uint32_t getId() const;

    protected:
        uint32_t _id;
    };

} // namespace STN
