#include "Utils/SigSlot.hpp"

#include <gtest/gtest.h>

using namespace Stone;

void add(int a, int b, int &result) {
	result = a + b;
}

TEST(Signal, SubscribeWithFunction) {
	int result = 0;

	Signal<int, int, int &> signal;
	signal.broadcast(5, 5, result);
	EXPECT_EQ(result, 0);
	{
		Slot<int, int, int &> slot(add);
		signal.bind(slot);
		signal.broadcast(5, 8, result);
		EXPECT_EQ(result, 13);
	}
	signal.broadcast(12, 5, result);
	EXPECT_EQ(result, 13);
}

TEST(Signal, SubscribeWithLambda) {
	int result = 0;
	auto addition = [&result](int a, int b) {
		result = a + b;
	};

	Signal<int, int> signal;
	signal.broadcast(5, 5);
	EXPECT_EQ(result, 0);
	{
		Slot<int, int> slot(addition);
		signal.bind(slot);
		signal.broadcast(5, 8);
		EXPECT_EQ(result, 13);
	}
	signal.broadcast(12, 5);
	EXPECT_EQ(result, 13);
}

TEST(Signal, SubscribeWithMethod) {
	struct Calc {
		int result = 0;
		void add_numbers(int a, int b) {
			result = a + b;
		}
	};

	Calc calc;
	EXPECT_EQ(calc.result, 0);

	Signal<int, int> signal;
	{
		Slot<int, int> slot(std::bind(&Calc::add_numbers, &calc, std::placeholders::_1, std::placeholders::_2));
		signal.bind(slot);
		signal(5, 8);
		EXPECT_EQ(calc.result, 13);
	}
	signal(12, 5);
	EXPECT_EQ(calc.result, 13);
}

TEST(Signal, SubscribeWithMethodConst) {
	struct Calc {
		Calc(int m) : modifier(m) {
		}
		int modifier;

		void add_numbers(int a, int b, int &result) const {
			result = modifier * (a + b);
		}
	};

	const Calc calc(3);
	int result = 0;

	Signal<int, int, int &> signal;

	{
		Slot<int, int, int &> slot(
			std::bind(&Calc::add_numbers, &calc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

		signal.bind(slot);

		signal(2, 3, result);
		EXPECT_EQ(result, 15);
	}

	signal(12, 5, result);

	EXPECT_EQ(result, 15);
}

TEST(Signal, Unbind) {
	int result = 0;
	auto addition = [&result](int a, int b) {
		result = a + b;
	};

	Signal<int, int> signal;
	{
		Slot<int, int> slot(addition);
		signal.bind(slot);
		signal.broadcast(5, 8);
		EXPECT_EQ(result, 13);
		slot.unbind();
		signal.broadcast(12, 5);
		EXPECT_EQ(result, 13);
	}
	signal.broadcast(12, 5);
	EXPECT_EQ(result, 13);
}

TEST(Signal, MultipleRebind) {
	int count = 0;
	auto increment = [&count](int addition) {
		count += addition;
	};

	Slot<int> sub(increment);

	Signal<int> pub1;
	Signal<int> pub2;
	Signal<int> pub3;

	pub1.bind(sub);
	pub2.bind(sub);
	pub3.bind(sub);
	// sub should only be bind to the last one

	pub1.broadcast(3);
	pub2.broadcast(7);
	pub3.broadcast(13);

	EXPECT_EQ(count, 13);
}

TEST(Signal, BulletGame) {
	class Actor {
	public:
		Actor() = default;
		virtual ~Actor() = default;
	};

	class PhysicBody {
	public:
		Signal<Actor *> onHit;

		void trigger_collision(Actor *actor) {
			onHit.broadcast(actor);
		}
	};

	class Character : public Actor {
	public:
		int health;
		Signal<Character *> on_death;

		Character() : Actor(), health(10) {
		}

		void getHit() {
			health -= 3;
			if (health <= 0)
				on_death.broadcast(this);
		}
	};

	class Bullet : public Actor {
	public:
		std::shared_ptr<PhysicBody> body;
		Slot<Actor *> on_body_hit;

		Bullet()
			: Actor(), body(std::make_shared<PhysicBody>()),
			  on_body_hit(std::bind(&Bullet::on_hit, this, std::placeholders::_1)) {
			body->onHit.bind(on_body_hit);
		}

		void on_hit(Actor *actor) {
			Character *character = dynamic_cast<Character *>(actor);
			if (character != nullptr) {
				character->getHit();
			}
		}
	};

	Character character;

	bool character_died = false;
	Slot<Character *> char_died_sub([&character_died](Character *) { character_died = true; });

	character.on_death.bind(char_died_sub);

	EXPECT_EQ(character.health, 10);

	Bullet bullet;
	bullet.body->trigger_collision(&character);
	EXPECT_EQ(character.health, 7);

	bullet.body->trigger_collision(&character);
	EXPECT_EQ(character.health, 4);

	bullet.on_body_hit.unbind();
	bullet.body->trigger_collision(&character);
	EXPECT_EQ(character.health, 4);
	EXPECT_FALSE(character_died);

	Bullet bullet2;
	bullet2.body->trigger_collision(&character);
	EXPECT_EQ(character.health, 1);
	EXPECT_FALSE(character_died);

	bullet2.body->trigger_collision(&character);
	EXPECT_LE(character.health, 0);
	EXPECT_TRUE(character_died);
}
