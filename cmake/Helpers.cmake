function(setup_files)
    if (ARGV)
        set(LST ${ARGV})
    else ()
        set(LST include src)
    endif ()

    set(TMP)
    foreach (path IN ITEMS ${LST})
        file(GLOB_RECURSE LOCAL_SRCS
                "${path}/*.cpp"
                "${path}/*.hpp"
                "${path}/*.cc"
                "${path}/*.hh"
                "${path}/*.c"
                "${path}/*.h"
        )

        set(TMP ${TMP} ${LOCAL_SRCS})
    endforeach ()

    set(SRCS ${TMP} PARENT_SCOPE)
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
        message(TRACE "module ${module_name}: checking target dep ${DEP}")
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

    set("${module_name}_ALL_FOUND" ${ALL_FOUND} PARENT_SCOPE)
endfunction()

#[[
    function setup_module(
        NAME <module_name>
        [VARIABLE_DEPS <lst>...]
        [TARGET_DEPS <lst>...]
        [SPECIAL_HEADER_PATHS <lst>...]
        [SPECIAL_LIBS <lst>...]
        [FATAL_ERROR|WARNING]
        [ENABLE_TESTS]
    )

    This function will set up a module with its dependencies and tests. It
    expects to be run in a folder's module that follows the pattern:
        ---> <Module name>                  # Module root
            ---> include                    # Will be set as the public module's include path
            ---> src                        # Used as module's private include path
            ---> test                       # Used as testing root (if provided)
            ---> CMakeLists.txt             # Build system's entry point, will call this function

    It will expect that all generated config headers will be stored in
    ${PROJECT_BINARY_DIR}/include.
    It can take a list of special header paths and libraries to link with
    (like Vulkan's Vulkan_INCLUDE_DIRS and Vulkan_LIBRARIES).

    If the folder test is not found but ENABLE_TESTS is set, it will print a
    warning but will finish the generation.
    If the variable STONE_ENGINE_DISABLE_FULL_COMPILATION is set, it will only
    declare a variable holding source files. This option may be used by the
    pipeline that will generate the documentation to avoid the need of setting
    up all the dependencies (especially Vulkan and GLFW).
]]
function(setup_module)
    set(FLAGS ENABLE_TESTS FATAL_ERROR WARNING)
    set(SINGLE_VALUE_OPTIONS NAME)
    set(MULTI_VALUE_OPTIONS TARGET_DEPS VARIABLE_DEPS SPECIAL_HEADER_PATHS SPECIAL_LIBS)
    cmake_parse_arguments(PARSE_ARGV 0 SETUP_MODULE "${FLAGS}" "${SINGLE_VALUE_OPTIONS}" "${MULTI_VALUE_OPTIONS}")

    if (NOT DEFINED SETUP_MODULE_NAME)
        message(SEND_ERROR "Expecting argument NAME for function setup_module")
        return()
    endif ()

    setup_files(src include)

    set(SRCS_FOR_DOCS ${SRCS_FOR_DOCS} ${SRCS} CACHE INTERNAL "")
    if (NOT FULL_CONFIGURE)
        message(NOTICE "module ${SETUP_MODULE_NAME}: Won't declare targets")
        return()
    endif ()

    if (SETUP_MODULE_FATAL_ERROR)
        set(DEPS ${DEPS} FATAL_ERROR)
    elseif (SETUP_MODULE_WARNING)
        set(DEPS ${DEPS} WARNING)
    endif ()

    if (SETUP_MODULE_TARGET_DEPS)
        set(DEPS ${DEPS} TARGET_DEPS ${SETUP_MODULE_TARGET_DEPS})
    endif ()
    if (SETUP_MODULE_VARIABLE_DEPS)
        set(DEPS ${DEPS} VARIABLE_DEPS ${SETUP_MODULE_VARIABLE_DEPS})
    endif ()

    check_dependencies(${SETUP_MODULE_NAME} ${DEPS})
    set(ALL_FOUND "${SETUP_MODULE_NAME}_ALL_FOUND")
    if (NOT ${ALL_FOUND})
        message(SEND_ERROR "module ${SETUP_MODULE_NAME}: Some dependencies are missing")
        return()
    else ()
        message(TRACE "module ${SETUP_MODULE_NAME}: got ALL_FOUND = ${ALL_FOUND} -> ${${ALL_FOUND}}")
    endif ()


    add_library(${SETUP_MODULE_NAME} ${SRCS})
    target_include_directories(${SETUP_MODULE_NAME}
            PUBLIC include
            PRIVATE src
            PRIVATE ${PROJECT_BINARY_DIR}/include
    )
    target_enable_clang_tidy(${SETUP_MODULE_NAME})

    set(UNIX_COMPILER_IDS GNU AppleClang Clang)
    message(STATUS "module ${SETUP_MODULE_NAME}: Enabling warning flags for target")
    if (CMAKE_CXX_COMPILER_ID IN_LIST UNIX_COMPILER_IDS)
        message(DEBUG "module ${SETUP_MODULE_NAME}: Enabling -Wall -Wextra")
        target_compile_options(${SETUP_MODULE_NAME}
                PUBLIC -Wall
                PUBLIC -Wextra
        )

        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            message(NOTICE "module ${SETUP_MODULE_NAME}: Enabling error flag")
            message(DEBUG "module ${SETUP_MODULE_NAME}: Enabling flag -Werror")
            target_compile_options(${SETUP_MODULE_NAME} PRIVATE -Werror)
        endif ()
    else ()
        message(AUTHOR_WARNING "module ${SETUP_MODULE_NAME}: Current compiler isn't supported for enabling warning or error flags")
    endif ()

    if (DEFINED SETUP_MODULE_SPECIAL_HEADER_PATHS)
        foreach (path IN ITEMS ${SETUP_MODULE_SPECIAL_HEADER_PATHS})
            target_include_directories(${SETUP_MODULE_NAME} PRIVATE ${path})
        endforeach ()
    endif ()

    if (DEFINED SETUP_MODULE_TARGET_DEPS)
        foreach (dep IN ITEMS ${SETUP_MODULE_TARGET_DEPS})
            target_link_libraries(${SETUP_MODULE_NAME} PUBLIC ${dep})
        endforeach ()
    endif ()

    if (DEFINED SETUP_MODULE_SPECIAL_LIBS)
        foreach (dep IN ITEMS ${SETUP_MODULE_SPECIAL_LIBS})
            target_link_libraries(${SETUP_MODULE_NAME} PRIVATE ${dep})
        endforeach ()
    endif ()

    if (SETUP_MODULE_ENABLE_TESTS AND STONE_ENGINE_ENABLE_TESTS)
        if (IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/test")
            set(SRCS "") # Empties old SRCS
            setup_files(test)

            set(TEST_EXEC "test_${SETUP_MODULE_NAME}")
            add_executable(${TEST_EXEC} EXCLUDE_FROM_ALL ${SRCS})
            target_link_libraries(${TEST_EXEC}
                    PRIVATE GTest::gtest_main
                    PRIVATE ${SETUP_MODULE_NAME}
            )
            gtest_discover_tests(${TEST_EXEC})
        else ()
            message(AUTHOR_WARNING "module ${SETUP_MODULE_NAME}: tests was enabled but no directory test was found")
        endif ()
    endif ()
endfunction()

function(get_all_targets var)
    set(targets)
    get_all_targets_recursive(targets ${CMAKE_CURRENT_LIST_DIR})
    set(${var} ${targets} PARENT_SCOPE)
endfunction()

macro(get_all_targets_recursive targets dir)
    get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)

    foreach (subdir IN ITEMS ${subdirectories})
        get_all_targets_recursive(${targets} ${subdir})
    endforeach ()

    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${targets} ${current_targets})
endmacro()
