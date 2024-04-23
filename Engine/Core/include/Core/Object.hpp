// Copyright 2024 Stone-Engine

#pragma once

#include <memory>
#include <iostream>
#include <string>

namespace Stone
{

    namespace Core
    {

        class Object : public std::enable_shared_from_this<Object>
        {
        public:
            Object();
            Object(const Object &other);

            virtual ~Object();

            uint32_t getId() const;

            virtual const char *getClassName() const;

            virtual std::string debugDescription() const;

        protected:
            uint32_t _id;
        };

    } // namespace Core

} // namespace Stone