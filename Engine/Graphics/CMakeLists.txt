set(LIB_NAME graphics)

setup_files()

check_dependencies(${LIB_NAME}
        TARGET_DEPS logging widgets
        VARIABLE_DEPS Vulkan_LIBRARIES Vulkan_INCLUDE_DIRS
        FATAL_ERROR
)

check_dependencies(sd)
add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
        PUBLIC ${Vulkan_LIBRARIES}
        PUBLIC logging
        PUBLIC widgets
)
target_include_directories(${LIB_NAME}
        PRIVATE ${PROJECT_BINARY_DIR}/include
        PUBLIC ${Vulkan_INCLUDE_DIRS}
)
