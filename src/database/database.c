//
// Created by Botan on 17/11/18.
//

#include <malloc.h>
#include <error_code.h>
#include <comparator.h>
#include <converter.h>


#include "tree.h"
#include "byte_buffer.h"
#include "database.h"
#include "utils.h"
#include "server.h"
#include "shell.h"
#include "buffer.h"

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

void loadDatabases() {
    mkdir(dataPath, 0777);

    globalDatabases = createList();
    globalDatabases->length = 0;
    globalDatabases->comparator = compareDatabase;

    List *databaseFolder = getFolders(buildPath(dataPath, 0));
    foreach(databaseFolder, (void *) loadDatabase);
    listFree(databaseFolder);

    foreach(globalDatabases, (void *) loadTables);
    foreach(globalDatabases, (void *) loadValues);

    print("\n");
}

void loadDatabase(char *databaseName) {
    Database *database = malloc(sizeof(Database));
    database->name = copyString(databaseName);
    database->tables = createList();
    database->tables->comparator = compareTable;

    listInsert(globalDatabases, database);
}

void loadTables(Database *database) {
    List *tablesFolder = getFolders(buildPath(dataPath, database->name, 0));

    for (Element *element = tablesFolder->element; element; element = element->next) {
        loadTable(database, element->value);
    }

    free(tablesFolder);
}

void loadTable(Database *database, char *tableName) {
    Table *table = malloc(sizeof(Table));
    table->name = copyString(tableName);
    table->database = database;
    table->index = readIndex(buildPath(dataPath, database->name, tableName, 0));
    table->values = createList();
    table->binaryTreeList = createList();
    table->binaryTreeList->comparator = compareBinaryTree;

    if (table->index > 0) {
        listInsert(database->tables, table);
    } else {
        setColor(RED);
        println("\nCannot load table [%s] : missing index", tableName);
        exit(EXIT_FAILURE);
    }

}

void loadValues(Database *database) {
    for (Element *element = database->tables->element; element != NULL; element = element->next) {
        Table *table = element->value;

        char *path = buildPath(dataPath, database->name, table->name, 0);
        print("Loading ");
        setColor(CYAN);
        print("{%s/%s}", database->name, table->name);

        printSpaces(30 - (strlen(database->name) + strlen(table->name)));

        setColor(YELLOW);
        unsigned long loaded = 0;
        for (uint64_t i = 1; i < table->index; i++) {
            char *filePath = malloc(strlen(path) + 20 + 1 + 3); //path + max ulong + . + bin
            sprintf(filePath, "%s%lu.bin", path, i);

            FILE *filePointer = fopen(filePath, "rb");

            if (!filePointer)
                continue;

            fseek(filePointer, 0L, SEEK_END);
            size_t size = (size_t) ftell(filePointer);
            fseek(filePointer, 0L, SEEK_SET);

            char *fileData = malloc(size);
            size_t readSize = fread(fileData, size, 1, filePointer);
            fclose(filePointer);

            if (readSize != 1)
                continue;

            ByteBuffer *byteBuffer = newByteBuffer(fileData, size);

            uint16_t nodeLength;
            byteBufferRead(byteBuffer, &nodeLength, 2);

            TableValue *tableValue = createTableValue();
            tableValue->table = table;
            tableValue->_uuid = i;
            List *nodes = createList();

            tableValue->nodes = nodes;

            Node *idNode = malloc(sizeof(Node));
            idNode->key = "uuid";
            idNode->type = ULONG;
            idNode->root = tableValue;
            idNode->value = INT2VOIDP(tableValue->_uuid);
            idNode->comparable = idNode->value;
            registerNode(idNode);
            listInsert(nodes, idNode);

            for (uint16_t j = 0; j < nodeLength; j++) {
                Node *node = malloc(sizeof(Node));
                unsigned char keyLength = 0;
                byteBufferRead(byteBuffer, &keyLength, 1);

                node->key = calloc(1, keyLength);
                byteBufferRead(byteBuffer, node->key, keyLength);

                byteBufferRead(byteBuffer, &node->type, 1);
                byteBufferRead(byteBuffer, &node->length, 4);

                char *data = calloc(1, node->length);
                byteBufferRead(byteBuffer, data, node->length);

                if (node->type == CHAR || node->type == UCHAR || node->type == STRING) {
                    if (node->type == STRING)
                        node->value = copyString(data);
                    else {
                        node->value = INT2VOIDP(data[0]);
                    }
                } else {
                    node->value = VAR_PARSER[node->type](data);
                }

                node->comparable = (node->type == STRING) ?
                                   INT2VOIDP(hash((const char *) node->value)) : node->value;

                node->root = tableValue;

                free(data);
                listInsert(nodes, node);
                registerNode(node);
            }
            freeByteBuffer(byteBuffer);
            loaded++;
            listInsert(table->values, tableValue);
        }
        printf("[ %lu values / index:%lu ]\n", loaded, table->index);
        resetColor();
    }

}


Table *findTable(char *databaseName, char *tableName) {
    Database *database = listSearch(globalDatabases, databaseName);

    if (database)
        return listSearch(database->tables, tableName);

    return NULL;
}

void writeTableValue(TableValue *tableValue) {
    Table *table = tableValue->table;
    char *path = buildPath(dataPath, table->database->name, table->name, 0);

    char fileName[sizeof(unsigned long) * 8 + 5]; //5 = 1 + 4(.bin)

    sprintf(fileName, "%lu.bin", tableValue->_uuid);

    FILE *filePointer;
    filePointer = fopen(concatString(path, fileName), "wb");

    if (filePointer == NULL) {
        println("Cannot create binary file[%s] in path: %s", fileName, path);
        return;
    }

    List *nodes = tableValue->nodes;


    fwrite(&nodes->length, 2, 1, filePointer);

    for (Element *currentNode = nodes->element; currentNode; currentNode = currentNode->next) {
        Node *node = currentNode->value;

        unsigned char keyLength = (unsigned char) (strlen(node->key) + 1);
        fwrite(&keyLength, 1, 1, filePointer);
        fwrite(node->key, keyLength, 1, filePointer);
        fwrite(&node->type, 1, 1, filePointer);
        fwrite(&node->length, 4, 1, filePointer);
        fwrite(node->type == STRING ? node->value : &node->value, node->length, 1, filePointer);
    }


    fflush(filePointer);
    fclose(filePointer);

    free(path);
}

TableValue *createTableValue() {
    TableValue *tableValue = malloc(sizeof(TableValue));
    tableValue->nodes = createList();
    tableValue->_uuid = 0;
    return tableValue;
}

void registerNode(Node *node) {
    char *key = node->key;
    List *binaryTreeList = node->root->table->binaryTreeList;
    BinaryTree *binaryTree = listSearch(binaryTreeList, key);

    if (binaryTree == NULL) {
        binaryTree = createBinaryTree(COMPARATORS[node->type], CONVERTERS[node->type], key);
        listInsert(binaryTreeList, binaryTree);
    }

    insertTreeNode(binaryTree, node);
}

