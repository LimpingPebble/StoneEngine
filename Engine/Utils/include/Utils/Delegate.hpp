// Copyright 2024 Stone-Engine

#pragma once

#include <functional>
#include <utility>

namespace Stone {

template <typename T>
class IDelegate;

/**
 * @brief Interface specialization for delegates with a return type and arguments.
 * @tparam R The return type.
 * @tparam Args The argument types.
 */
template <typename R, typename... Args>
class IDelegate<R(Args...)> {
public:
	virtual ~IDelegate() = default;

	/**
	 * @brief Performs the delegate operation.
	 * @param args The arguments for the delegate.
	 * @return The result of the delegate operation.
	 */
	virtual R perform(Args...) = 0;

	/**
	 * @brief Performs the delegate operation.
	 */
	inline void operator()(Args... args) {
		perform(std::forward<Args>(args)...);
	}
};

/**
 * @brief Delegate specialization for function pointers with a return type and arguments.
 * @tparam R The return type.
 * @tparam Args The argument types.
 */
template <typename T>
class FunctionDelegate;

template <typename R, typename... Args>
class FunctionDelegate<R(Args...)> : public IDelegate<R(Args...)> {
public:
	using Fn = R (*)(Args...);

	FunctionDelegate(const FunctionDelegate &) = delete;
	FunctionDelegate &operator=(const FunctionDelegate &) = delete;

	/**
	 * @brief Constructs a FunctionDelegate object with the given function pointer.
	 * @param fn The function pointer.
	 */
	explicit FunctionDelegate(const Fn fn) : _fn(fn) {
	}

	/**
	 * @brief Performs the delegate operation by calling the function pointer.
	 * @param args The arguments for the delegate.
	 * @return The result of the delegate operation.
	 */
	R perform(Args... args) override {
		return _fn(std::forward<Args>(args)...);
	}

private:
	const Fn _fn;
};

template <typename T>
class LambdaDelegate;

/**
 * @brief Delegate specialization for lambda functions with a return type and arguments.
 * @tparam R The return type.
 * @tparam Args The argument types.
 */
template <typename R, typename... Args>
class LambdaDelegate<R(Args...)> : public IDelegate<R(Args...)> {
public:
	using Fn = std::function<R(Args...)>;

	LambdaDelegate(const LambdaDelegate &) = delete;
	LambdaDelegate &operator=(const LambdaDelegate &) = delete;

	/**
	 * @brief Constructs a LambdaDelegate object with the given lambda function.
	 * @param fn The lambda function.
	 */
	explicit LambdaDelegate(const Fn fn) : _fn(fn) {
	}

	/**
	 * @brief Performs the delegate operation by calling the lambda function.
	 * @param args The arguments for the delegate.
	 * @return The result of the delegate operation.
	 */
	R perform(Args... args) override {
		return _fn(std::forward<Args>(args)...);
	}

private:
	const Fn _fn;
};

/**
 * @brief Delegate specialization for member function pointers with a return type and arguments.
 * @tparam C The class type.
 * @tparam R The return type.
 * @tparam Args The argument types.
 */
template <typename T>
class MethodDelegate;

template <typename C, typename R, typename... Args>
class MethodDelegate<R (C::*)(Args...)> : public IDelegate<R(Args...)> {
public:
	using Method = R (C::*)(Args...);

	MethodDelegate(const MethodDelegate &) = delete;
	MethodDelegate &operator=(const MethodDelegate &) = delete;

	/**
	 * @brief Constructs a MethodDelegate object with the given target object and member function pointer.
	 * @param target The target object.
	 * @param method The member function pointer.
	 */
	MethodDelegate(C *target, Method method) : _target(target), _method(method) {
	}

	/**
	 * @brief Performs the delegate operation by calling the member function pointer on the target object.
	 * @param args The arguments for the delegate.
	 * @return The result of the delegate operation.
	 */
	R perform(Args... args) override {
		return (_target->*_method)(std::forward<Args>(args)...);
	}

private:
	C *const _target;
	const Method _method;
};

/**
 * @brief Delegate specialization for const member function pointers with a return type and arguments.
 * @tparam C The class type.
 * @tparam R The return type.
 * @tparam Args The argument types.
 */
template <typename T>
class ConstMethodDelegate;

template <typename C, typename R, typename... Args>
class ConstMethodDelegate<R (C::*)(Args...)> : public IDelegate<R(Args...)> {
public:
	using Method = R (C::*)(Args...) const;

	ConstMethodDelegate(const ConstMethodDelegate &) = delete;
	ConstMethodDelegate &operator=(const ConstMethodDelegate &) = delete;

	/**
	 * @brief Constructs a ConstMethodDelegate object with the given target object and const member function pointer.
	 * @param target The target object.
	 * @param method The const member function pointer.
	 */
	ConstMethodDelegate(const C *target, const Method method) : _target(target), _method(method) {
	}

	/**
	 * @brief Performs the delegate operation by calling the const member function pointer on the target object.
	 * @param args The arguments for the delegate.
	 * @return The result of the delegate operation.
	 */
	R perform(Args... args) override {
		return (_target->*_method)(std::forward<Args>(args)...);
	}

private:
	const C *const _target;
	const Method _method;
};

} // namespace Stone
