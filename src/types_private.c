#include "types_private.h"
#include <string.h>
#include <stdlib.h>

link_list_t *
link_list_new ()
{
    return (link_list_t *) malloc (sizeof (link_list_t));
}

void
link_list_destroy (link_list_t *list)
{
    link_list_clear (list);
    free (list);
}

void
link_list_append (link_list_t *list,
                  void *data,
                  list_delete_function_t delete_function)
{
    list_node_t *new_node = (list_node_t *) malloc (sizeof (list_node_t));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->delete_function = delete_function;

    if (!list->tail) {
        list->tail = list->head = new_node;
        return;
    }

    new_node->prev = list->tail;
    list->tail->next = new_node;
    list->tail = new_node;
}

void
link_list_prepend (link_list_t *list,
                   void *data,
                   list_delete_function_t delete_function)
{
    list_node_t *new_node = (list_node_t *) malloc (sizeof (list_node_t));
    new_node->data = data;
    new_node->delete_function = delete_function;
    new_node->prev = NULL;

    if (!list->head) {
        list->tail = list->head = new_node;
        return;
    }

    new_node->next = list->head;
    list->head->prev = new_node;
    list->head = new_node;
}

void
link_list_delete_first_entry_matching_data (link_list_t *list, void *data)
{
    list_node_t *current = list->head;
    while (current) {
        if (current->data == data) {
            link_list_delete_element (list, current);
            return;
        }
        current = current->next;
    }
}

void
link_list_delete_element (link_list_t *list, list_node_t *element)
{
    if ((list->head != element) && list->tail != element) {
        element->prev->next = element->next;
        element->next->prev = element->prev;
    } else {
        if (list->head == element) {
            list->head = list->head->next;
            if (list->head)
                list->head->prev = NULL;
        }

        if (list->tail == element) {
            list->tail = list->tail->prev;
            if (list->tail)
                list->tail->next = NULL;
        }
    }

    if (element->delete_function)
        element->delete_function (element->data);

    free (element);
}

void
link_list_clear (link_list_t* list)
{
    list_node_t *current = list->head;
    while (current) {
        list_node_t *next = current->next;
        if (current->delete_function)
            current->delete_function (current->data);
        free (current);
        current = next;
    }

    list->head = list->tail = NULL;
}

bool
link_list_match (link_list_t *list, void *data)
{
    if (!data)
        return false;

    list_node_t *current = list->head;
    while (current) {
        if (current->data == data)
            return true;
        current = current->next;
    }

    return false;
}
