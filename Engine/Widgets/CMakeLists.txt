set(LIB_NAME widgets)

setup_files()

check_dependencies(
        ${LIB_NAME}
        TARGET_DEPS logging scene utils
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC scene
        PUBLIC utils
        PUBLIC logging
)
target_include_directories(${LIB_NAME}
        PUBLIC include
        PRIVATE ${PROJECT_BINARY_DIR}/include
)
