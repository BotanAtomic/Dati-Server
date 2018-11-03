//
// Created by Botan on 27/10/18.
//

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "stdio.h"
#include "server.h"
#include "message_parser.h"
#include "buffer.h"
#include "../shell/shell.h"
#include "error_code.c"

#define SUCCESS 1
#define FAILED 0

void (*messages[])(session *session, __uint16_t size) ={
        login, get_databases, create_database
};


void parse(unsigned char id, session *session) {
    __uint16_t size = read_ushort(session->socket);

    println("Reception of [message_id : %d / size : %d]", id, size);

    if (session->connected || id == 0) {
        messages[id](session, size);
    }
}

void login(session *session, __uint16_t size) {
    unsigned char response[2] = {0};

    unsigned char user_length = read_ubyte(session->socket);

    char *buffer = read_string(size, session->socket);

    if (strncmp(buffer, username, user_length) == 0 &&
        strncmp(buffer + user_length, password, (size - user_length)) == 0) {
        response[1] = SUCCESS;
        session->connected = 1;
    } else {
        response[1] = FAILED;
        session->connected = 0;
    }

    send(session->socket, response, 2, 0);
}


void get_databases(session *session, __uint16_t size) {

}

void create_database(session *session, __uint16_t size) {
    unsigned char response[2] = {2};

    char *database = read_string(size, session->socket);

    __uint16_t path_size = (__uint16_t) (strlen(data_path) + strlen(database) + 2);

    char *path = malloc(path_size);
    snprintf(path, path_size, "%s/%s", data_path, database);

    int result = mkdir(path, 0777);

    response[1] = (unsigned char) (result == 0 ? 1 : 0);

    free(path);

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte(DATABASE_ALREADY_EXIST, session->socket);
}
