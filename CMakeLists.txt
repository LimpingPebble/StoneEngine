cmake_minimum_required(VERSION 3.25)
project(StoneEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(USE_SYSTEM_GLM OFF CACHE BOOL "Uses glm from system libraries")
set(USE_SYSTEM_BOOST ON CACHE BOOL "Uses boost from system libraries")
set(SKIP_TESTS ON CACHE BOOL "Disables tests")
set(USE_SYSTEM_PAUSE OFF CACHE BOOL "Enables the use of Windows' pause after ending console only programs. (Windows only)")
set(FULL_CONFIGURE ON CACHE BOOL "Full configure project (may be used in pipeline to avoid the setup of all the dependencies)")
set(ENABLE_DOCS OFF CACHE BOOL "Builds documentation with doxygen")

# TODO: dependencies are not mandatory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(ClangTidy)
include(Helpers)
include(OSDetection)

include(FetchContent)

if ( FULL_CONFIGURE )
	# Require Vulkan
	find_package(Vulkan)
	find_package(Python REQUIRED COMPONENTS Interpreter)

	if ( ${Python_VERSION_MAJOR} STRLESS 3 AND ${Python_VERSION_MINOR} STRLESS 10 )
		message(FATAL_ERROR "Got bad version of python: ${Python_VERSION}")
	endif ()

	# Require ShaderC (google's shader compiler, compatible with GLSL or HLSL)
	FetchContent_Declare(
			shaderc
			GIT_REPOSITORY https://github.com/google/shaderc
			GIT_TAG 9a658e242ad4d1a4b3491383c1c58c780e3c01ff # tag/v2024.1
	)
	FetchContent_GetProperties(shaderc)

	if ( NOT shaderc_POPULATED )
		FetchContent_Populate(shaderc)
		set($ENV{GIT_SYNC_DEPS_QUIET} 1)
		execute_process(
				COMMAND ${Python_EXECUTABLE} utils/git-sync-deps
				WORKING_DIRECTORY ${shaderc_SOURCE_DIR}
				COMMAND_ERROR_IS_FATAL ANY
				OUTPUT_FILE git-sync-deps.out
		)

		set(SHADERC_SKIP_TESTS ON CACHE INTERNAL "Disables tests in ShaderC")
		set(SHADERC_SKIP_EXAMPLES ON CACHE INTERNAL "Disables examples in ShaderC")
		set(SHADERC_SKIP_INSTALL ON CACHE INTERNAL "Disables installation in ShaderC")

		add_subdirectory(${shaderc_SOURCE_DIR} ${shaderc_BINARY_DIR})
	endif ()

	# Fetches GLM
	if ( USE_SYSTEM_GLM )
		find_package(glm)

		if ( NOT GLM_FOUND )
			message(STATUS "Couldn't find glm in system, will use online source instead")
		endif ()
	else ()
		message(STATUS "Will fetch glm from online source")
	endif ()

	if ( NOT GLM_FOUND )
		FetchContent_Declare(
				glm
				GIT_REPOSITORY https://github.com/g-truc/glm.git
				GIT_TAG 33b0eb9fa336ffd8551024b1d2690e418014553b # refs/tags/1.0.0
		)

		set(GLM_ENABLE_CXX_17 ON CACHE INTERNAL "Forces C++17 in GLM")

		FetchContent_MakeAvailable(glm)
		set(GLM_FOUND ON)
		set_target_properties(glm PROPERTIES SYSTEM ON)
	endif ()

	# Fetches GoogleTest only if needed
	if ( NOT SKIP_TESTS )
		FetchContent_Declare(
				gtest
				GIT_REPOSITORY https://github.com/google/googletest
				GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571
		)

		# For Windows: Prevent overriding the parent project's compiler/linker settings
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
		FetchContent_MakeAvailable(gtest)
		set_target_properties(gtest PROPERTIES SYSTEM ON)

		if ( NOT SKIP_TESTS )
			enable_testing()
		endif ()
	endif ()

	# Setup Glfw
	FetchContent_Declare(
			glfw
			GIT_REPOSITORY https://github.com/glfw/glfw
			GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579 # tag/3.4
	)
	FetchContent_GetProperties(glfw)

	if ( NOT glfw_POPULATED )
		FetchContent_Populate(glfw)

		set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
		set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
		set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
		set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

		if ( STONE_ENGINE_OS_NAME STREQUAL Linux )
			set(GLFW_BUILD_WAYLAND OFF CACHE INTERNAL "Enables the use of Wayland (Linux only)")
			set(GLFW_BUILD_X11 ON CACHE INTERNAL "Enables the use of X11 (Linux only)")
		endif ()

		add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
	endif ()

	set_target_properties(glfw PROPERTIES SYSTEM ON)

	# Setup Json
	FetchContent_Declare(
			json
			GIT_REPOSITORY https://github.com/nlohmann/json
			GIT_TAG 9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03 # tag/v3.11.3
	)
	FetchContent_MakeAvailable(json)
	add_library(json ALIAS nlohmann_json)

	set_target_properties(nlohmann_json PROPERTIES SYSTEM ON)

	# Setup Assimp
	FetchContent_Declare(
			assimp
			GIT_REPOSITORY https://github.com/assimp/assimp
			GIT_TAG 8b9ed34eaa3e6ad24254cb7e058fb9150f66b865 # tag/v5.4.0
	)
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
	set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
	set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(assimp)

	set_target_properties(assimp PROPERTIES SYSTEM ON)

	if ( USE_SYSTEM_BOOST )
		find_package(Boost 1.74.0)
	endif ()

	if ( NOT USE_SYSTEM_BOOST OR NOT Boost_FOUND )
		set(BOOST_GIT_TAG a7090e8ce184501cfc9e80afa6cafb5bfd3b371c) # tag/boost-1.74.0
		FetchContent_Declare(Boost
				GIT_REPOSITORY https://github.com/boostorg/boost
				GIT_TAG ${BOOST_GIT_TAG}
		)
		if ( NOT Boost_POPULATED )
			message(STATUS "Fetching Boost from git repository")

			FetchContent_Populate(Boost)

			message(STATUS "Configuring Boost")
			add_subdirectory(${boost_SOURCE_DIR} ${boost_BINARY_DIR} EXCLUDE_FROM_ALL)
		endif ()
	endif ()
endif ()

add_subdirectory(Engine)

if ( FULL_CONFIGURE )
	add_subdirectory(examples)
endif ()

if ( ENABLE_DOCS )
	include(Doxygen)
endif ()
