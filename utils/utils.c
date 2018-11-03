//
// Created by Botan on 26/10/18.
//


#include "utils.h"


void concat_string(char **str, const char *str2) {
    char *tmp = NULL;

    if (*str != NULL && str2 == NULL) {
        free(*str);
        *str = NULL;
        return;
    }

    if (*str == NULL) {
        *str = calloc(strlen(str2) + 1, sizeof(char));
        memcpy(*str, str2, strlen(str2));
    } else {
        tmp = calloc(strlen(*str) + 1, sizeof(char));
        memcpy(tmp, *str, strlen(*str));
        *str = calloc(strlen(*str) + strlen(str2) + 1, sizeof(char));
        memcpy(*str, tmp, strlen(tmp));
        memcpy(*str + strlen(*str), str2, strlen(str2));
        free(tmp);
    }

}

char *memstrcpy(const char *in) {
    char *out;

    out = malloc(strlen(in) + 1);
    strcpy(out, in);

    return out;
}


char *get_json_value(json_object *json_obj, const char *key) {
    return memstrcpy(json_object_get_string(json_object_object_get(json_obj, key)));
}

unsigned char contains(char *str, char c[]) {
    for (int i = 0; i < strlen(str); i++) {
        for(int y = 0; y < sizeof(c); y++) {
            if (str[i] == c[y])
                return 1;
        }

    }
    return 0;
}
