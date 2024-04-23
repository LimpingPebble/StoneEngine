set(LIB_NAME logging)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS utils
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC utils
)
target_include_directories(${LIB_NAME}
        PUBLIC include
        PRIVATE ${PROJECT_BINARY_DIR}/include
)
