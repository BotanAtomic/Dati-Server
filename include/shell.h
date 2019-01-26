//
// Created by Botan on 29/10/18.
//

#include "stdio.h"

#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"

void println(const char *__restrict __format, ...);

void print(const char *__restrict __format, ...);

void printSpaces(size_t);

void setColor(char *);

void resetColor();

