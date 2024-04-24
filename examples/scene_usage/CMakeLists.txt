set(NAME scene_usage)

add_executable(${NAME} main.cpp test_Node.hpp
        ../../Engine/Core/test/test.cpp)
target_include_directories(${NAME} PRIVATE ${PROJECT_BINARY_DIR}/include)
target_link_libraries(${NAME} PRIVATE scene)
