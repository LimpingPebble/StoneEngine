// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Delegate.hpp"

namespace Stone::Core
{

    template <typename T>
    class Publisher;

    template <typename T>
    class Subscriber;

    /**
     * @brief A class template representing a subscriber in an event system.
     *
     * This class template is used to create subscribers that can be bound to publishers in an event system.
     * Subscribers can be bound to functions, lambda expressions, or member functions of a class.
     *
     * @tparam R The return type of the subscriber's callback function.
     * @tparam Args The argument types of the subscriber's callback function.
     */
    template <typename R, typename... Args>
    class Subscriber<R(Args...)>
    {
    public:
        Subscriber(const Subscriber &) = delete;
        Subscriber &operator=(const Subscriber &) = delete;

        /**
         * @brief Constructs a subscriber with a function pointer as the callback.
         *
         * @param fn A function pointer to the callback function.
         */
        Subscriber(R (*fn)(Args... args)) : _publisher(nullptr)
        {
            new (_delegateptr) FunctionDelegate<R(Args...)>(fn);
        }

        /**
         * @brief Constructs a subscriber with a lambda expression as the callback.
         *
         * @param fn A lambda expression representing the callback function.
         */
        Subscriber(std::function<R(Args...)> fn) : _publisher(nullptr)
        {
            new (_delegateptr) LambdaDelegate<R(Args...)>(fn);
        }

        /**
         * @brief Constructs a subscriber with a member function pointer as the callback.
         *
         * @tparam C The class type that the member function belongs to.
         * @param target A pointer to an instance of the class.
         * @param method A pointer to the member function.
         */
        template <typename C>
        Subscriber(C *target, R (C::*method)(Args... args)) : _publisher(nullptr)
        {
            new (_delegateptr) MethodDelegate<R (C::*)(Args...)>(target, method);
        }

        /**
         * @brief Constructs a subscriber with a const member function pointer as the callback.
         *
         * @tparam C The class type that the member function belongs to.
         * @param target A pointer to a const instance of the class.
         * @param method A pointer to the const member function.
         */
        template <typename C>
        Subscriber(const C *target, R (C::*method)(Args... args) const) : _publisher(nullptr)
        {
            new (_delegateptr) ConstMethodDelegate<R (C::*)(Args...)>(target, method);
        }

        /**
         * @brief Destructor.
         *
         * Unbinds the subscriber from the publisher.
         */
        virtual ~Subscriber()
        {
            unbind();
        }

        /**
         * @brief Calls the callback function of the subscriber.
         *
         * @param args The arguments to be passed to the callback function.
         * @return The return value of the callback function.
         */
        R perform(Args... args)
        {
            return reinterpret_cast<IDelegate<R(Args...)> *>(_delegateptr)->perform(std::forward<Args>(args)...);
        }

        /**
         * @brief Unbinds the subscriber from the publisher.
         *
         * If the subscriber is bound to a publisher, it will be unbound.
         */
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
