add_subdirectory(scene_usage)

get_all_targets(all_targets)
if (all_targets)
    message(DEBUG "adding examples target")
    add_custom_target(examples DEPENDS ${all_targets})
endif ()
