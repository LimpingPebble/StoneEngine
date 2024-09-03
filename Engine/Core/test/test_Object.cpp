#include "Core/Object.hpp"

#include <gtest/gtest.h>

using namespace Stone;

class MockObject : public Core::Object {
	STONE_OBJECT(MockObject)
};

class MockSubObject : public MockObject {
	STONE_OBJECT(MockSubObject)
};

TEST(Object, ClassName) {
	auto mockObject = std::make_shared<MockObject>();
	EXPECT_STREQ(mockObject->getClassName(), "MockObject");

	auto mockSubObject = std::make_shared<MockSubObject>();
	EXPECT_STREQ(mockSubObject->getClassName(), "MockSubObject");

	const void *ptrFromObject = mockObject->getClassName();
	const void *ptrFromClass = MockObject::StaticClassName();

	EXPECT_EQ(ptrFromObject, ptrFromClass);

	EXPECT_EQ(mockObject->getClassName(), MockObject::StaticClassName());
}

TEST(Object, HashCode) {
	auto mockObject = std::make_shared<MockObject>();
	auto mockSubObject = std::make_shared<MockSubObject>();

	EXPECT_EQ(mockObject->getClassHashCode(), MockObject::StaticHashCode());
	EXPECT_EQ(mockSubObject->getClassHashCode(), MockSubObject::StaticHashCode());
}

TEST(Object, SwitchClassName) {

	auto mockObject = std::make_shared<MockObject>();
	auto mockSubObject = std::make_shared<MockSubObject>();

	std::map<std::intptr_t, int> classCaster = {
		{ Core::Object::StaticHashCode(), 0},
		 {	  MockObject::StaticHashCode(), 1},
		{MockSubObject::StaticHashCode(), 2}
	};

	EXPECT_EQ(classCaster[mockObject->getClassHashCode()], 1);
	EXPECT_EQ(classCaster[mockSubObject->getClassHashCode()], 2);
}

TEST(Object, Id) {
	auto object = std::make_shared<MockObject>();
	auto object2 = std::make_shared<MockObject>();

	EXPECT_NE(object->getId(), object2->getId());
}
