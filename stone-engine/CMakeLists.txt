set(ENABLE_TESTS OFF)
if (NOT SKIP_TESTS AND GTEST_FOUND)
    set(ENABLE_TESTS ON)
endif ()

# Layer 0
add_subdirectory(logging)
# Layer 1
add_subdirectory(core)
# Layer 2
add_subdirectory(scene)
# Layer 3
add_subdirectory(physics)
add_subdirectory(sound)
add_subdirectory(widgets)
# Layer 4
add_subdirectory(graphics)
# Layer 5
add_subdirectory(window)
