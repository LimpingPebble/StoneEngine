set(LIB_NAME scene)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS glm assimp core logging
        VARIABLE_DEPS assimp_SOURCE_DIR
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        INTERFACE glm
        PUBLIC assimp
        INTERFACE core
        INTERFACE logging
)
target_include_directories(${LIB_NAME}
        PUBLIC ${assimp_SOURCE_DIR}/include
        INTERFACE include
)
