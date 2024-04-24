set(LIB_NAME utils)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS glm assimp json glfw
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC glm
        PUBLIC assimp
        PUBLIC json
        PUBLIC glfw
)
target_include_directories(${LIB_NAME}
        PRIVATE ${PROJECT_BINARY_DIR}/include
        PUBLIC include
)
