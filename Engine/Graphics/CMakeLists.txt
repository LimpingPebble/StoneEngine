setup_module(
        NAME graphics
        TARGET_DEPS logging widgets utils
        VARIABLE_DEPS Vulkan_LIBRARIES Vulkan_INCLUDE_DIRS
        SPECIAL_HEADER_PATHS ${Vulkan_INCLUDE_DIRS}
        SPECIAL_LIBS ${Vulkan_LIBRARIES}
        FATAL_ERROR
)
