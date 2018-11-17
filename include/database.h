//
// Created by Botan on 17/11/18.
//

#ifndef DATI_DATABASE_H
#define DATI_DATABASE_H

#include <stdint.h>
#include "variable.h"

typedef struct node {
    char *key;
    var_type type;
    void *value;
} node;

typedef struct database {
    char *name;
    struct database *next;
} database;

typedef struct table {
    char *name;
    struct table *next;
} table;


void load_databases();

void load_database(void *database_name);

void load_tables(database database);

void load_table(database *database, char *table_name);

void load_values(table *table);

database * get_database(char *name);

#endif //DATI_DATABASE_H
