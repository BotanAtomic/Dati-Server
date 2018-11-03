//
// Created by Botan on 29/10/18.
//

#include "stdio.h"
#include "shell.h"

void println(const char *__restrict __format, ...) {
    printf(__format);
    printf("\n");
    fflush(stdin);
}

void print(const char *__restrict __format, ...) {
    printf(__format);
    fflush(stdin);
}
