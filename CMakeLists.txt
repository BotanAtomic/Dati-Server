cmake_minimum_required(VERSION 3.12)
project(Dati C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-lpthread")

link_directories(${PROJECT_SOURCE_DIR}/json-c/.libs)

add_executable(Dati core/main.c network/server.c network/server.h utils/utils.h network/message_parser.c utils/buffer.c utils/buffer.h)

target_link_libraries(Dati json-c)
