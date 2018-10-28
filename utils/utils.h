//
// Created by Botan on 26/10/18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#ifndef DATI_UTILS_H
#define DATI_UTILS_H

#endif //DATI_UTILS_H

char* memstrcpy(const char* in) {
    char * out;

    out = malloc(strlen(in) + 1);
    strcpy(out, in);

    return out;
}


char* get_json_string_val(json_object* json_obj, const char* key) {
    return memstrcpy(json_object_get_string(json_object_object_get(json_obj, key)));
}