//
// Created by Botan on 26/10/18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "json-c/json.h"
#include "../utils/utils.h"
#include "server.h"
#include "message_parser.h"

#define CONFIGURATION_FILE "configuration.json"
#define BACKLOG 20

int port, max_connection;

char *data;

void load_configuration() {
    printf("Loading file '%s' : ", CONFIGURATION_FILE);

    char *content = NULL;
    long int size = 0;
    FILE *file;
    struct json_object *json_object;


    file = fopen(CONFIGURATION_FILE, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        rewind(file);

        content = (char *) malloc(sizeof(char) * size);
        fread(content, 1, (size_t) size, file);
        json_object = json_tokener_parse(content);

        port = (int) strtol(get_json_string_val(json_object, "network-port"), NULL, 10);
        max_connection = (int) strtol(get_json_string_val(json_object, "max-connection"), NULL, 10);

        data = get_json_string_val(json_object, "data-path");

        printf("OK\n");
        fclose(file);
    } else {
        printf("FAILED\n");
        exit(EXIT_FAILURE);
    }
}

void bind_server() {
    setbuf(stdout, NULL);

    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        printf("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    int iSetOption = 1;

    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, (char *) &iSetOption,
               sizeof(iSetOption));

    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("Cannot bind server on port %d", port);
        return;
    }
    printf("Server started on port %d\n", port);

    listen(socket_desc, max_connection);

    c = sizeof(struct sockaddr_in);

    pthread_t thread_id;

    while ((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c))) {
        puts("Connection accepted \n");

        pthread_create(&thread_id, NULL, connection_handler, (void *) &client_sock);
        pthread_join(thread_id, NULL);
    }

}

void *connection_handler(void *socket_desc) {
    int sock = *(int *) socket_desc;
    int read_size;
    unsigned char *message[1];

    session session;

    session.connected = 0;
    session.socket = sock;
    while ((read_size = (int) recv(sock, message, 1, 0)) > 0) {
        parse((unsigned char) message[0], &session);
    }

    if (read_size == 0) {
        puts("Client disconnected \n");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("Cannot read message \n");
    }

    return 0;
}