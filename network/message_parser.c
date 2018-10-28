//
// Created by Botan on 27/10/18.
//

#include <sys/socket.h>
#include "stdio.h"
#include "server.h"
#include "message_parser.h"


void (*messages[])(session *session, __uint16_t size) = {NULL};


void login(session *session, __uint16_t size) {
    //logic here



}

void parse(unsigned char id, session *session) {
    printf("Reception of message_id : %d \n", id);

    unsigned char size_buffer[2];

    if (recv(session->socket, size_buffer, 2, 0) > 0) {

        __uint16_t size = (__uint16_t) size_buffer[0] | ((__uint16_t) size_buffer[1] << 8);

        printf("Size : %d \n",size);

        if (session->connected) {
            messages[id](session, size);
        } else if (id == 0) {
            login(session, size);
        }
    }


}