set (LIB_NAME logging)

setup_files()

add_library(${LIB_NAME} ${SRCS})
target_include_directories(${LIB_NAME}
    PUBLIC include
    PRIVATE ${PROJECT_BINARY_DIR}/include
)
