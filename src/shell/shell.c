//
// Created by Botan on 29/10/18.
//

#include "shell.h"

void println(const char *__restrict __format, ...) {
    printf(__format);
    printf("\n");
    reset_color();
    fflush(stdin);
}

void print(const char *__restrict __format, ...) {
    printf(__format);
    reset_color();
    fflush(stdin);
}

void set_color(char* color) {
    printf("%s", color);
}

void reset_color() {
    printf("\x1B[0m");
}
