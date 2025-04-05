#include "Utils/SigSlot.hpp"

#include <gtest/gtest.h>

using namespace Stone;

static int global_result = 0;
void add(int a, int b) {
	global_result = a + b;
}

TEST(Slot, PerformFunction) {
	Slot<int, int> slot(add);
	slot(5, 8);
	EXPECT_EQ(global_result, 13);
}

TEST(Slot, PerformLambda) {
	int result = 0;
	auto addition = [&result](int a, int b) {
		result = a + b;
	};

	Slot<int, int> slot(addition);
	slot(5, 8);
	EXPECT_EQ(result, 13);
}

TEST(Slot, PerformMethod) {
	class Calc {
	public:
		int internal;

		void docalc(int a, int b) {
			internal = a + b;
		}
	};

	Calc calc;
	calc.internal = 0;

	Slot<int, int> slot(std::bind(&Calc::docalc, &calc, std::placeholders::_1, std::placeholders::_2));
	slot.perform(5, 8);
	EXPECT_EQ(calc.internal, 13);
}

TEST(Slot, PerformMethodConst) {
	struct Calc {
		Calc(int &intern) : internal(intern) {
		}
		int &internal;

		void docalc(int a, int b) const {
			internal = a + b;
		}
	};

	int result = 0;
	const Calc calc(result);

	Slot<int, int> slot(std::bind(&Calc::docalc, &calc, std::placeholders::_1, std::placeholders::_2));
	slot.perform(5, 8);
	EXPECT_EQ(result, 13);
}
