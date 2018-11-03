//
// Created by Botan on 27/10/18.
//


void parse(unsigned char id, session *session);

void login(session *session, __uint16_t size);

void get_databases(session *session, __uint16_t size);

void create_database(session *session, __uint16_t size);