//
// Created by Botan on 03/11/18.
//

#include "variable.h"
#include "message_parser.h"

#define SUCCESS 1
#define FAILED 0


void (*messages[])(struct session *session, __uint16_t size) ={
        login,
        get_databases, create_database, remove_database, rename_database,
        get_table, create_table, remove_table, rename_table,
        insert_value
};


void parse(unsigned char id, struct session *session) {
    __uint16_t size = read_ushort(session->socket);

    println("Reception of [message_id : %d / size : %d]", id, size);

    if (session->connected || id == 0)
        messages[id](session, size);

}

void login(struct session *session, __uint16_t size) {
    unsigned char response[2] = {0};

    unsigned char user_length = read_ubyte(session->socket);

    char *buffer = read_string(size, session->socket);

    if (strncmp(buffer, username, user_length) == 0 &&
        strncmp(buffer + user_length, password, (size - user_length)) == 0) {
        response[1] = SUCCESS;
        session->connected = 1;
    } else {
        response[1] = FAILED;
        session->connected = 0;
    }

    free(buffer);

    send(session->socket, response, 2, 0);
}


void get_databases(struct session *session, __uint16_t size) {
    container container = list_folders(data_path);

    write_ubyte(1, session->socket);
    write_ushort((__uint16_t) strlen(container.elements), session->socket);
    write_ushort((__uint16_t) container.length, session->socket);

    if (container.length > 0)
        write_string(container.elements, session->socket);

    println("Send database '%s'", container.elements);

    if (strlen(container.elements) > 0)
        free(container.elements);
}

