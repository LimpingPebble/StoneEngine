set(STONE_ENGINE_ENABLE_TESTS OFF)
if (NOT SKIP_TESTS AND TARGET GTest::gtest_main)
    set(STONE_ENGINE_ENABLE_TESTS ON)
    enable_testing()
    include(GoogleTest)
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

get_all_targets(all_targets)
set(test_targets)
foreach (target IN ITEMS ${all_targets})
    if (target MATCHES "^test_")
        message(DEBUG "Matching target: ${target}")
        list(APPEND test_targets ${target})
    endif ()
endforeach ()

if (test_targets)
    message(DEBUG "adding unit test target")
    add_custom_target(test DEPENDS ${test_targets})
endif ()
