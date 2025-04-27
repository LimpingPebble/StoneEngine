set(NAME scop)

add_executable(${NAME} EXCLUDE_FROM_ALL main.cpp RotatingNode.cpp)
target_include_directories(${NAME} PRIVATE ${PROJECT_BINARY_DIR}/include)
target_link_libraries(${NAME}
		PRIVATE scene
		PRIVATE window

)
