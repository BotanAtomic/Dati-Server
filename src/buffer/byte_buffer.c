//
// Created by Botan on 08/12/18.
//
#include "byte_buffer.h"

ByteBuffer *newByteBuffer(char *data, unsigned long length) {
    ByteBuffer *byteBuffer = malloc(sizeof(ByteBuffer));
    byteBuffer->position = 0;
    byteBuffer->length = length;
    byteBuffer->data = data;
    return byteBuffer;
}

void byteBufferWrite(ByteBuffer *byteBuffer,  void *__restrict __ptr, unsigned long length) {
    byteBuffer->data = realloc(byteBuffer->data, byteBuffer->length + length);
    memcpy(byteBuffer->data + byteBuffer->length, __ptr, length);
    byteBuffer->length += length;
    byteBuffer->position += length;
}

void byteBufferRead(ByteBuffer *byteBuffer, void *__restrict __ptr, unsigned long length) {
    memcpy(__ptr, byteBuffer->data + byteBuffer->position, length);
    byteBuffer->position += length;
}

void freeByteBuffer(ByteBuffer *byteBuffer) {
    free(byteBuffer->data);
    free(byteBuffer);
}
