set(LIB_NAME widgets)

setup_files()

check_dependencies(
        ${LIB_NAME}
        TARGET_DEPS logging scene
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        INTERFACE scene
        INTERFACE logging
)
