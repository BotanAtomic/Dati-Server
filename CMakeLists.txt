cmake_minimum_required(VERSION 3.12)
project(Dati C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-lpthread")

link_directories(${PROJECT_SOURCE_DIR}/json-c/.libs)

add_executable(Dati core/main.c network/server.c network/server.h utils/utils.c utils/utils.h network/buffer.c network/buffer.h
        shell/shell.c shell/shell.h network/error_code.c network/message_parser.c network/message_parser.h)

target_link_libraries(Dati json-c)
