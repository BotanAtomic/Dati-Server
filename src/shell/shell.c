//
// Created by Botan on 29/10/18.
//

#include "shell.h"

void println(const char *__restrict __format, ...) {
    printf(__format);
    printf("\n");
    resetColor();
    fflush(stdout);
}

void print(const char *__restrict __format, ...) {
    printf(__format);
    resetColor();
    fflush(stdout);
}

void printSpaces(size_t n) {
    if(n > 100)
        return;

    for (int i = 0; i < n; i++)
        printf(" ");
    resetColor();
}

void setColor(char *color) {
    printf("%s", color);
}

void resetColor() {
    printf("\x1B[0m");
}
