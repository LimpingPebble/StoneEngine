set(LIB_NAME core)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS logging
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        INTERFACE logging
)
target_include_directories(
        ${LIB_NAME}
        INTERFACE include
)
