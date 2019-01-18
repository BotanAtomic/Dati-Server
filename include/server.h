//
// Created by Botan on 26/10/18.
//

#ifndef DATI_SERVER_H
#define DATI_SERVER_H


#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "shell.h"
#include "utils.h"

typedef struct Session {
    unsigned char connected;
    int socket;
    int id;
} Session;

int port, maxConnection;

char *dataPath;

char *username, *password;

void *connectionHandler(void *);

void loadConfiguration();

void bindServer();

#endif //DATI_SERVER_H


