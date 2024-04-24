#include <gtest/gtest.h>
#include "Utils/SigSlot.hpp"

using namespace Stone;

int add(int a, int b)
{
    return a + b;
}

TEST(Slot, PerformFunction)
{
    Slot<int(int, int)> slot(add);
    int result = slot.perform(5, 8);
    EXPECT_EQ(result, 13);
}

TEST(Slot, PerformLambda)
{
    int result = 0;
    auto addition = [&result](int a, int b)
    {
        result = a + b;
        return result;
    };

    Slot<int(int, int)> slot(addition);
    int res = slot.perform(5, 8);
    EXPECT_EQ(res, 13);
    EXPECT_EQ(result, 13);
}

TEST(Slot, PerformMethod)
{
    class Calc
    {
    public:
        int internal;

        int docalc(int a, int b)
        {
            return internal + a + b;
        }
    };

    Calc calc;
    calc.internal = 10;

    Slot<int(int, int)> slot(&calc, &Calc::docalc);
    int result = slot.perform(5, 8);
    EXPECT_EQ(result, 23);
}

TEST(Slot, PerformMethodConst)
{
    struct Calc
    {
        Calc(int intern) : internal(intern) {}
        int internal;

        int docalc(int a, int b) const
        {
            return internal + a + b;
        }
    };

    const Calc calc(10);

    Slot<int(int, int)> slot(&calc, &Calc::docalc);
    int result = slot.perform(5, 8);
    EXPECT_EQ(result, 23);
}
