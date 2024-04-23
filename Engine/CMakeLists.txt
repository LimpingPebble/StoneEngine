set(ENABLE_TESTS OFF)
if (NOT SKIP_TESTS AND GTEST_FOUND)
    set(ENABLE_TESTS ON)
endif ()

# Layer 0
add_subdirectory(Utils)
# Layer 1
add_subdirectory(Logging)
# Layer 2
add_subdirectory(Core)
# Layer 3
add_subdirectory(Scene)
# Layer 4
add_subdirectory(Physics)
# Layer 5
add_subdirectory(Sound)
add_subdirectory(Widgets)
add_subdirectory(Graphics)
# Layer 6
add_subdirectory(Window)
