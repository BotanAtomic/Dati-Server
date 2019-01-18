//
// Created by Botan on 28/10/18.
//

#ifndef DATI_BUFFER_H
#define DATI_BUFFER_H

#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "database.h"

static const unsigned char PRIMITIVE_SIZE[] = {1, 1, 2, 2, 4, 4, 8, 8, 0, 0, 0};

void writeByte(char, int);

void writeUByte(unsigned char, int);

void writeShort(int16_t , int);

void writeUShort(__uint16_t, int);

void writeInt(int32_t , int);

void writeUInt(uint32_t , int);

void writeLong(int64_t, int);

void writeULong(__uint64_t, int);

void writeString(char *, int);

char *readString(size_t, int);

__uint16_t readUShort(int);

__uint32_t readUInt(int);

unsigned char readUByte(int);

__uint64_t readULong(int);

int16_t getShort(const char *);

__uint16_t getUShort(const char *);

int32_t getInt(const char *);

__uint32_t getUInt(const char *);

int64_t getLong(const char *);

__uint64_t getULong(const char *);

void sendTableValue(TableValue*, int);

void sendNode(Node *, int);

static void *(*VAR_PARSER[])(const char *) ={
        NULL, NULL, (void *(*)(const char *)) getShort, (void *(*)(const char *)) getShort,
        (void *(*)(const char *)) getInt, (void *(*)(const char *)) getUInt,
        (void *(*)(const char *)) getLong, (void *(*)(const char *)) getULong, NULL, NULL, NULL
};

#endif //DATI_BUFFER_H
