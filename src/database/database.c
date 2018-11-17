//
// Created by Botan on 17/11/18.
//

#include <malloc.h>
#include "database.h"
#include "utils.h"
#include "server.h"
#include "shell.h"

static list *global_databases;

void load_databases() {
    global_databases = list_create();
    global_databases->length = 0;

    list *database_folder = get_folders(build_path(1, data_path));
    foreach(database_folder, load_database);
    list_free(database_folder);
}

void load_database(void *database_name) {
    database *database = malloc(sizeof(database));
    database->name = (char*) database_name;

    list_insert(global_databases, database);
    load_tables(*database);
}

void load_tables(database database) {
    println("Load tables of %s", build_path(2, data_path, database.name));

    list *tables_folder = get_folders(build_path(2, data_path, database.name));

    element *element = tables_folder->element;

    while (element) {
        println("Table : %s", element->value);
        element = element->next;
    }

}

void load_table(database *database, char *table_name) {

}

void load_values(table *table) {

}

database *get_database(char *name) {
    element *element = global_databases->element;


    while (element) {
        if (strcmp(((database *) element->value)->name, name) == 0)
            return element->value;

        element = element->next;
    }

    return NULL;
}

