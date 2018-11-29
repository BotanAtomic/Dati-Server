//
// Created by Botan on 17/11/18.
//

#include <list.h>
#include <string.h>
#include <database.h>

#include "list.h"

list *list_create() {
    list *list = malloc(sizeof(*list));

    if (list) {
        list->element = NULL;
        list->length = 0;
    }

    return list;
}

void list_insert(list *list, void *value) {
    element *new_element = malloc(sizeof(element));
    if (list && new_element) {
        new_element->value = value;
        new_element->next = list->element;
        list->element = new_element;
        list->length++;
    }
}

void list_free(list *list) {
    element *element = list->element;
    while (element != NULL) {
        free(element);
        element = element->next;
    }

    free(list);
}


void foreach(list *list, void (*loop)(void *)) {
    element *element = list->element;

    while (element) {
        loop(element->value);
        element = element->next;
    }
}

void *list_search(list *list, void *value) {
    element *element = list->element;

    while (element) {
        if (list->comparator(value, element->value) == 0)
            return element->value;

        element = element->next;
    }

    return NULL;
}

void list_delete(list *list, void *value) {
    element *previous = list->element;
    element *element = list->element;


    while (element) {
        if (list->comparator(value, element->value) == 0) {
            printf("Delete %s \n", ((database *) element->value)->name);
            list->length--;

            if (list->element != element)
                previous->next = element->next;
            else
                list->element = element->next;

            free(element);
            break;
        }

        previous = element;
        element = element->next;
    }
}
