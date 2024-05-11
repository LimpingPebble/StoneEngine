#include "Utils/CurveFunction.hpp"

#include <gtest/gtest.h>

using namespace Stone::Curve;

TEST(CurveFunction, Linear) {
	Function<float> linearFunc = Function<float>::linear();

	EXPECT_EQ(linearFunc(0.0f), 0.0f);
	EXPECT_EQ(linearFunc(0.5f), 0.5f);
	EXPECT_EQ(linearFunc(1.0f), 1.0f);
}

TEST(CurveFunction, EaseIn) {
	Function<float> easeInFunc = Function<float>::easeIn();

	EXPECT_EQ(easeInFunc(0.0f), 0.0f);
	EXPECT_LT(easeInFunc(0.25f), 0.25f);
	EXPECT_NEAR(easeInFunc(0.5f), 0.25f, 0.001f);
	EXPECT_LT(easeInFunc(0.75f), 0.75f);
	EXPECT_EQ(easeInFunc(1.0f), 1.0f);
}

TEST(CurveFunction, EaseOut) {
	Function<float> easeOutFunc = Function<float>::easeOut();

	EXPECT_EQ(easeOutFunc(0.0f), 0.0f);
	EXPECT_GT(easeOutFunc(0.25f), 0.25f);
	EXPECT_NEAR(easeOutFunc(0.5f), 0.75f, 0.001f);
	EXPECT_GT(easeOutFunc(0.75f), 0.75f);
	EXPECT_EQ(easeOutFunc(1.0f), 1.0f);
}

TEST(CurveFunction, EaseInOut) {
	Function<float> easeInOutFunc = Function<float>::easeInOut();

	EXPECT_EQ(easeInOutFunc(0.0f), 0.0f);
	EXPECT_NEAR(easeInOutFunc(0.25f), 0.125f, 0.001f);
	EXPECT_NEAR(easeInOutFunc(0.5f), 0.5f, 0.001f);
	EXPECT_NEAR(easeInOutFunc(0.75f), 0.875f, 0.001f);
	EXPECT_EQ(easeInOutFunc(1.0f), 1.0f);
}
