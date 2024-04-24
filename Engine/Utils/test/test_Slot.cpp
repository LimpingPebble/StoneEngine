#include <gtest/gtest.h>
#include "Utils/PubSub.hpp"

using namespace Stone::Core;

int add(int a, int b)
{
    return a + b;
}

TEST(Subscriber, PerformFunction)
{
    Subscriber<int(int, int)> subscriber(add);
    int result = subscriber.perform(5, 8);
    EXPECT_EQ(result, 13);
}

TEST(Subscriber, PerformLambda)
{
    int result = 0;
    auto addition = [&result](int a, int b)
    {
        result = a + b;
        return result;
    };

    Subscriber<int(int, int)> subscriber(addition);
    int res = subscriber.perform(5, 8);
    EXPECT_EQ(res, 13);
    EXPECT_EQ(result, 13);
}

TEST(Subscriber, PerformMethod)
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

    Subscriber<int(int, int)> subscriber(&calc, &Calc::docalc);
    int result = subscriber.perform(5, 8);
    EXPECT_EQ(result, 23);
}

TEST(Subscriber, PerformMethodConst)
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

    Subscriber<int(int, int)> subscriber(&calc, &Calc::docalc);
    int result = subscriber.perform(5, 8);
    EXPECT_EQ(result, 23);
}

// Run all the tests
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}