//
// Created by Botan on 26/10/18.
//

#include "server.h"
#include "message_parser.h"


#define CONFIGURATION_FILE "configuration.json"

int client_identity = 0;

void load_configuration() {
    set_color(BLUE);
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

        set_color(BLUE);
        println("SUCCESS");
        fclose(file);
    } else {
        set_color(RED);
        println("FAILED");
        exit(EXIT_FAILURE);
    }

}

void bind_server() {
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        set_color(RED);
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
        set_color(RED);
        println("Cannot bind server on port %d", port);
        return;
    }

    set_color(GREEN);
    println("Server started on port %d\n", port);

    listen(socket_desc, max_connection);

    c = sizeof(struct sockaddr_in);

    pthread_t thread;

    while ((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c))) {
        pthread_create(&thread, NULL, connection_handler, (void *) &client_sock);
        pthread_join(thread, NULL);
    }

}

void *connection_handler(void *socket_desc) {
    session session;

    session.id = client_identity++;

    set_color(MAGENTA);
    println("Session[%d] : connection accepted", session.id);

    int sock = *(int *) socket_desc;
    int read_size;
    unsigned char message[1];


    session.connected = 0;
    session.socket = sock;

    while ((read_size = (int) recv(sock, message, 1, 0)) > 0) {
        parse(message[0], &session);
    }

    if (read_size == 0) {
        set_color(MAGENTA);
        println("Session[%d] : disconnected\n", session.id);
    } else if (read_size == -1) {
        set_color(MAGENTA);
        println("Session[%d] : connection closed (cannot read message)\n", session.id);
    }

    return 0;
}