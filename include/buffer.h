//
// Created by Botan on 28/10/18.
//

#ifndef DATI_BUFFER_H
#define DATI_BUFFER_H

#include <sys/socket.h>

static const unsigned char PRIMITIVE_SIZE[] = {1, 1, 2, 2, 4, 4, 8, 8, 0, 0, 0};

void write_ubyte(unsigned char byte, int socket);

void write_ushort(__uint16_t u16, int socket);

void write_ulong(__uint64_t value, int socket);

void write_string(char *str, int socket);

char *read_string(size_t size, int socket);

__uint16_t read_ushort(int socket);

__uint32_t read_uint(int socket);

unsigned char read_ubyte(int socket);

__uint64_t read_ulong(int socket);

int16_t get_short(const char * data);

__uint16_t get_ushort(const char * data);

int32_t get_int(const char * data);

__uint32_t get_uint(const char * data);

int64_t get_long(const char * data);

__uint64_t get_ulong(const char * data);

#endif //DATI_BUFFER_H
