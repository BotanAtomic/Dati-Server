//
// Created by Botan on 28/10/18.
//

#ifndef DATI_BUFFER_H
#define DATI_BUFFER_H

#endif //DATI_BUFFER_H

#include <sys/socket.h>

char* read_string(size_t size, int socket);

__uint16_t read_ushort(int socket);

unsigned char read_ubyte(int socket);