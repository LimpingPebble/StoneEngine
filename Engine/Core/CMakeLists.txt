set(LIB_NAME core)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS logging
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC logging
)
target_include_directories(${LIB_NAME}
        PRIVATE ${PROJECT_BINARY_DIR}/include
        PUBLIC include
)
