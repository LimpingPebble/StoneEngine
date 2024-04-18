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

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const override;

            std::shared_ptr<Object> graphicModuleData;
        };

    } // namespace Scene

} // namespace STN
