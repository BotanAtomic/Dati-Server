//
// Created by Botan on 11/11/18.
//

#ifndef DATI_VALUE_H
#define DATI_VALUE_H

#include <sys/types.h>

typedef struct node {
    char *key;
    unsigned char type;
    void *value;
    void *comparable;
    __uint32_t length;
} node;

#endif //DATI_VALUE_H
