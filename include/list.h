//
// Created by Botan on 17/11/18.
//

#ifndef DATI_LIST_H
#define DATI_LIST_H

#include <malloc.h>

typedef char (*comparator)(void *, void *);

typedef struct Element {
    void *value;
    struct Element *next;
} Element;

typedef struct List {
    Element *element;
    __uint16_t length;
    comparator comparator;
} List;

List *createList();

void listInsert(List *, void *);

void listFree(List *);

void foreach(List *, void (*)(void *));

void * listSearch(List *, void *);

void listDelete(List *, void *);

#endif //DATI_LIST_H
