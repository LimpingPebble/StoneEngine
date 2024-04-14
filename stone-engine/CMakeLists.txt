set(ENABLE_TESTS OFF)
if (NOT SKIP_TESTS AND GTEST_FOUND)
    set(ENABLE_TESTS ON)
endif ()

if (VULKAN_FOUND AND GLFW_FOUND)
    add_subdirectory(graphics)
endif ()

if (GLM_FOUND)
    add_subdirectory(logging)
endif ()

if (GLM_FOUND)
    add_subdirectory(physics)
endif ()

if (GLM_FOUND AND GLFW_FOUND)
    add_subdirectory(scene)
endif ()

if (GLM_FOUND)
    add_subdirectory(sound)
endif ()
