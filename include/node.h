//
// Created by Botan on 11/11/18.
//

#ifndef DATI_VALUE_H
#define DATI_VALUE_H

typedef struct node {
    char *key;
    unsigned char type;
    void *value;
    char *data;
    void *comparable;
} node;

#endif //DATI_VALUE_H
