//
// Created by Botan on 20/01/19.
//

#ifndef DATI_TIMER_H
#define DATI_TIMER_H

#define MAX_BUFFER_SIZE 100000

#include <unistd.h>
#include <pthread.h>
#include "database.h"
#include "utils.h"
#include "server.h"

TableValue ** bufferActions;
unsigned long bufferIndex;

/**
 * Delay in seconds
 */
void startTimer(unsigned short);

void insertFuture(TableValue * tableValue);

#endif //DATI_TIMER_H
