// Copyright 2024 Stone-Engine

#pragma once

#include "core/Object.hpp"

namespace Stone
{

    namespace Scene
    {

        class Material : public Core::Object
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

} // namespace Stone
