//
// Created by Botan on 28/10/18.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

void write_ubyte(unsigned char byte, int socket) {
    send(socket, &byte, 1, 0);
}

void write_ushort(__uint16_t u16, int socket) {
    unsigned char buffer[2];
    buffer[0] = (unsigned char) (u16 & 0xFF);
    buffer[1] = (unsigned char) ((u16 >> 8) & 0xFF);
    send(socket, buffer, 2, 0);
}

void write_ulong(__uint64_t value, int socket) {
    unsigned char buffer[8];

    for (int i = 0; i < 8; i++)
        buffer[i] = (unsigned char) ((value >> (8 * i) != 0));

    send(socket, buffer, 8, 0);
}

void write_string(char *str, int socket) {
    send(socket, str, strlen(str), 0);
}

char *read_string(size_t size, int socket) {
    char *buffer = malloc(size + 1);
    recv(socket, buffer, size, 0);
    buffer[size] = '\0';
    return buffer;
}

__uint16_t read_ushort(int socket) {
    unsigned char buffer[2];
    recv(socket, buffer, 2, 0);
    return (__uint16_t) buffer[0] | ((__uint16_t) buffer[1] << 8);
}

unsigned char read_ubyte(int socket) {
    unsigned char buffer;
    recv(socket, &buffer, 1, 0);
    return buffer;
}

__uint64_t read_ulong(int socket) {
    unsigned char buffer[8];
    recv(socket, buffer, 8, 0);

    __uint64_t value = 0;

    for (int i = 0; i < 8; i++)
        value += (buffer[i] << (8 * i));

    return value;
}