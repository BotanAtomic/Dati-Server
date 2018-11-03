//
// Created by Botan on 27/10/18.
//

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "server.h"
#include "message_parser.h"
#include "../utils/buffer.h"
#include "../shell/shell.h"

#define SUCCESS 1
#define FAILED 0

void (*messages[])(session *session, __uint16_t size) = {login};


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

void parse(unsigned char id, session *session) {
    __uint16_t size = read_ushort(session->socket);

    println("Reception of [message_id : %d / size : %d", id, size);

    if (session->connected || id == 0) {
        messages[id](session, size);
    }


}