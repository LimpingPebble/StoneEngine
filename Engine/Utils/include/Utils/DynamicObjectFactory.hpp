// Copyright 2024 Stone-Engine

#pragma once

#include <memory>
#include <map>
#include <functional>

template <class T, class... Args>
class DynamicObjectFactory {
public:
    using Constructor = std::function<std::shared_ptr<T>(Args...)>;

    DynamicObjectFactory() = default;
    static DynamicObjectFactory& getInstance() {
        static DynamicObjectFactory instance;
        return instance;
    }

    std::shared_ptr<T> createObject(const std::string& type, Args... arg) {
        auto it = _constructors.find(type);
        if (it != _constructors.end()) {
            return it->second(arg...);
        }
        return nullptr;
    }

    void registerConstructor(const std::string& type, Constructor constructor) {
        _constructors[type] = constructor;
    }

protected:

    std::map<std::string, Constructor> _constructors;
};
