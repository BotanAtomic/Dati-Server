//
// Created by Botan on 18/11/18.
//

#ifndef DATI_COMPARATOR_H
#define DATI_COMPARATOR_H

#include <sys/param.h>
#include <string.h>


char compare_char(void *first, void *second);

char compare_uchar(void *first, void *second);

char compare_short(void *first, void *second);

char compare_ushort(void *first, void *second);

char compare_int(void *first, void *second);

char compare_uint(void *first, void *second);

char compare_long(void *first, void *second);

char compare_ulong(void *first, void *second);

char compare_database(void *first, void *second);

char compare_table(void *first, void *second);

char compare_string(void *first, void *second);

static char (*COMPARATORS[])(void *first, void *second) ={
        compare_char, compare_uchar, compare_short, compare_ushort, compare_int, compare_uint,
        compare_long, compare_ulong, NULL/** float **/, NULL /** double **/, compare_uint /** hash **/
};

#endif //DATI_COMPARATOR_H
