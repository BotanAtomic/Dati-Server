//
// Created by Botan on 28/10/18.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

char *read_string(size_t size, int socket) {
    char *buffer = malloc(size);
    recv(socket, buffer, size, 0);
    return buffer;
}

__uint16_t read_ushort(int socket) {
    unsigned char buffer[2];
    recv(socket, buffer, 2, 0);
    return (__uint16_t) buffer[0] | ((__uint16_t) buffer[1] << 8);
}

unsigned char read_ubyte(int socket) {
    unsigned char buffer[1];
    recv(socket, buffer, 1, 0);
    return buffer[0];
}