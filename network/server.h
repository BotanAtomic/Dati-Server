//
// Created by Botan on 26/10/18.
//

#ifndef DATI_SERVER_H
#define DATI_SERVER_H

#endif //DATI_SERVER_H

typedef struct session {
    unsigned char connected;
    int socket;
} session;

int port, max_connection;

char *data_path;

char* username, *password;

void *connection_handler(void *socket_desc);

void load_configuration();

void bind_server();

