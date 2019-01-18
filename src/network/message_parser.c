//
// Created by Botan on 03/11/18.
//

#include <message_parser.h>
#include <tree.h>

#include "variable.h"
#include "message_parser.h"
#include "database.h"
#include "comparator.h"

#define SUCCESS 1
#define FAILED 0

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

void (*messages[])(Session *session, __uint16_t size) ={
        login,
        getDatabases, createDatabase, removeDatabase, renameDatabase,
        getTables, createTable, removeTable, renameTable,
        insertValue, findValue
};

void parse(unsigned char id, Session *session) {
    __uint16_t size = readUShort(session->socket);

    println("Reception of [message_id : %d / size : %d]", id, size);

    if (session->connected || id == 0)
        messages[id](session, size);

}

void login(Session *session, __uint16_t size) {
    unsigned char response[2] = {0};

    unsigned char userLength = readUByte(session->socket);

    char *buffer = readString(size, session->socket);

    char _username[userLength + 1];
    strncpy(_username, buffer, userLength);
    _username[userLength] = 0;

    char _password[strlen(buffer) + userLength + 1];
    strcpy(_password, buffer + userLength);
    _password[strlen(buffer) + userLength + 1] = 0;

    if (strcmp(_username, username) == 0 &&
        strcmp(_password, password) == 0) {
        response[1] = SUCCESS;
        session->connected = 1;
    } else {
        response[1] = FAILED;
        session->connected = 0;
    }

    free(buffer);

    send(session->socket, response, 2, 0);
}


void getDatabases(Session *session, __uint16_t size) {
    Container container = listFolders(dataPath);

    writeUByte(1, session->socket);
    writeUShort((__uint16_t) strlen(container.elements), session->socket);
    writeUShort((__uint16_t) container.length, session->socket);

    if (container.length > 0)
        writeString(container.elements, session->socket);

    println("Send database '%s'", container.elements);

    if (strlen(container.elements) > 0)
        free(container.elements);
}

