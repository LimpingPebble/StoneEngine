// Copyright 2024 Stone-Engine

#pragma once

#include <utility>

namespace Stone::Core
{

    template <typename... Args>
    class IDelegate
    {
    public:
        virtual ~IDelegate() = default;
        virtual void perform(Args...) = 0;
    };

    template <typename... Args>
    class FunctionDelegate : public IDelegate<Args...>
    {
        using Fn = void (*)(Args...);

        Fn const fn;

    public:
        FunctionDelegate(const FunctionDelegate &) = delete;
        FunctionDelegate &operator=(const FunctionDelegate &) = delete;

        FunctionDelegate(const Fn fn) : fn(fn) {}

        virtual void perform(Args... args) override
        {
            fn(std::forward<Args>(args)...);
        }
    };

    template <typename T>
    class MethodDelegate;

    template <typename C, typename... Args>
    class MethodDelegate<void (C::*)(Args...)> : public IDelegate<Args...>
    {
        using Method = void (C::*)(Args...);

        C *const object;
        Method const method;

    public:
        MethodDelegate(const MethodDelegate &) = delete;
        MethodDelegate &operator=(const MethodDelegate &) = delete;

        MethodDelegate(C *object, Method method)
            : object(object), method(method)
        {
        }

        virtual void perform(Args... args) override
        {
            return (object->*method)(std::forward<Args>(args)...);
        }
    };

    template <typename T>
    class ConstMethodDelegate;

    template <typename C, typename... Args>
    class ConstMethodDelegate<void (C::*)(Args...)> : public IDelegate<Args...>
    {
        using Method = void (C::*)(Args...) const;

        const C *const object;
        Method const method;

    public:
        ConstMethodDelegate(const ConstMethodDelegate &) = delete;
        ConstMethodDelegate &operator=(const ConstMethodDelegate &) = delete;

        ConstMethodDelegate(const C *object, const Method method)
            : object(object), method(method)
        {
        }

        virtual void perform(Args... args) override
        {
            return (object->*method)(std::forward<Args>(args)...);
        }
    };

} // namespace Stone::Core
