cmake_minimum_required(VERSION 3.12)
project(Dati C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-lpthread -lm")

link_directories(${PROJECT_SOURCE_DIR}/lib/json-c/.libs)

include_directories(include lib)

add_executable(Dati src/core/main.c src/network/server.c include/server.h src/utils/utils.c include/utils.h src/network/buffer.c include/buffer.h
        src/shell/shell.c include/shell.h include/error_code.h src/network/message_parser.c include/message_parser.h include/container.h
        include/variable.h src/database/database.c include/database.h include/list.h src/list/list.c include/node.h include/tree.h src/tree/tree.c include/comparator.h src/comparator/comparaor.c)

target_link_libraries(Dati json-c)
