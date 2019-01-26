//
// Created by Botan on 18/11/18.
//

#include "comparator.h"

char compareChar(void *first, void *second) {
    char left = (char) first;
    char right = (char) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compareUChar(void *first, void *second) {
    unsigned char left = (unsigned char) first;
    unsigned char right = (unsigned char) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compareShort(void *first, void *second) {
    int16_t left = (int16_t) first;
    int16_t right = (int16_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compareUShort(void *first, void *second) {
    __uint16_t left = (__uint16_t) first;
    __uint16_t right = (__uint16_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compareInt(void *first, void *second) {
    int32_t left = (int32_t) first;
    int32_t right = (int32_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}


char compareUInt(void *first, void *second) {
    __uint32_t left = (__uint32_t) first;
    __uint32_t right = (__uint32_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compareLong(void *first, void *second) {
    int64_t left = (int64_t) first;
    int64_t right = (int64_t) second;


    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compareULong(void *first, void *second) {
    __uint64_t left = (__uint64_t) first;
    __uint64_t right = (__uint64_t) second;

    return (char) (left > right ? 1 : left < right ? -1 : 0);
}

char compareDatabase(void *first, void *second) {
    Database *database = second;
    return (char) strcmp(database->name, (char *) first);
}

char compareTable(void *first, void *second) {
    Table *table = second;
    return (char) strcmp(table->name, (char *) first);
}

char compareTableValue(void *first, void *second) {
    TableValue *left = first;
    TableValue *right = second;

    return left->_uuid == right->_uuid;
}

char compareString(void *first, void *second) {
    return (char) strcmp((char *) first, (char *) second);
}

char compareBinaryTree(void *first, void *second) {
    BinaryTree *binaryTree = (BinaryTree *) second;
    return (char) strcmp(binaryTree->key, (char *) first);
}
