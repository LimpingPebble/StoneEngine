set(LIB_NAME networking)

setup_files()
check_dependencies(${LIB_NAME}
        TARGET_DEPS logging core utils
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC logging
        PUBLIC utils
        PUBLIC core
)
target_include_directories(${LIB_NAME}
        PUBLIC include
        PRIVATE ${PROJECT_BINARY_DIR}/include
)
