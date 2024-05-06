function (clang_tidy_validator result item)
	execute_process(
			COMMAND ${item} --version
			OUTPUT_VARIABLE clang_tidy_stdout
	)

	if ( ${clang_tidy_stdout} MATCHES "version ([0-9]+)\.([0-9]+)\.([0-9]+)" )
		set(MAJOR ${CMAKE_MATCH_1})
		set(MINOR ${CMAKE_MATCH_2})
		set(PATCH ${CMAKE_MATCH_3})

		message(STATUS "Found clang-tidy version ${MAJOR}.${MINOR}.${PATCH}")
		if ( MAJOR LESS 17 )
			message(WARNING "Found major version that may not work for all tests")
		elseif ( MAJOR LESS_EQUAL 15 )
			set(${result} FALSE PARENT_SCOPE)
		endif ()
	else ()
		message(WARNING "Couldn't read clang-tidy version")
		set(${result} FALSE PARENT_SCOPE)
	endif ()
endfunction ()

find_program(
		CLANG_TIDY_EXE
		NAMES clang-tidy-18 clang-tidy-17 clang-tidy
		PATHS /opt/homebrew/opt/llvm/bin
		VALIDATOR clang_tidy_validator
)

if ( NOT CLANG_TIDY_EXE )
	message(WARNING "clang-tidy not found, disabling tidy checks")
	set(ENABLE_CLANG_TIDY OFF CACHE BOOL "Enables clang tidy checks")
else ()
	set(ENABLE_CLANG_TIDY ON CACHE BOOL "Enables clang tidy checks")
	set(CLANG_TIDY_CMD_LINE ${CLANG_TIDY_EXE} --export-fixes=fixes.yml)
endif ()


macro (target_enable_clang_tidy TARGET)
	if ( ENABLE_CLANG_TIDY AND TARGET ${TARGET} )
		set_target_properties(${TARGET} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_CMD_LINE}")
		set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_CMD_LINE}")
	endif ()
endmacro ()

file(WRITE ${PROJECT_BINARY_DIR}/.clang-tidy "Checks: '.*'")
