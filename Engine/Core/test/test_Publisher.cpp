#include <gtest/gtest.h>
#include "Core/Publisher.hpp"

using namespace Stone::Core;

void add(int a, int b, int &result)
{
    result = a + b;
}

TEST(Publisher, SubscribeWithFunction)
{
    int result = 0;

    Publisher<void(int, int, int &)> publisher;
    publisher.broadcast(5, 5, result);
    EXPECT_EQ(result, 0);
    {
        Subscriber<void(int, int, int &)> subscriber(add);
        publisher.bind(subscriber);
        publisher.broadcast(5, 8, result);
        EXPECT_EQ(result, 13);
    }
    publisher.broadcast(12, 5, result);
    EXPECT_EQ(result, 13);
}

TEST(Publisher, SubscribeWithLambda)
{
    int result = 0;
    auto addition = [&result](int a, int b)
    {
        result = a + b;
    };

    Publisher<void(int, int)> publisher;
    publisher.broadcast(5, 5);
    EXPECT_EQ(result, 0);
    {
        Subscriber<void(int, int)> subscriber(addition);
        publisher.bind(subscriber);
        publisher.broadcast(5, 8);
        EXPECT_EQ(result, 13);
    }
    publisher.broadcast(12, 5);
    EXPECT_EQ(result, 13);
}

TEST(Publisher, SubscribeWithMethod)
{
    struct Calc
    {
        int result = 0;
        void add_numbers(int a, int b) { result = a + b; }
    };

    Calc calc;
    EXPECT_EQ(calc.result, 0);

    Publisher<void(int, int)> publisher;
    {
        Subscriber<void(int, int)> subscriber(&calc, &Calc::add_numbers);
        publisher.bind(subscriber);
        publisher.broadcast(5, 8);
        EXPECT_EQ(calc.result, 13);
    }
    publisher.broadcast(12, 5);
    EXPECT_EQ(calc.result, 13);
}

TEST(Publisher, SubscribeWithMethodConst)
{
    struct Calc
    {
        Calc(int m) : modifier(m) {}
        int modifier;

        void add_numbers(int a, int b, int &result) const
        {
            result = modifier * (a + b);
        }
    };

    const Calc calc(3);
    int result = 0;

    Publisher<void(int, int, int &)> publisher;

    {
        Subscriber<void(int, int, int &)> subscriber(&calc, &Calc::add_numbers);

        publisher.bind(subscriber);

        publisher.broadcast(2, 3, result);
        EXPECT_EQ(result, 15);
    }

    publisher.broadcast(12, 5, result);

    EXPECT_EQ(result, 15);
}

// Run all the tests
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}