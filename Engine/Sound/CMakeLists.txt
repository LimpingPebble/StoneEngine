set (LIB_NAME sound)

#TODO: add OpenAL dependency
setup_files()
check_dependencies(${LIB_NAME}
        TARGET_DEPS scene logging
        FATAL_ERROR
)

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC logging
        PUBLIC scene
)
target_include_directories(${LIB_NAME}
        PUBLIC include
        PRIVATE ${PROJECT_BINARY_DIR}/include
)
