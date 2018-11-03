//
// Created by Botan on 03/11/18.
//

#include "message_parser.h"

#define SUCCESS 1
#define FAILED 0

void (*messages[])(struct session *session, __uint16_t size) ={
        login, get_databases, create_database
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

    send(session->socket, response, 2, 0);
}


void get_databases(struct session *session, __uint16_t size) {
    DIR *dir = opendir("database");
    char *databases = "";

    int count = 0;
    if (dir != NULL) {

        struct dirent *path_entries;
        while ((path_entries = readdir(dir)) != NULL) {
            if (path_entries->d_type == DT_DIR && strcmp(path_entries->d_name, ".") != 0 &&
                strcmp(path_entries->d_name, "..") != 0) {

                if (strlen(databases) > 0)
                    concat_string(&databases, "@");

                concat_string(&databases, path_entries->d_name);
                count++;
            }
        }
    }

    closedir(dir);

    write_ubyte(1, session->socket);
    write_ushort((__uint16_t) strlen(databases), session->socket);
    write_ushort((__uint16_t) count, session->socket);

    if (strlen(databases) > 0)
        write_string(databases, session->socket);

    free(databases);
}

void create_database(struct session *session, __uint16_t size) {
    unsigned char response[2] = {2};

    char *database = read_string(size, session->socket);

    __uint16_t path_size = (__uint16_t) (strlen(data_path) + strlen(database) + 2);

    char *path = malloc(path_size);
    snprintf(path, path_size, "%s/%s", data_path, database);

    int error_code;
    int result;

    char exclude[] = {' ', '@', '\0'};

    if (!contains(path, exclude) && size < 256) {
        result = mkdir(path, 0777);
        response[1] = (unsigned char) (result == 0 ? 1 : 0);
        error_code = DATABASE_ALREADY_EXIST;
    } else {
        response[1] = 0;
        error_code = DATABASE_UNAUTORIZED_NAME;
    }

    free(path);

    send(session->socket, response, 2, 0);

    if (response[1] == 0)
        write_ubyte((unsigned char) error_code, session->socket);
}

