#include <gtest/gtest.h>
#include "Utils/Delegate.hpp"

using namespace Stone::Core;

std::string add_two(float a, float b, int& result)
{
    result = a + b;
    return std::to_string(result);
}

TEST(FunctionDelegate, Perform)
{
    FunctionDelegate<std::string(float, float, int&)> functionDelegate(&add_two);

    int result = 0;
    std::string str = functionDelegate.perform(6.5f, 2.6f, result);
    EXPECT_EQ(result, 9);
    EXPECT_EQ(str, "9");
}

TEST(LambdaDelegate, Perform)
{
    auto mult_two_lambda = [](int a, float b, int& result)
    {
        result = a * b;
        return std::to_string(result);
    };

    LambdaDelegate<std::string(int, float, int&)> lambdaDelegate(mult_two_lambda);

    int result = 0;
    std::string str = lambdaDelegate.perform(8, 2.5f, result);
    EXPECT_EQ(result, 20);
    EXPECT_EQ(str, "20");
}

TEST(MethodDelegate, Perform)
{
    class Calc
    {
    public:
        int mult;

        std::string dothing(float a, int b, int& result)
        {
            result = (a + b) * mult;
            return std::to_string(result);
        }
    };

    Calc calc;
    calc.mult = 3;

    int result = 0;
    MethodDelegate<std::string (Calc::*)(float, int, int&)> methodDelegate(&calc, &Calc::dothing);
    std::string str = methodDelegate.perform(1.7f, 3, result);
    EXPECT_EQ(result, 14);
    EXPECT_EQ(str, "14");
}

TEST(MethodDelegate, PerformConst)
{
    class Calc
    {
    public:
        int mult;

        std::string dothing(float a, int b, int& result) const
        {
            result = (a + b) * mult;
            return std::to_string(result);
        }
    };

    Calc calc;
    calc.mult = 3;

    int result = 0;
    ConstMethodDelegate<std::string (Calc::*)(float, int, int&)> methodDelegate(&calc, &Calc::dothing);
    std::string str = methodDelegate.perform(1.7f, 3, result);
    EXPECT_EQ(result, 14);
    EXPECT_EQ(str, "14");
}

// Run all the tests
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}