//
// Created by Botan on 20/01/19.
//
#include "scheduler.h"

void writeTableIndex(void *tablePointer) {
    Table *table = tablePointer;
    writeIndex(table->index, buildPath(dataPath, table->database->name, table->name, 0));
}

void *schedule(void *second) {
    while (1) {
        sleep((unsigned short) second);
        long long startTime = currentTimestamp();

        for (long i = 0; i < bufferIndex; i++) {
            writeTableValue(bufferActions[i]);
            bufferActions[i] = NULL;
        }

        bufferIndex = 0;

        for (Element *element = globalDatabases->element; element != NULL; element = element->next) {
            Database *database = element->value;
            foreach(database->tables, writeTableIndex);
        }

        long long endTime = currentTimestamp();

        printf("Database flushed in %lu ms\n", (unsigned long) (endTime - startTime));
        fflush(stdout);
    }
}

void startTimer(unsigned short second) {
    printf("Flush timer configured for %u seconds\n\n", second);
    bufferActions = malloc(sizeof(TableValue) * MAX_BUFFER_SIZE);
    bufferIndex = 0;
    pthread_t thread;
    pthread_create(&thread, NULL, schedule, (void *) second);
}

void insertFuture(TableValue *tableValue) {
    if (bufferIndex < MAX_BUFFER_SIZE) {
        bufferActions[bufferIndex] = tableValue;
        bufferIndex++;
    }
}
