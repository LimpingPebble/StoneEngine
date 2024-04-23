#include <gtest/gtest.h>
#include "Core/Delegate.hpp"

using namespace Stone::Core;

void add_two(int a, int b, int& result)
{
    result = a + b;
}

TEST(FunctionDelegate, Perform)
{
    int result = 0;
    FunctionDelegate<int, int, int&> functionDelegate(&add_two);
    functionDelegate.perform(1, 2, result);
    EXPECT_EQ(result, 3);
}

TEST(MethodDelegate, Perform)
{
    class Adder
    {
    public:
        int mult;

        void add(int a, int b, int& result)
        {
            result = (a + b) * mult;
        }
    };

    Adder adder;
    adder.mult = 3;

    int result = 0;
    MethodDelegate<void (Adder::*)(int, int, int&)> methodDelegate(&adder, &Adder::add);
    methodDelegate.perform(1, 2, result);
    EXPECT_EQ(result, 9);
}

TEST(MethodDelegate, PerformConst)
{
    class ConstAdder
    {
    public:
        int mult;

        void add(int a, int b, int& result) const
        {
            result = (a + b) * mult;
        }
    };

    ConstAdder adder;
    adder.mult = 4;

    int result = 0;
    ConstMethodDelegate<void (ConstAdder::*)(int, int, int&)> methodDelegate(&adder, &ConstAdder::add);
    methodDelegate.perform(1, 2, result);
    EXPECT_EQ(result, 12);
}

// Run all the tests
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}