//
// Created by Botan on 03/11/18.
//

#include "message_parser.h"

#define SUCCESS 1
#define FAILED 0


void (*messages[])(struct session *session, __uint16_t size) ={
        login,
        get_databases, create_database, remove_database, rename_database,
        get_table, create_table, remove_table, rename_table
};


void parse(unsigned char id, struct session *session) {
    __uint16_t size = read_ushort(session->socket);

    println("Reception of [message_id : %d / size : %d]", id, size);

    if (session->connected || id == 0) {
        messages[id](session, size);
    }
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
    single_container container = list_folders(data_path);

    write_ubyte(1, session->socket);
    write_ushort((__uint16_t) strlen(container.elements), session->socket);
    write_ushort((__uint16_t) container.count, session->socket);

    if (container.count > 0)
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

    single_container container = list_folders(build_path(2, data_path, database));

    write_ubyte(5, session->socket);
    write_ushort((__uint16_t) strlen(container.elements), session->socket);
    write_ushort((__uint16_t) container.count, session->socket);

    if (strlen(container.elements) > 0)
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
        create_index(build_path(4, data_path, database, table, "index"));
        if (response[1] == 0)
            error_code = path_exists(path) ? TABLE_ALREADY_EXIST : DATABASE_NOT_EXIST;
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

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte((unsigned char) error_code, session->socket);
}
