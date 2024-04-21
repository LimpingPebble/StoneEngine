set(LIB_NAME window)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS glfw graphics logging scene
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC glfw
        INTERFACE graphic
        INTERFACE logging
        INTERFACE scene
)
