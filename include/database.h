//
// Created by Botan on 17/11/18.
//

#ifndef DATI_DATABASE_H
#define DATI_DATABASE_H

#include <stdint.h>
#include "variable.h"
#include "list.h"
#include "tree.h"

list *global_databases;

typedef struct database {
    char *name;
    list *tables;
} database;

typedef struct table {
    uint64_t index;
    char* path;
    char *name;
    list search_tree;
} table;


void load_databases();

void load_database(char *database_name);

void load_tables(database *database);

void load_table(database *database, char *table_name);

void load_values(table *table);

unsigned char insert_data(char *database_name, char *table_name, list * nodes);

table *find_table(char * database, char *name);

#endif //DATI_DATABASE_H
