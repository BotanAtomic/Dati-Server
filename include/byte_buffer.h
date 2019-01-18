//
// Created by Botan on 08/12/18.
//

#ifndef DATI_BYTE_BUFFER_H
#define DATI_BYTE_BUFFER_H

#include <malloc.h>
#include <memory.h>

typedef struct ByteBuffer {
    unsigned long position;
    unsigned long length;
    char *data;
} ByteBuffer;

ByteBuffer *newByteBuffer(char *, unsigned long);

void byteBufferWrite(ByteBuffer *,  void *__restrict __ptr, unsigned long);

void byteBufferRead(ByteBuffer *, void *__restrict __ptr, unsigned long);

void freeByteBuffer(ByteBuffer *);

#endif //DATI_BYTE_BUFFER_H

