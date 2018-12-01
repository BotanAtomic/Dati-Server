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
    table->path = build_path(data_path, database->name, table_name, 0);
    table->index = read_index(table->path);

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
    FILE *file_ptr = fopen(concat_string(memstrcpy(table->path), "2.bin"), "r");

    if (file_ptr == NULL) {
        printf("%s\n", build_path(memstrcpy(table->path), "2.bin", 0));
        return;
    }


    fseek(file_ptr, 11, 0);
    char * key = malloc(6);

    fread(key, 6, 1, file_ptr);

    printf("Key : %s", key);

}


table *find_table(char *database_name, char *table_name) {
    database *database = list_search(global_databases, database_name);

    if (database)
        return list_search(database->tables, table_name);

    return NULL;
}

unsigned char insert_data(char *database_name, char *table_name, list *nodes) {
    table *table = find_table(database_name, table_name);

    char *table_path = build_path(data_path, database_name, table_name, 0);

    unsigned long index = read_index(table_path);

    char file_name[sizeof(unsigned long) * 8 + 5]; //5 = 1 + 4(.bin)

    sprintf(file_name, "%lu.bin", index);

    FILE *file_ptr;
    file_ptr = fopen(concat_string(table_path, file_name), "w+b");

    if (file_ptr == NULL) {
        println("Cannot create binary file[%s] in path: %s", file_name, table_path);
        return 0;
    }

    printf("Write in file %s\n", file_name);

    fwrite(&nodes->length, 2, 1, file_ptr);

    if (table) {
        element *current_node = nodes->element;

        while (current_node) {
            node *node = current_node->value;

            if (node->type == STRING) {
                printf("String:%s\n", ((char *) node->value));
            } else if (node->type == CHAR) {
                printf("Char:%c\n", (char) node->value);
            } else if (node->type == SHORT) {
                printf("UInt:%d\n", (int16_t) node->value);
            }

            uint16_t key_length = (uint16_t) strlen(node->key);
            fwrite(&key_length, 2, 1, file_ptr);
            fwrite(&node->key, key_length, 1, file_ptr);
            fwrite(&node->type, 1, 1, file_ptr);
            char *data;
            memcpy(&data, node->type == STRING ? node->value : &node->value, node->length);
            fwrite(&data, node->length, 1, file_ptr);
            current_node = current_node->next;


        }
    }

    write_index(++index, table_path);

    fflush(file_ptr);
    fclose(file_ptr);

    return (unsigned char) (table ? 1 : 0);
}

