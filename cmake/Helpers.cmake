function(setup_files)
    file(GLOB_RECURSE LOCAL_SRCS "*.cpp" "*.hpp" "*.cc" "*.hh" "*.c" "*.h")
    set(SRCS ${LOCAL_SRCS} PARENT_SCOPE)
endfunction()

#[[
    function check_dependencies(
        <module_name>
        [VARIABLE_DEPS <lst>...]
        [TARGET_DEPS <lst>...]
        [FATAL_ERROR|WARNING]
    )

    This function checks dependencies and may print a warning or end the
    generation if some dependencies are missing.

    It will set ${module_name}_ALL_FOUND accordingly.
]]
function(check_dependencies module_name)
    set(FLAGS FATAL_ERROR WARNING)
    set(MULTI_VALUE_OPTIONS VARIABLE_DEPS TARGET_DEPS)
    cmake_parse_arguments(PARSE_ARGV 1 CHK_DEPS "${FLAGS}" "" "${MULTI_VALUE_OPTIONS}")

    set(ALL_FOUND ON)
    foreach (DEP IN ITEMS ${CHK_DEPS_TARGET_DEPS})
        message(DEBUG "module ${module_name}: checking target dep ${DEP}")
        if (NOT TARGET ${DEP})
            message(SEND_ERROR "${DEP} is required to build the ${module_name} module")
            set(ALL_FOUND OFF)
        endif ()
    endforeach ()

    foreach (VAR IN LISTS ${CHK_DEPS_VARIABLE_DEPS})
        message(STATUS "module ${module_name}: checking variable dep ${VAR}")
        if (NOT DEFINED VAR)
            message(SEND_ERROR "variable ${VAR} is required to build the ${module_name} module")
            set(ALL_FOUND OFF)
        endif ()
    endforeach ()

    if (NOT ALL_FOUND)
        if (CHK_DEPS_FATAL_ERROR)
            message(FATAL_ERROR "module ${module_name}: Some dependencies are missing")
        elseif (CHK_WARNING)
            message(WARNING "module ${module_name}: Some dependencies are missing")
        endif ()
    endif ()

    set("${module_name}_ALL_FOUND" ${ALL_FOUND})
endfunction()
