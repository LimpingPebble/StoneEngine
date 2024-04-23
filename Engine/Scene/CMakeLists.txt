set(LIB_NAME scene)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS glm assimp core logging json utils
        VARIABLE_DEPS assimp_SOURCE_DIR
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC glm
        PUBLIC assimp
        PUBLIC json
        PUBLIC core
        PUBLIC utils
        PUBLIC logging
)
target_include_directories(${LIB_NAME}
        PRIVATE ${PROJECT_BINARY_DIR}/include
        PUBLIC ${assimp_SOURCE_DIR}/include
        PUBLIC include
)
