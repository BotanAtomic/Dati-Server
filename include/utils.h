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


void concat_string(char **str, const char *str2);

char *memstrcpy(const char *in);

char *get_json_value(json_object *json_obj, const char *key);

unsigned char contains(char *str, char c[]);

int remove_directory(char *path);

unsigned char path_exists(char *path);

unsigned char valid_name(char *name);

char *build_path(unsigned char count, char *args, ...);

container list_folders(char *path);

void write_index(unsigned long value, char* path);

unsigned long next_index(char *path);

#endif //DATI_UTILS_H
