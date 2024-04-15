set (LIB_NAME graphics)

setup_files()

add_library(${LIB_NAME} ${SRCS}
        src/test.cpp)
