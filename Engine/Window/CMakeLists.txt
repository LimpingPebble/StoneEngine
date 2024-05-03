setup_module(
        NAME window
        TARGET_DEPS glfw render logging scene utils
        VARIABLE_DEPS Vulkan_LIBRARIES Vulkan_INCLUDE_DIRS
        SPECIAL_HEADER_PATHS ${Vulkan_INCLUDE_DIRS}
        SPECIAL_LIBS ${Vulkan_LIBRARIES}
        FATAL_ERROR
)
