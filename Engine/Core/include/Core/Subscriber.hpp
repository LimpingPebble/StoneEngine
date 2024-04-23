// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Delegate.hpp"

namespace Stone::Core
{

    template <typename T>
    class Publisher;

    template <typename T>
    class Subscriber;

    template <typename R, typename... Args>
    class Subscriber<R(Args...)>
    {
    public:
        Subscriber(const Subscriber &) = delete;
        Subscriber &operator=(const Subscriber &) = delete;

        Subscriber(R (*fn)(Args... args)) : _publisher(nullptr)
        {
            new (_delegateptr) FunctionDelegate<R(Args...)>(fn);
        }

        Subscriber(std::function<R(Args...)> fn) : _publisher(nullptr)
        {
            new (_delegateptr) LambdaDelegate<R(Args...)>(fn);
        }

        template <typename C>
        Subscriber(C *target, R (C::*method)(Args... args)) : _publisher(nullptr)
        {
            new (_delegateptr) MethodDelegate<R (C::*)(Args...)>(target, method);
        }

        template <typename C>
        Subscriber(const C *target, R (C::*method)(Args... args) const) : _publisher(nullptr)
        {
            new (_delegateptr) ConstMethodDelegate<R (C::*)(Args...)>(target, method);
        }

        virtual ~Subscriber()
        {
            unbind();
        }

        R perform(Args... args)
        {
            return reinterpret_cast<IDelegate<R(Args...)> *>(_delegateptr)->perform(std::forward<Args>(args)...);
        }

        void unbind()
        {
            if (_publisher != nullptr)
            {
                _publisher->unbind(*this);
                _publisher = nullptr;
            }
        }

    private:
        uint8_t _delegateptr[sizeof(LambdaDelegate<void()>)];

        Publisher<R(Args...)> *_publisher;
        friend Publisher<R(Args...)>;
    };

} // namespace Stone::Core

#include "Core/Publisher.hpp"
