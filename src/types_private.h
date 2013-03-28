#ifndef GPUPROCESS_TYPES_PRIVATE_H
#define GPUPROCESS_TYPES_PRIVATE_H

#include <stdbool.h>

typedef void (*list_delete_function_t)(void *data);

typedef struct list_node
{
    void *data;
    struct list_node *next;
    struct list_node *prev;
    list_delete_function_t delete_function;
} list_node_t;

typedef struct link_list
{
    list_node_t *head;
    list_node_t *tail;
} link_list_t;

#define v_ref_count_t unsigned int

#define v_client_id_t pid_t

link_list_t *
link_list_new ();

void
link_list_destroy (link_list_t *list);

void
link_list_append (link_list_t *list,
                  void *data,
                  list_delete_function_t delete_function);

void
link_list_prepend (link_list_t *list,
                   void *data,
                   list_delete_function_t delete_function);


void
link_list_delete_first_entry_matching_data (link_list_t *list,
                                            void *data);

void
link_list_delete_element (link_list_t *list,
                          list_node_t *element);

void
link_list_clear (link_list_t *list);

bool
link_list_match (link_list_t *list, void *data);

#endif /* GPUPROCESS_TYPES_PRIVATE_H */
