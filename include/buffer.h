//
// Created by Botan on 28/10/18.
//

#ifndef DATI_BUFFER_H
#define DATI_BUFFER_H

#include <sys/socket.h>

void write_ubyte(unsigned char byte, int socket);

void write_ushort(__uint16_t u16, int socket);

void write_ulong(__uint64_t value, int socket);

void write_string(char *str, int socket);

char *read_string(size_t size, int socket);

__uint16_t read_ushort(int socket);

unsigned char read_ubyte(int socket);

__uint64_t read_ulong(int socket);

#endif //DATI_BUFFER_H
