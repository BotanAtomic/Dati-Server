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

static char EXCLUDE[] = {' ', '@', '\0'};

char *concatString(char *str, char *str2) {
    if (str == NULL)
        str = "";
    else if (str2 == NULL)
        str2 = "";

    char *string = malloc(strlen(str) + strlen(str2) + 2);
    strcpy(string, str);
    strcpy(string + strlen(str), str2);

    return string;
}

char *copyString(const char *in) {
    char *out = malloc(strlen(in) + 1);
    strcpy(out, in);
    return out;
}


char *getJsonValue(json_object *json_obj, const char *key) {
    return copyString(json_object_get_string(json_object_object_get(json_obj, key)));
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

int removeDirectory(char *path) {
    size_t pathLength;
    char *fullPath;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;

    stat(path, &stat_path);

    if (S_ISDIR(stat_path.st_mode) == 0) {
        return -1;
    }

    if ((dir = opendir(path)) == NULL) {
        return -1;
    }

    pathLength = strlen(path);

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        fullPath = calloc(pathLength + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(fullPath, path);
        strcat(fullPath, FILE_SEPARATOR);
        strcat(fullPath, entry->d_name);

        stat(fullPath, &stat_entry);

        if (S_ISDIR(stat_entry.st_mode) != 0) {
            removeDirectory(fullPath);
            continue;
        }
        unlink(fullPath);
    }

    closedir(dir);

    return rmdir(path);
}

unsigned char pathExists(char *path) {
    struct stat sb;
    return (unsigned char) ((stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) ? 1 : 0);
}

Container listFolders(char *path) {
    DIR *directory = opendir(path);
    char *elements = "";

    uint16_t count = 0;
    if (directory != NULL) {

        struct dirent *path_entries;
        while ((path_entries = readdir(directory)) != NULL) {
            if (path_entries->d_type == DT_DIR && strcmp(path_entries->d_name, ".") != 0 &&
                strcmp(path_entries->d_name, "..") != 0) {

                if (strlen(elements) > 0)
                    elements = concatString(elements, "@");

                elements = concatString(elements, path_entries->d_name);
                count++;
            }
        }
    }

    closedir(directory);

    Container container;
    container.length = count;
    container.elements = elements;

    return container;
}

List *getFolders(char *path) {
    List *folders = createList();

    DIR *dir = opendir(path);

    if (dir != NULL) {

        struct dirent *path_entries;
        while ((path_entries = readdir(dir)) != NULL) {
            if (path_entries->d_type == DT_DIR && strcmp(path_entries->d_name, ".") != 0 &&
                strcmp(path_entries->d_name, "..") != 0) {
                listInsert(folders, path_entries->d_name);
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

char *buildPath(char *args, ...) {
    char *path = "";

    va_list ap;
    va_start(ap, args);

    path = concatString(path, args);

    if (args[strlen(args) - 1] != FILE_SEPARATOR[0])
        path = concatString(path, FILE_SEPARATOR);

    char *tmp = args;
    int i = 0;
    while (tmp) {
        if (i > 0)
            path = concatString(path, FILE_SEPARATOR);
        path = concatString(path, tmp = va_arg(ap, char*));
        i++;
    }

    va_end(ap);

    return path;
}

unsigned char isValidName(char *name) {
    if (strlen(name) > 255 || contains(name, EXCLUDE))
        return 0;
    return 1;
}

void writeIndex(unsigned long value, char *path) {
    FILE *filePointer;
    filePointer = fopen(concatString(path, "index"), "w+b");

    if (filePointer == NULL)
        return;

    fwrite(&value, sizeof(value), 1, filePointer);

    fclose(filePointer);
}


unsigned long readIndex(char *path) {
    FILE *filePointer;
    filePointer = fopen(concatString(path, "index"), "rb");

    if (filePointer == NULL) {
        return 0;
    }

    unsigned long index = 0;

    fread(&index, sizeof(index), 1, filePointer);

    fclose(filePointer);

    return index;
}

unsigned hash(const char *data) {
    uint32_t a = 1, b = 0;
    size_t index;

    for (index = 0; index < strlen(data); ++index) {
        a = (a + data[index]) % 65521;
        b = (b + a) % 65521;
    }

    return (b << 16) | a;
}

char *removeAllSpaces(char *source) {
    char *result = malloc(strlen(source));

    int x = 0;
    do {
        if (*source != ' ')
            result[x++] = *source;
    } while (*source++ != 0);

    result = realloc(result, strlen(result) + 1);
    return result;
}


long long currentTimestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milliseconds;
}

