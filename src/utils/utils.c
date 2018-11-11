//
// Created by Botan on 26/10/18.
//


#include "utils.h"

#ifdef _WIN32
#define FILE_SEPARATOR "\\"
#endif

#ifdef linux
#define FILE_SEPARATOR "/"
#endif

static char exclude[] = {' ', '@', '\0'};

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
        for (int y = 0; y < sizeof(c); y++) {
            if (str[i] == c[y])
                return 1;
        }

    }
    return 0;
}

int remove_directory(char *path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        struct dirent *p;

        r = 0;

        while (!r && (p = readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
                continue;

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf) {
                struct stat statbuf;

                buf = build_path(2, path, p->d_name);

                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = remove_directory(buf);
                    } else {
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }

            r = r2;
        }

        closedir(d);
    }

    if (!r) {
        r = rmdir(path);
    }

    return r;
}

unsigned char path_exists(char *path) {
    struct stat sb;

    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return 1;
    } else {
        return 0;
    }
}

single_container list_folders(char *path) {
    DIR *dir = opendir(path);
    char *elements = "";

    uint16_t count = 0;
    if (dir != NULL) {

        struct dirent *path_entries;
        while ((path_entries = readdir(dir)) != NULL) {
            if (path_entries->d_type == DT_DIR && strcmp(path_entries->d_name, ".") != 0 &&
                strcmp(path_entries->d_name, "..") != 0) {

                if (strlen(elements) > 0)
                    concat_string(&elements, "@");

                concat_string(&elements, path_entries->d_name);
                count++;
            }
        }
    }

    closedir(dir);

    single_container container;
    container.count = count;
    container.elements = elements;

    return container;
}

char *build_path(unsigned char count, char *args, ...) {
    char *path = NULL;

    va_list ap;
    va_start(ap, args);

    concat_string(&path, args);
    concat_string(&path, FILE_SEPARATOR);

    for (int i = 0; i < (count - 1); i++) {
        if (i > 0)
            concat_string(&path, FILE_SEPARATOR);
        concat_string(&path, va_arg(ap, char*));
    }

    va_end(ap);
    return path;
}

unsigned char valid_name(char *name) {
    if (strlen(name) > 255 || contains(name, exclude))
        return 0;
    return 1;
}

void create_index(char *path) {
    FILE *fPtr;
    fPtr = fopen(path, "w");

    if (fPtr == NULL) {
        return;
    }

    fputs("1", fPtr);

    fclose(fPtr);
}
