//
// Created by Botan on 17/11/18.
//

#ifndef DATI_DATABASE_H
#define DATI_DATABASE_H

#include <stdint.h>
#include "variable.h"
#include "list.h"

List *globalDatabases;

typedef struct Database {
    char *name;
    List *tables;
} Database;

typedef struct Table {
    uint64_t index;
    Database *database;
    char *name;
    List *binaryTreeList;
    List *values;
} Table;

typedef struct TableValue {
    List *nodes;
    __uint64_t _uuid;
    Table* table;
} TableValue;

typedef struct Node {
    char *key;
    void *value;
    void *comparable;
    unsigned char type;
    __uint32_t length;
    TableValue *root;
} Node;

TableValue *createTableValue();

void loadDatabases();

void loadDatabase(char *);

void loadTables(Database *);

void loadTable(Database *, char *);

void loadValues(Database *);

void insertNodes(char *, char *, TableValue *, int);

Table *findTable(char *, char *);

void registerNode(Node* node);

#endif //DATI_DATABASE_H
