set(ENABLE_TESTS OFF)
if (NOT SKIP_TESTS AND GTEST_FOUND)
    set(ENABLE_TESTS ON)
endif ()

# Layer 0
add_subdirectory(Logging)
# Layer 1
add_subdirectory(Core)
# Layer 2
add_subdirectory(Scene)
# Layer 3
add_subdirectory(Physics)
add_subdirectory(Sound)
add_subdirectory(Widgets)
# Layer 4
add_subdirectory(Graphics)
# Layer 5
add_subdirectory(Window)
