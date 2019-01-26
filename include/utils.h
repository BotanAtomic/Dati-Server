//
// Created by Botan on 26/10/18.
//


#ifndef DATI_UTILS_H
#define DATI_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <stdarg.h>

#include "json-c/json.h"
#include "container.h"
#include "list.h"
#include <sys/time.h>

char *concatString(char *, char *);

char *copyString(const char *);

char *getJsonValue(json_object *, const char *);

unsigned char contains(char *, char[]);

int removeDirectory(char *);

unsigned char pathExists(char *);

unsigned char isValidName(char *);

char *buildPath(char *, ...);

Container listFolders(char *);

List *getFolders(char *);

void writeIndex(unsigned long, char *);

unsigned long readIndex(char *);

unsigned hash(const char *);

char *removeAllSpaces(char *);

long long currentTimestamp();

#endif //DATI_UTILS_H