void createDatabase(Session *session, __uint16_t size) {
    unsigned char response[2] = {2};

    char *database = readString(size, session->socket);

    char *path = buildPath(dataPath, database, 0);

    int result, error_code = 0;

    if (isValidName(path)) {
        result = mkdir(path, 0777);
        response[1] = (unsigned char) (!result ? 1 : 0);
        error_code = DATABASE_ALREADY_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    send(session->socket, response, 2, 0);

    if (response[1])
        loadDatabase(database);

    free(path);
    free(database);

    if (!response[1])
        writeUByte((unsigned char) error_code, session->socket);
}

void removeDatabase(Session *session, __uint16_t size) {
    unsigned char response[2] = {3};

    char *database_name = readString(size, session->socket);

    char *path = buildPath(dataPath, database_name, 0);

    int result, error_code = 0;

    if (isValidName(path)) {
        result = removeDirectory(path);
        response[1] = (unsigned char) (!result ? 1 : 0);
        error_code = DATABASE_NOT_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    send(session->socket, response, 2, 0);

    if (response[1]) {
        Database *database = listSearch(globalDatabases, database_name);
        listFree(database->tables);
        free(database->name);
        listDelete(globalDatabases, database_name);
    }

    free(path);
    free(database_name);

    if (!response[1])
        writeUByte((unsigned char) error_code, session->socket);
}

void renameDatabase(Session *session, __uint16_t size) {
    unsigned char response[2] = {4};

    char *database_name = readString(size, session->socket);
    char *new_name = readString(readUShort(session->socket), session->socket);

    char *path = buildPath(dataPath, database_name, 0);
    char *new_path = buildPath(dataPath, new_name, 0);

    println("Rename database '%s' to '%s'", database_name, new_name);

    int result, error_code = 0;

    if (isValidName(path)) {
        result = rename(path, new_path);
        response[1] = (unsigned char) (!result ? 1 : 0);
        error_code = DATABASE_NOT_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    send(session->socket, response, 2, 0);

    if (response[1]) {
        Database *database = listSearch(globalDatabases, database_name);
        database->name = copyString(new_name);
    }

    free(new_name);
    free(new_path);
    free(path);
    free(database_name);


    if (!response[1])
        writeUByte((unsigned char) error_code, session->socket);
}

void getTables(Session *session, __uint16_t size) {
    char *database = readString(size, session->socket);

    Container container = listFolders(buildPath(dataPath, database, 0));

    writeUByte(5, session->socket);
    writeUShort((__uint16_t) strlen(container.elements), session->socket);
    writeUShort((__uint16_t) container.length, session->socket);

    if (container.length > 0)
        writeString(container.elements, session->socket);

    println("Send tables of database[%s] :  '%s'", database, container.elements);

    if (strlen(container.elements) > 0)
        free(container.elements);
}

void createTable(Session *session, __uint16_t size) {
    unsigned char response[2] = {6};

    char *database_name = readString(size, session->socket);
    char *table = readString(readUShort(session->socket), session->socket);

    char *path = buildPath(dataPath, database_name, table, 0);

    int result, error_code = 0;

    if (isValidName(path)) {
        result = mkdir(path, 0777);
        response[1] = (unsigned char) (!result ? 1 : 0);

        if (!response[1])
            error_code = pathExists(path) ? TABLE_ALREADY_EXIST : DATABASE_NOT_EXIST;
        else
            writeIndex(1, buildPath(dataPath, database_name, table, 0));

    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    send(session->socket, response, 2, 0);

    if (response[1])
        loadTable((Database *) listSearch(globalDatabases, database_name), table);

    free(database_name);
    free(table);
    free(path);

    if (!response[1])
        writeUByte((unsigned char) error_code, session->socket);
}

void removeTable(Session *session, __uint16_t size) {
    unsigned char response[2] = {7};

    char *database = readString(size, session->socket);
    char *tableName = readString(readUShort(session->socket), session->socket);

    char *path = buildPath(dataPath, database, tableName, 0);

    int result, error_code = 0;

    if (isValidName(path)) {
        result = removeDirectory(path);
        response[1] = (unsigned char) (!result ? 1 : 0);
        error_code = pathExists(buildPath(dataPath, database, 0)) ? TABLE_NOT_EXIST : DATABASE_NOT_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    if (response[1]) {
        Table *table = findTable(database, tableName);
        listFree(table->binaryTreeList);
        List *values = table->values;
        for (Element *element = values->element; element != NULL;) {
            TableValue *tableValue = element->value;
            listFree(tableValue->nodes);
            element = element->next;
        }
        listFree(table->values);

        listDelete(table->database->tables, table->name);

        free(table->name);
        free(table);
    }

    send(session->socket, response, 2, 0);

    free(database);
    free(tableName);
    free(path);

    if (!response[1])
        writeUByte((unsigned char) error_code, session->socket);
}

void renameTable(Session *session, __uint16_t size) {
    unsigned char response[2] = {8};

    char *database = readString(size, session->socket);
    char *tableName = readString(readUShort(session->socket), session->socket);
    char *newTableName = readString(readUShort(session->socket), session->socket);

    char *path = buildPath(dataPath, database, tableName, 0);
    char *newPath = buildPath(dataPath, database, newTableName, 0);

    int result, error_code = 0;

    if (isValidName(path)) {
        result = rename(path, newPath);
        response[1] = (unsigned char) (!result ? 1 : 0);
        error_code = pathExists(path) ? TABLE_NOT_EXIST : DATABASE_NOT_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    send(session->socket, response, 2, 0);

    if (response[1]) {
        Table *table = findTable(database, tableName);
        table->name = copyString(newTableName);
    }

    free(database);
    free(tableName);
    free(path);
    free(newPath);
    free(newTableName);

    if (!response[1])
        writeUByte((unsigned char) error_code, session->socket);
}

void insertValue(Session *session, __uint16_t size) {
    char *database = readString(size, session->socket);
    char *table = readString(readUShort(session->socket), session->socket);
    unsigned char async = readUByte(session->socket);


    uint16_t nodesLength = readUShort(session->socket);

    TableValue *tableValue = createTableValue();

    for (int i = 0; i < nodesLength; i++) {
        char *key = readString(readUByte(session->socket), session->socket);

        unsigned char type = readUByte(session->socket);

        uint32_t dataLength = PRIMITIVE_SIZE[type];

        if (!dataLength)
            dataLength = readUInt(session->socket);

        char *data = malloc(dataLength);

        recv(session->socket, data, dataLength, 0);

        if (strcmp("_id", key) == 0) { //reserved key
            continue;
        }

        Node *node = malloc(sizeof(Node));
        node->key = key;
        node->type = type;
        node->length = dataLength;
        node->root = tableValue;

        if (type == CHAR || type == UCHAR || type == STRING) {
            if (type == STRING) {
                node->value = malloc(dataLength + 1);
                strcpy(node->value, data);
                ((char *) node->value)[dataLength] = 0;
            } else {
                char c = data[0];
                node->value = INT2VOIDP(c);
            }
        } else {
            node->value = VAR_PARSER[type](data);
        }

        free(data);
        node->comparable = (type == STRING) ? INT2VOIDP(hash(copyString((const char *) node->value))) : node->value;

        listInsert(tableValue->nodes, node);
    }

    insertNodes(database, table, tableValue, !async ? session->socket : -1);
}

void findValue(Session *session, __uint16_t size) {
    char *databaseName = readString(size, session->socket);
    char *tableName = readString(readUShort(session->socket), session->socket);
    char *filter = removeAllSpaces(readString(readUByte(session->socket), session->socket));

    char *key = strtok(filter, "=");
    char *value = strtok(NULL, "=");


    Table *table = findTable(databaseName, tableName);

    List *binaryTreeList = table->binaryTreeList;
    BinaryTree *binaryTree = listSearch(binaryTreeList, key);

    println("Search : %s/%s", key, value);

    if (binaryTree != NULL) {
        searchNode(binaryTree, binaryTree->converter(value), session->socket);
    } else {
        writeUByte(0, session->socket);
    }
}
