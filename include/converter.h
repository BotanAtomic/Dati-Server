//
// Created by Botan on 18/11/18.
//

#ifndef DATI_CONVERTER_H
#define DATI_CONVERTER_H

#include <sys/param.h>
#include <string.h>


void *convertChar(char *);

void *convertUChar(char *);

void *convertShort(char *);

void *convertUShort(char *);

void *convertInt(char *);

void *convertUInt(char *);

void *convertLong(char *);

void *convertULong(char *);

void *convertString(char *);

static void* (*CONVERTERS[])(char *) ={
        convertChar, convertUChar, convertShort, convertUShort, convertInt, convertUInt,
        convertLong, convertULong, NULL/** float **/, NULL /** double **/, convertString
};

#endif //DATI_CONVERTER_H
