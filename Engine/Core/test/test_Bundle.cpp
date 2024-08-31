#include "Core/Assets/Bundle.hpp"

#include <gtest/gtest.h>

using namespace Stone::Core::Assets;

TEST(Bundle, RootDirectory) {
    Bundle bundle("test");
    EXPECT_EQ(bundle.getRootDirectory(), "test/");
}
