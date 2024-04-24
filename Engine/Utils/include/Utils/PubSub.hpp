// Copyright 2024 Stone-Engine

#pragma once

#include "Utils/Delegate.hpp"
#include <set>

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

    template <typename T>
    class Subscriber;

    /**
     * @brief The Publisher class is responsible for managing subscribers and broadcasting events.
     *
     * @tparam R The return type of the event.
     * @tparam Args The argument types of the event.
     */
    template <typename R, typename... Args>
    class Publisher<R(Args...)>
    {
    public:
        Publisher(const Publisher &) = delete;
        Publisher &operator=(const Publisher &) = delete;

        /**
         * @brief Default constructor to create a Publisher object.
         */
        Publisher() : _subscribers()
        {
        }

        /**
         * @brief Destructor to clean up the Publisher object.
         *        It removes the Publisher reference from all subscribers.
         */
        virtual ~Publisher()
        {
            for (auto &subscriber : _subscribers)
            {
                if (subscriber != nullptr && subscriber->_publisher == this)
                    subscriber->_publisher = nullptr;
            }
        }

        /**
         * @brief Broadcasts an event to all subscribers.
         *
         * @param args The arguments to be passed to the subscribers.
         */
        void broadcast(Args... args) const
        {
            for (auto &subscriber : _subscribers)
            {
                if (subscriber != nullptr)
                    subscriber->perform(std::forward<Args>(args)...);
            }
        }

        /**
         * @brief Binds a subscriber to the Publisher.
         *        It unbinds the subscriber from any previous Publisher.
         *
         * @param sub The subscriber to be bound.
         */
        void bind(Subscriber<R(Args...)> &sub)
        {
            sub.unbind();
            sub._publisher = this;
            _subscribers.insert(&sub);
        }

        /**
         * @brief Unbinds a subscriber from this Publisher.
         *
         * @param sub The subscriber to be unbound.
         */
        void unbind(Subscriber<R(Args...)> &sub)
        {
            if (sub._publisher == this)
            {
                sub._publisher = nullptr;
                _subscribers.erase(&sub);
            }
        }

    private:
        std::set<Subscriber<R(Args...)> *> _subscribers;
    };

} // namespace Stone::Core
