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

char *concat_string(char *str, char *str2) {
    if (str == NULL)
        str = "";
    else if (str2 == NULL)
        str2 = "";

    char *string = malloc(strlen(str) + strlen(str2) + 2);
    strcpy(string, str);
    strcpy(string + strlen(str), str2);

    return string;
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

unsigned char contains(char *str, char *c) {
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

                buf = build_path(path, p->d_name, 0);

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

container list_folders(char *path) {
    DIR *dir = opendir(path);
    char *elements = "";

    uint16_t count = 0;
    if (dir != NULL) {

        struct dirent *path_entries;
        while ((path_entries = readdir(dir)) != NULL) {
            if (path_entries->d_type == DT_DIR && strcmp(path_entries->d_name, ".") != 0 &&
                strcmp(path_entries->d_name, "..") != 0) {

                if (strlen(elements) > 0)
                    elements = concat_string(elements, "@");

                elements = concat_string(elements, path_entries->d_name);
                count++;
            }
        }
    }

    closedir(dir);

    container container;
    container.length = count;
    container.elements = elements;

    return container;
}

list *get_folders(char *path) {
    list *folders = list_create();

    DIR *dir = opendir(path);

    if (dir != NULL) {

        struct dirent *path_entries;
        while ((path_entries = readdir(dir)) != NULL) {
            if (path_entries->d_type == DT_DIR && strcmp(path_entries->d_name, ".") != 0 &&
                strcmp(path_entries->d_name, "..") != 0) {
                list_insert(folders, path_entries->d_name);
            }
        }
    }

    closedir(dir);

    return folders;
}

/**
 * Must end with 0 !
 * @param args
 * @param ...
 * @return
 */

char *build_path(char *args, ...) {
    char *path = "";

    va_list ap;
    va_start(ap, args);

    path = concat_string(path, args);
    path = concat_string(path, FILE_SEPARATOR);

    char *tmp = args;
    int i = 0;
    while (tmp) {
        if (i > 0)
            path = concat_string(path, FILE_SEPARATOR);
        path = concat_string(path, tmp = va_arg(ap, char*));
        i++;
    }

    va_end(ap);

    return path;
}

unsigned char valid_name(char *name) {
    if (strlen(name) > 255 || contains(name, exclude))
        return 0;
    return 1;
}

void write_index(unsigned long value, char *path) {
    FILE *file_ptr;
    file_ptr = fopen(concat_string(path, "index"), "wb");

    if (file_ptr == NULL)
        return;

    fprintf(file_ptr, "%lu", value);

    fclose(file_ptr);
}


unsigned long read_index(char *path) {
    FILE *file_ptr;
    file_ptr = fopen(concat_string(path, "index"), "r");

    if (file_ptr == NULL) {
        return 0;
    }

    unsigned long index = 0;

    fscanf(file_ptr, "%lu", &index);

    fclose(file_ptr);

    return index;
}

unsigned hash(char *data) {
    uint32_t a = 1, b = 0;
    size_t index;

    for (index = 0; index < strlen(data); ++index) {
        a = (a + data[index]) % 65521;
        b = (b + a) % 65521;
    }

    return (b << 16) | a;
}




