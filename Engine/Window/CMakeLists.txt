set(LIB_NAME window)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS glfw graphics logging scene
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC glfw
        PUBLIC graphic
        PUBLIC logging
        PUBLIC scene
)
target_include_directories(${LIB_NAME}
    PUBLIC include
        PRIVATE ${PROJECT_BINARY_DIR}/include
)
