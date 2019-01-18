//
// Created by Botan on 03/11/18.
//

#ifndef DATI_MESSAGE_HANDLER_H
#define DATI_MESSAGE_HANDLER_H

#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "buffer.h"
#include "shell.h"
#include "error_code.h"
#include "utils.h"

void parse(unsigned char id, struct Session *);

void login(Session *, __uint16_t);

void getDatabases(Session *, __uint16_t);

void createDatabase(Session *, __uint16_t);

void removeDatabase(Session *, __uint16_t);

void renameDatabase(Session *, __uint16_t);

void getTables(Session *, __uint16_t);

void createTable(Session *, __uint16_t);

void removeTable(Session *, __uint16_t);

void renameTable(Session *, __uint16_t);

void insertValue(Session *, __uint16_t);

void findValue(Session *, __uint16_t);

#endif //DATI_MESSAGE_HANDLER_H
