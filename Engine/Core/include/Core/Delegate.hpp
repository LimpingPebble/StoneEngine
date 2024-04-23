// Copyright 2024 Stone-Engine

#pragma once

#include <utility>

namespace Stone::Core
{

    template <typename T>
    class IDelegate;

    template <typename R, typename... Args>
    class IDelegate<R(Args...)>
    {
    public:
        virtual ~IDelegate() = default;
        virtual R perform(Args...) = 0;
    };

    template <typename T>
    class FunctionDelegate;

    template <typename R, typename... Args>
    class FunctionDelegate<R(Args...)> : public IDelegate<R(Args...)>
    {
        using Fn = R (*)(Args...);

        Fn const _fn;

    public:
        FunctionDelegate(const FunctionDelegate &) = delete;
        FunctionDelegate &operator=(const FunctionDelegate &) = delete;

        FunctionDelegate(const Fn fn) : _fn(fn) {}

        virtual R perform(Args... args) override
        {
            return _fn(std::forward<Args>(args)...);
        }
    };

    template <typename T>
    class LambdaDelegate;

    template <typename R, typename... Args>
    class LambdaDelegate<R(Args...)> : public IDelegate<R(Args...)>
    {
        using Fn = std::function<R(Args...)>;

        Fn const _fn;

    public:
        LambdaDelegate(const LambdaDelegate &) = delete;
        LambdaDelegate &operator=(const LambdaDelegate &) = delete;

        LambdaDelegate(const Fn fn) : _fn(fn) {}

        virtual R perform(Args... args) override
        {
            return _fn(std::forward<Args>(args)...);
        }
    };

    template <typename T>
    class MethodDelegate;

    template <typename C, typename R, typename... Args>
    class MethodDelegate<R (C::*)(Args...)> : public IDelegate<R(Args...)>
    {
        using Method = R (C::*)(Args...);

        C *const _target;
        Method const _method;

    public:
        MethodDelegate(const MethodDelegate &) = delete;
        MethodDelegate &operator=(const MethodDelegate &) = delete;

        MethodDelegate(C *target, Method method)
            : _target(target), _method(method)
        {
        }

        virtual R perform(Args... args) override
        {
            return (_target->*_method)(std::forward<Args>(args)...);
        }
    };

    template <typename T>
    class ConstMethodDelegate;

    template <typename C, typename R, typename... Args>
    class ConstMethodDelegate<R (C::*)(Args...)> : public IDelegate<R(Args...)>
    {
        using Method = R (C::*)(Args...) const;

        const C *const _target;
        Method const _method;

    public:
        ConstMethodDelegate(const ConstMethodDelegate &) = delete;
        ConstMethodDelegate &operator=(const ConstMethodDelegate &) = delete;

        ConstMethodDelegate(const C *target, const Method method)
            : _target(target), _method(method)
        {
        }

        virtual R perform(Args... args) override
        {
            return (_target->*_method)(std::forward<Args>(args)...);
        }
    };

} // namespace Stone::Core
