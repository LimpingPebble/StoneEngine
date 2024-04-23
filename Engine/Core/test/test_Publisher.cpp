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

TEST(Publisher, Unbind)
{
    int result = 0;
    auto addition = [&result](int a, int b)
    {
        result = a + b;
    };

    Publisher<void(int, int)> publisher;
    {
        Subscriber<void(int, int)> subscriber(addition);
        publisher.bind(subscriber);
        publisher.broadcast(5, 8);
        EXPECT_EQ(result, 13);
        subscriber.unbind();
        publisher.broadcast(12, 5);
        EXPECT_EQ(result, 13);
    }
    publisher.broadcast(12, 5);
    EXPECT_EQ(result, 13);
}

TEST(Publisher, BulletGame)
{
    class Actor
    {
    public:
        Actor() = default;
        virtual ~Actor() = default;
    };

    class PhysicBody
    {
    public:
        Publisher<void(Actor *)> onHit;

        void trigger_collision(Actor *actor)
        {
            onHit.broadcast(actor);
        }
    };

    class Character : public Actor
    {
    public:
        int health = 10;

        Character() = default;
    };

    class Bullet
    {
    public:
        std::shared_ptr<PhysicBody> body;
        Subscriber<void(Actor *)> on_body_hit;

        Bullet() : body(std::make_shared<PhysicBody>()),
                   on_body_hit(this, &Bullet::on_hit)
        {
            body->onHit.bind(on_body_hit);
        }

        void on_hit(Actor *actor)
        {
            Character *character = dynamic_cast<Character *>(actor);
            if (character != nullptr)
            {
                character->health -= 1;
            }
        }
    };

    Character character;

    EXPECT_EQ(character.health, 10);

    Bullet bullet;
    bullet.body->trigger_collision(&character);

    EXPECT_EQ(character.health, 9);

    bullet.body->trigger_collision(&character);
    EXPECT_EQ(character.health, 8);

    bullet.on_body_hit.unbind();
    bullet.body->trigger_collision(&character);
    EXPECT_EQ(character.health, 8);

}

// Run all the tests
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}