//
// Created by Botan on 28/10/18.
//

#include "buffer.h"

void writeByte(char byte, int socket) {
    send(socket, &byte, 1, 0);
}

void writeUByte(unsigned char byte, int socket) {
    send(socket, &byte, 1, 0);
}

void writeShort(int16_t short_value, int socket) {
    char buffer[2];
    memcpy(buffer, &short_value, 2);
    send(socket, buffer, 2, 0);
}


void writeUShort(__uint16_t u16, int socket) {
    char buffer[2];
    memcpy(buffer, &u16, 2);
    send(socket, buffer, 2, 0);
}

void writeInt(int32_t value, int socket) {
    char buffer[4];

    memcpy(buffer, &value, 4);

    send(socket, buffer, 4, 0);
}

void writeUInt(__uint32_t value, int socket) {
    unsigned char buffer[4];

    memcpy(buffer, &value, 4);

    send(socket, buffer, 4, 0);
}

void writeLong(int64_t value, int socket) {
    unsigned char buffer[8];

    memcpy(buffer, &value, 8);

    send(socket, buffer, 8, 0);
}

void writeULong(__uint64_t value, int socket) {
    unsigned char buffer[8];

    memcpy(buffer, &value, 8);

    send(socket, buffer, 8, 0);
}

void writeString(char *str, int socket) {
    send(socket, str, strlen(str), 0);
}

char *readString(size_t size, int socket) {
    char *buffer = malloc(size + 1);
    recv(socket, buffer, size, 0);
    buffer[size] = '\0';
    return buffer;
}

__uint16_t readUShort(int socket) {
    unsigned char buffer[2];
    recv(socket, buffer, 2, 0);
    return (__uint16_t) buffer[0] | ((__uint16_t) buffer[1] << 8);
}

__uint32_t readUInt(int socket) {
    unsigned char buffer[4];
    recv(socket, buffer, 4, 0);

    __uint32_t value = 0;

    for (int i = 0; i < 4; i++)
        value += (buffer[i] << (8 * i));

    return value;
}


unsigned char readUByte(int socket) {
    unsigned char buffer;
    recv(socket, &buffer, 1, 0);
    return buffer;
}

__uint64_t readULong(int socket) {
    unsigned char buffer[8];
    recv(socket, buffer, 8, 0);

    __uint64_t value = 0;

    for (int i = 0; i < 8; i++)
        value += (buffer[i] << (8 * i));

    return value;
}

int16_t getShort(const char *buffer) {
    int16_t value = 0;

    memcpy(&value, buffer, 2);

    return value;
}

__uint16_t getUShort(const char *buffer) {
    __uint16_t value = 0;

    memcpy(&value, buffer, 2);

    return value;
}

__uint32_t getUInt(const char *buffer) {
    __uint32_t value = 0;

    memcpy(&value, buffer, 4);

    return value;
}

int32_t getInt(const char *buffer) {
    int32_t value = 0;

    memcpy(&value, buffer, 4);

    return value;
}

__uint64_t getULong(const char *buffer) {
    __uint64_t value = 0;

    memcpy(&value, buffer, 8);

    return value;
}

int64_t getLong(const char *buffer) {
    int64_t value = 0;

    memcpy(&value, buffer, 8);

    return value;
}

void sendTableValue(TableValue *tableValue, int socket) {
    writeUShort(tableValue->nodes->length, socket);
    writeULong(tableValue->_uuid, socket);

    for (Element *element = tableValue->nodes->element; element != NULL; element = element->next)
        sendNode(element->value, socket);
}

void sendNode(Node *value, int socket) {
    if (strlen(value->key) > 255)
        return;

    writeUByte((unsigned char) strlen(value->key), socket);

    writeString(value->key, socket);

    writeUByte(value->type, socket);

    if (value->type == STRING) {
        writeUInt((uint32_t) strlen((const char *) value->value), socket);
    }

    switch (value->type) {
        case CHAR:
            writeByte((char) value->value, socket);
            break;

        case UCHAR:
            writeUByte((unsigned char) value->value, socket);
            break;

        case SHORT:
            writeShort((int16_t) value->value, socket);
            break;

        case USHORT:
            writeUShort((uint16_t) value->value, socket);
            break;

        case INT:
            writeInt((int32_t) value->value, socket);
            break;

        case UINT:
            writeUInt((uint32_t) value->value, socket);
            break;

        case LONG:
            writeLong((int64_t) value->value, socket);
            break;

        case ULONG:
            writeULong((uint64_t) value->value, socket);
            break;

        case FLOAT:
            break;

        case DOUBLE:
            break;

        case STRING:
            writeString((char *) value->value, socket);
            break;
        default:
            break;
    }
}
