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
#include "../shell/shell.h"
#include "error_code.c"
#include "../utils/utils.h"

void parse(unsigned char id, struct session *session);

void login(struct session *session, __uint16_t size);

void get_databases(struct session *session, __uint16_t size);

void create_database(struct session *session, __uint16_t size);

void remove_database(struct session *session, __uint16_t size);

void rename_database(struct session *session, __uint16_t size);

void get_table(struct session *session, __uint16_t size);

void create_table(struct session *session, __uint16_t size);

void remove_table(struct session *session, __uint16_t size);

void rename_table(struct session *session, __uint16_t size);

#endif //DATI_MESSAGE_HANDLER_H
