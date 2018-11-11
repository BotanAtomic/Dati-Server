//
// Created by Botan on 26/10/18.
//

#include "server.h"
#include "message_parser.h"


#define CONFIGURATION_FILE "configuration.json"


void load_configuration() {
    print("Loading file '%s' : ", CONFIGURATION_FILE);

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

        port = (int) strtol(get_json_value(json_object, "network-port"), NULL, 10);
        max_connection = (int) strtol(get_json_value(json_object, "max-connection"), NULL, 10);

        data_path = get_json_value(json_object, "databases-path");

        username = get_json_value(json_object, "username");
        password = get_json_value(json_object, "password");

        println("SUCCESS");
        fclose(file);
    } else {
        println("FAILED");
        exit(EXIT_FAILURE);
    }
}

void bind_server() {
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        print("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons((uint16_t) port);

    int set = 1;

    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, (char *) &set,
               sizeof(set));

    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        println("Cannot bind server on port %d", port);
        return;
    }
    println("Server started on port %d", port);

    listen(socket_desc, max_connection);

    c = sizeof(struct sockaddr_in);

    pthread_t thread;

    while ((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c))) {
        println("Connection accepted");

        pthread_create(&thread, NULL, connection_handler, (void *) &client_sock);
        pthread_join(thread, NULL);
    }

}

void *connection_handler(void *socket_desc) {
    int sock = *(int *) socket_desc;
    int read_size;
    unsigned char *message[1];

    struct session session;

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