void create_database(struct session *session, __uint16_t size) {
    unsigned char response[2] = {2};

    char *database = read_string(size, session->socket);

    char *path = build_path(2, data_path, database);

    int result, error_code = 0;

    if (valid_name(path)) {
        result = mkdir(path, 0777);
        response[1] = (unsigned char) (result == 0 ? 1 : 0);
        error_code = DATABASE_ALREADY_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    free(path);
    free(database);

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte((unsigned char) error_code, session->socket);
}

void remove_database(struct session *session, __uint16_t size) {
    unsigned char response[2] = {3};

    char *database = read_string(size, session->socket);

    char *path = build_path(2, data_path, database);

    int result, error_code = 0;

    if (valid_name(path)) {
        result = remove_directory(path);
        response[1] = (unsigned char) (result == 0 ? 1 : 0);
        error_code = DATABASE_NOT_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    free(path);
    free(database);

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte((unsigned char) error_code, session->socket);
}

void rename_database(struct session *session, __uint16_t size) {
    unsigned char response[2] = {4};

    char *database = read_string(size, session->socket);
    char *new_name = read_string(read_ushort(session->socket), session->socket);

    char *path = build_path(2, data_path, database);
    char *new_path = build_path(2, data_path, new_name);

    println("Rename database '%s' to '%s'", database, new_name);

    int result, error_code = 0;

    if (valid_name(path)) {
        result = rename(path, new_path);
        response[1] = (unsigned char) (result == 0 ? 1 : 0);
        error_code = DATABASE_NOT_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    free(new_name);
    free(new_path);
    free(path);
    free(database);

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte((unsigned char) error_code, session->socket);
}

void get_table(struct session *session, __uint16_t size) {
    char *database = read_string(size, session->socket);

    container container = list_folders(build_path(2, data_path, database));

    write_ubyte(5, session->socket);
    write_ushort((__uint16_t) strlen(container.elements), session->socket);
    write_ushort((__uint16_t) container.length, session->socket);

    if (container.length > 0)
        write_string(container.elements, session->socket);

    println("Send tables of database[%s] :  '%s'", database, container.elements);

    if (strlen(container.elements) > 0)
        free(container.elements);
}

void create_table(struct session *session, __uint16_t size) {
    unsigned char response[2] = {6};

    char *database = read_string(size, session->socket);
    char *table = read_string(read_ushort(session->socket), session->socket);

    char *path = build_path(3, data_path, database, table);

    int result, error_code = 0;

    if (valid_name(path)) {
        result = mkdir(path, 0777);
        response[1] = (unsigned char) (result == 0 ? 1 : 0);

        if (response[1] == 0)
            error_code = path_exists(path) ? TABLE_ALREADY_EXIST : DATABASE_NOT_EXIST;
        else
            write_index(1, build_path(4, data_path, database, table, "index"));

    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    free(database);
    free(table);
    free(path);

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte((unsigned char) error_code, session->socket);
}

void remove_table(struct session *session, __uint16_t size) {
    unsigned char response[2] = {7};

    char *database = read_string(size, session->socket);
    char *table = read_string(read_ushort(session->socket), session->socket);

    char *path = build_path(3, data_path, database, table);

    int result, error_code = 0;

    if (valid_name(path)) {
        result = remove_directory(path);
        response[1] = (unsigned char) (result == 0 ? 1 : 0);
        error_code = path_exists(build_path(2, data_path, database)) ? TABLE_NOT_EXIST : DATABASE_NOT_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    free(database);
    free(table);
    free(path);

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte((unsigned char) error_code, session->socket);
}

void rename_table(struct session *session, __uint16_t size) {
    unsigned char response[2] = {8};

    char *database = read_string(size, session->socket);
    char *table = read_string(read_ushort(session->socket), session->socket);
    char *new_table_name = read_string(read_ushort(session->socket), session->socket);

    char *path = build_path(3, data_path, database, table);
    char *new_path = build_path(3, data_path, database, new_table_name);

    int result, error_code = 0;

    if (valid_name(path)) {
        result = rename(path, new_path);
        response[1] = (unsigned char) (result == 0 ? 1 : 0);
        error_code = path_exists(path) ? TABLE_NOT_EXIST : DATABASE_NOT_EXIST;
    } else {
        response[1] = 0;
        error_code = UNAUTHORIZED_NAME;
    }

    free(database);
    free(table);
    free(path);
    free(new_path);
    free(new_table_name);

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte((unsigned char) error_code, session->socket);
}

void insert_value(struct session *session, __uint16_t size) {
    unsigned char response[2] = {9};

    char *database = read_string(size, session->socket);
    char *table = read_string(read_ushort(session->socket), session->socket);

    uint16_t data_size = read_ushort(session->socket);

    println("Database : %s / Table : %s / Insert length : %d", database, table, data_size);

    for (int i = 0; i < data_size; i++) {
        char *key = read_string(read_ushort(session->socket), session->socket);
        unsigned char type = read_ubyte(session->socket);


        uint32_t data_length = PRIMITIVE_SIZE[type];

        if (data_length == 0)
            data_length = read_uint(session->socket);

        char *data = malloc(data_length);

        recv(session->socket, data, data_length, 0);


        if(strcmp("_id", key) == 0) { //reserved key
            continue;
        }


        printf("Key %s/ Type %d", key, type);

        switch (type) {
            case CHAR:
                printf(" / Value :%d|%c \n", data[0], data[0]);
                break;

            case UCHAR:
                printf(" / Value :%d|%c \n", (unsigned char) data[0], (unsigned char) data[0]);
                break;

            case SHORT:
                printf(" / Value :%d \n", get_short(data));
                break;

            case USHORT:
                printf(" / Value :%u \n", get_ushort(data));
                break;

            case INT:
                printf(" / Value :%d \n", get_int(data));
                break;

            case UINT:
                printf(" / Value :%u \n", get_uint(data));
                break;

            case LONG:
                printf(" / Value :%ld \n", get_long(data));
                break;

            case ULONG:
                printf(" / Value :%lu \n", get_ulong(data));
                break;

            case FLOAT: {
                float float_value = strtof(data, NULL);
                printf(" / Value :%f|%s$ \n", float_value, data);
                break;
            }

            case DOUBLE: {
                printf("Receive double : %s", data);
                double double_value = strtod(data, NULL);
                printf(" / Value :%lf|%s$ \n", double_value, data);
                break;
            }

            case STRING:
                printf(" / Value :%s \n", data);
                break;

            default:
                //send error code
                break;
        }
    }

}
