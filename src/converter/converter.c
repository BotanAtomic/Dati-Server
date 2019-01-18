//
// Created by Botan on 16/12/18.
//

#include <stdlib.h>

#include "converter.h"
#include "utils.h"

void *convertChar(char * string) {
    char c = (char) atoi(string);
    return (void*) c;
}

void *convertUChar(char * string) {
    unsigned char c = (unsigned char) atoi(string);
    return (void*) c;
}

void *convertShort(char * string) {
    int16_t value = (int16_t) strtol(string, NULL, 10);
    return (void*) value;
}

void *convertUShort(char * string) {
    __uint16_t value = (__uint16_t) strtol(string, NULL, 10);
    return (void*) value;
}

void *convertInt(char * string) {
    int32_t value = (int32_t) strtol(string, NULL, 10);
    return (void*) value;
}

void *convertUInt(char * string) {
    __uint32_t value = (__uint32_t) strtol(string, NULL, 10);
    return (void*) value;
}

void *convertLong(char * string) {
    int64_t value = (int64_t) strtol(string, NULL, 10);
    return (void*) value;
}

void *convertULong(char * string) {
    __uint64_t value = (__uint64_t) strtol(string, NULL, 10);
    return (void*) value;
}

void *convertString(char * string) {
    return (void*) hash(string);
}