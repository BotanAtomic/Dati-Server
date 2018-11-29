//
// Created by Botan on 18/11/18.
//

#include <stdio.h>
#include <comparator.h>
#include <database.h>

#include "comparator.h"

char compare_char(void *first, void *second) {
    char left = (char) first;
    char right = (char) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compare_uchar(void *first, void *second) {
    unsigned char left = (unsigned char) first;
    unsigned char right = (unsigned char) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compare_short(void *first, void *second) {
    int16_t left = (int16_t) first;
    int16_t right = (int16_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compare_ushort(void *first, void *second) {
    __uint16_t left = (__uint16_t) first;
    __uint16_t right = (__uint16_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compare_int(void *first, void *second) {
    int32_t left = (int32_t) first;
    int32_t right = (int32_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}


char compare_uint(void *first, void *second) {
    __uint32_t left = (__uint32_t) first;
    __uint32_t right = (__uint32_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compare_long(void *first, void *second) {
    int64_t left = (int64_t) first;
    int64_t right = (int64_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compare_ulong(void *first, void *second) {
    __uint64_t left = (__uint64_t) first;
    __uint64_t right = (__uint64_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compare_database(void *first, void *second) {
    database *database = second;
    return (char) strcmp(database->name, (char *) first);
}

char compare_table(void *first, void *second) {
    table *table = second;
    return (char) strcmp(table->name, (char *) first);
}

char compare_string(void *first, void *second) {
    return (char) strcmp((char *) first, (char *) second);
}
