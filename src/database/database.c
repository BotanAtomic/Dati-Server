//
// Created by Botan on 17/11/18.
//

#include <malloc.h>
#include <database.h>

#include "database.h"
#include "utils.h"
#include "server.h"
#include "shell.h"
#include "comparator.h"

void load_databases() {
    global_databases = list_create();
    global_databases->length = 0;
    global_databases->comparator = compare_database;


    list *database_folder = get_folders(build_path(data_path, 0));
    foreach(database_folder, (void *) load_database);
    list_free(database_folder);

    foreach(global_databases, (void *) load_tables);
}

void load_database(char *database_name) {
    database *database = malloc(sizeof(database));
    database->name = memstrcpy(database_name);
    database->tables = list_create();
    database->tables->comparator = compare_table;


    list_insert(global_databases, database);
}

void load_tables(database *database) {
    print("Loading {%s} [", database->name);

    list *tables_folder = get_folders(build_path(data_path, database->name, 0));

    element *element = tables_folder->element;

    while (element) {
        load_table(database, element->value);
        element = element->next;
    }

    print("]\n");

}

void load_table(database *database, char *table_name) {
    table *table = malloc(sizeof(table));
    table->name = memstrcpy(table_name);
    table->index = read_index(build_path(data_path, database->name, table_name, 0));

    if (table->index > 0) {
        list_insert(database->tables, table);
        print("%s:%d, ", table_name, table->index);
    } else {
        set_color(RED);
        println("\nCannot load table [%s] : missing index", table_name);
        exit(EXIT_FAILURE);
    }

}

void load_values(table *table) {

}


table *find_table(char *database_name, char *table_name) {
    database *database = list_search(global_databases, database_name);

    if (database)
        return list_search(database->tables, table_name);

    return NULL;
}

unsigned char insert_data(char *database_name, char *table_name, list *nodes) {
    table *table = find_table(database_name, table_name);

    unsigned  long index = read_index(build_path(data_path, database_name, table_name, 0)) + 1;

    println("Insert in index %lu", index);

    if (table) {
        element *node = nodes->element;

        while (node) {

            node = node->next;
        }
        return 1;
    }

    return 0;
}

