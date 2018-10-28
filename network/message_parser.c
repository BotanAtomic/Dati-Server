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

#define SUCCESS 1
#define FAILED 0

void (*messages[])(session *session, __uint16_t size) = {login};


void login(session *session, __uint16_t size) {
    unsigned char user_length = read_ubyte(session->socket);

    char *buffer = read_string(size, session->socket);

    if (strncmp(buffer, username, user_length) == 0 &&
        strncmp(buffer + user_length, password, (size - user_length)) == 0) {
        printf("login success\n");
    } else {
        printf("login failed\n");
    }

}

void parse(unsigned char id, session *session) {
    printf("Reception of message_id : %d \n", id);

    __uint16_t size = read_ushort(session->socket);

    printf("Size : %d \n", size);

    if (session->connected || id == 0) {
        messages[id](session, size);
    }


}