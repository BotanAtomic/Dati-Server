//
// Created by Botan on 18/11/18.
//

#ifndef DATI_COMPARATOR_H
#define DATI_COMPARATOR_H

#include <sys/param.h>
#include <string.h>
#include "database.h"
#include "tree.h"

#define EQUAL 61
#define INFERIOR 60
#define SUPERIOR 62

char compareChar(void *, void *);

char compareUChar(void *, void *);

char compareShort(void *, void *);

char compareUShort(void *, void *);

char compareInt(void *, void *);

char compareUInt(void *, void *);

char compareLong(void *, void *);

char compareULong(void *, void *);

char compareDatabase(void *, void *);

char compareTable(void *, void *);

char compareTableValue(void *, void *);

char compareString(void *, void *);

char compareBinaryTree(void *, void *);

static char (*COMPARATORS[])(void *, void *) ={
        compareChar, compareUChar, compareShort, compareUShort, compareInt, compareUInt,
        compareLong, compareULong, NULL/** float **/, NULL /** double **/, compareUInt /** hash **/
};

#endif //DATI_COMPARATOR_H
