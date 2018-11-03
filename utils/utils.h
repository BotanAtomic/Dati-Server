//
// Created by Botan on 26/10/18.
//


#ifndef DATI_UTILS_H
#define DATI_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>


void concat_string(char **str, const char *str2);

char *memstrcpy(const char *in);

char *get_json_value(json_object *json_obj, const char *key);

unsigned char contains(char* str, char c[]);

#endif //DATI_UTILS_H
