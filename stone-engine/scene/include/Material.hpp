// Copyright 2024 Stone-Engine

#pragma once

#include "Object.hpp"

namespace STN
{

    namespace Scene
    {

        class Material : public Object
        {
        public:
            Material();
            Material(const Material &other);
            virtual ~Material();

            float red;
            float green;
            float blue;
            float shininess;
            // [...]
        };

    } // namespace Scene

} // namespace STN
