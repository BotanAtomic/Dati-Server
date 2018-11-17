//
// Created by Botan on 17/11/18.
//

#include <list.h>
#include <string.h>

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
    element *new_element = malloc(sizeof(*new_element));
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