get_subdirs(EXAMPLE_DIRS ${CMAKE_CURRENT_LIST_DIR})

foreach (EXAMPLE_DIR IN ITEMS ${EXAMPLE_DIRS})
    message(STATUS "Adding example ${EXAMPLE_DIR}")
    add_subdirectory(${EXAMPLE_DIR})
endforeach ()

get_all_targets(all_targets)
if (all_targets)
    message(DEBUG "adding examples target")
    add_custom_target(examples DEPENDS ${all_targets})
endif ()
