add_subdirectory(scene_usage)
add_subdirectory(scop)

get_all_targets(all_targets)
if (all_targets)
    message(DEBUG "adding examples target")
    add_custom_target(examples DEPENDS ${all_targets})
endif ()
