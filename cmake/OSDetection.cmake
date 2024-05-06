set(keys OS_NAME)

foreach ( key IN LISTS keys )
	cmake_host_system_information(RESULT STONE_ENGINE_${key} QUERY ${key})
endforeach ()

set(STONE_ENGINE_USE_SYSTEM_PAUSE ${USE_SYSTEM_PAUSE})
if ( NOT STONE_ENGINE_OS_NAME STREQUAL Windows )
	set(STONE_ENGINE_USE_SYSTEM_PAUSE OFF)
endif ()

configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/include/config.h @ONLY)
