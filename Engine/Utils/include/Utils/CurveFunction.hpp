// Copyright 2024 Stone-Engine

#pragma once

#include <cmath>
#include <functional>

/**
 * @brief Collection of curve functions for easing animations.
 * The functions provide various easing effects such as linear, sine, quadratic, cubic, quartic, quintic, exponential,
 * circular, back, elastic, bounce, and teleport. These functions can be used to create smooth and natural animations
 * with different easing behaviors.
 *
 * @tparam T The type of the input and output values for the curve functions.
 */
namespace Stone::Curve {

template <typename T = float>
T easeLinear(T x) {
	return x;
}

template <typename T = float>
T easeInSine(T x) {
	return 1 - cos((x * M_PI) / 2);
}

template <typename T = float>
T easeInQuad(T x) {
	return x * x;
}

template <typename T = float>
T easeInCubic(T x) {
	return x * x * x;
}

template <typename T = float>
T easeInQuart(T x) {
	return x * x * x * x;
}

template <typename T = float>
T easeInQuint(T x) {
	return x * x * x * x * x;
}

template <typename T = float>
T easeInExpo(T x) {
	return x == 0 ? 0 : pow(2, 10 * x - 10);
}

template <typename T = float>
T easeInCirc(T x) {
	return 1 - sqrt(1 - pow(x, 2));
}

template <typename T = float>
T easeInBack(T x) {
	T c1 = 1.70158;
	T c3 = c1 + 1;

	return c3 * x * x * x - c1 * x * x;
}

template <typename T = float>
T easeInElastic(T x) {
	T c4 = (2 * M_PI) / 3;

	return x == 0 ? 0 : x == 1 ? 1 : -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
}

template <typename T = float>
T easeInBounce(T x) {
	return 1 - easeOutBounce(1 - x);
}

template <typename T = float>
T easeInTeleport(T x) {
	return x < 1.0 ? 0.0 : 1.0;
}

template <typename T = float>
T easeOutSine(T x) {
	return sin((x * M_PI) / 2);
}

template <typename T = float>
T easeOutQuad(T x) {
	return 1 - (1 - x) * (1 - x);
}
template <typename T = float>
T easeOutCubic(T x) {
	return 1 - pow(1 - x, 3);
}

template <typename T = float>
T easeOutQuart(T x) {
	return 1 - pow(1 - x, 4);
}

template <typename T = float>
T easeOutQuint(T x) {
	return 1 - pow(1 - x, 5);
}

template <typename T = float>
T easeOutExpo(T x) {
	return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

template <typename T = float>
T easeOutCirc(T x) {
	return sqrt(1 - pow(x - 1, 2));
}

template <typename T = float>
T easeOutBack(T x) {
	T c1 = 1.70158;
	T c3 = c1 + 1;

	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

template <typename T = float>
T easeOutElastic(T x) {
	T c4 = (2 * M_PI) / 3;

	return x == 0 ? 0 : x == 1 ? 1 : pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}

template <typename T = float>
T easeOutBounce(T x) {
	const T n1 = 7.5625;
	const T d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	} else if (x < 2 / d1) {
		const T xm = x - (1.5 / d1);
		return n1 * xm * xm + 0.75;
	} else if (x < 2.5 / d1) {
		const T xm = x - (2.25 / d1);
		return n1 * xm * xm + 0.9375;
	} else {
		const T xm = x - (2.625 / d1);
		return n1 * xm * xm + 0.984375;
	}
}

template <typename T = float>
T easeOutTeleport(T x) {
	return x < 0.0 ? 0.0 : 1.0;
}

template <typename T = float>
T easeInOutSine(T x) {
	return -(cos(M_PI * x) - 1) / 2;
}

template <typename T = float>
T easeInOutQuad(T x) {
	return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
}

template <typename T = float>
T easeInOutCubic(T x) {
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

template <typename T = float>
T easeInOutQuart(T x) {
	return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}

template <typename T = float>
T easeInOutQuint(T x) {
	return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
}

template <typename T = float>
T easeInOutExpo(T x) {
	return x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ? pow(2, 20 * x - 10) / 2 : (2 - pow(2, -20 * x + 10)) / 2;
}

template <typename T = float>
T easeInOutCirc(T x) {
	return x < 0.5 ? (1 - sqrt(1 - pow(2 * x, 2))) / 2 : (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
}

template <typename T = float>
T easeInOutBack(T x) {
	T c1 = 1.70158;
	T c2 = c1 * 1.525;

	return x < 0.5 ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
				   : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

template <typename T = float>
T easeInOutElastic(T x) {
	T c5 = (2 * M_PI) / 4.5;

	return x == 0	 ? 0
		   : x == 1	 ? 1
		   : x < 0.5 ? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
					 : (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
}

template <typename T = float>
T easeInOutBounce(T x) {
	return x < 0.5 ? (1 - easeOutBounce(1 - 2 * x)) / 2 : (1 + easeOutBounce(2 * x - 1)) / 2;
}

template <typename T = float>
T easeInOutTeleport(T x) {
	return x < 0.5 ? 0.0 : 1.0;
}

/**
 * @brief A template class representing a mathematical function.
 *
 * This class provides a way to define and use mathematical functions.
 * It supports various types of functions, including custom functions,
 * linear functions, easing functions, elastic functions, and bounce functions.
 *
 * @tparam T The type of the function's input and output.
 */
template <typename T = float>
class Function {
public:
	Function() = default;
	Function(const Function &copy) = default;
	Function(T (*f)(T)) : _func(f) {
	}

	virtual ~Function() = default;

	Function &operator=(const Function &copy) = default;

	/**
	 * @brief Function call operator.
	 *
	 * @param t The input value for the function.
	 * @return T The output value of the function.
	 */
	T operator()(T t) const {
		return _func(t);
	}

	/**
	 * @brief Creates a custom function.
	 *
	 * @param f A function pointer representing the custom function.
	 * @return Function A Function object representing the custom function.
	 */
	static Function custom(T (*f)(T)) {
		return Function(f);
	}

	/**
	 * @brief Creates a linear function.
	 * The linear function is a straight line with a slope of 1.
	 *
	 * @return Function A Function object representing the linear function.
	 */
	static Function linear() {
		return Function(easeLinear);
	}

	/**
	 * @brief Creates an ease-in function.
	 * The ease-in function starts slow and accelerates towards the end.
	 *
	 * @return Function A Function object representing the ease-in function.
	 */
	static Function easeIn() {
		return Function(easeInQuad);
	}

	/**
	 * @brief Creates an ease-out function.
	 * The ease-out function starts fast and decelerates towards the end.
	 *
	 * @return Function A Function object representing the ease-out function.
	 */
	static Function easeOut() {
		return Function(easeOutQuad);
	}

	/**
	 * @brief Creates an ease-in-out function.
	 * The ease-in-out function starts slow, accelerates in the middle, and decelerates towards the end.
	 *
	 * @return Function A Function object representing the ease-in-out function.
	 */
	static Function easeInOut() {
		return Function(easeInOutQuad);
	}

	/**
	 * @brief Creates an elastic function.
	 * The elastic function simulates an elastic band that stretches and snaps back into place.
	 * It has a bouncy effect that overshoots the target value before settling.
	 *
	 * @return Function A Function object representing the elastic function.
	 */
	static Function elastic() {
		return Function(easeOutElastic);
	}

	/**
	 * @brief Creates a bounce function.
	 * The bounce function simulates a bouncing ball that bounces up and down before coming to rest.
	 * It has a bouncy effect that overshoots the target value before settling.
	 *
	 * @return Function A Function object representing the bounce function.
	 */
	static Function bounce() {
		return Function(easeOutBounce);
	}

private:
	T (*_func)(T) = &easeLinear<T>; /**< A function pointer representing the function. */
};

} // namespace Stone::Curve
