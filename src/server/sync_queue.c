
#include "sync_queue.h"

#include <stdlib.h>

static link_list_t call_list = {NULL, NULL};

static void
_call_order_list_append (thread_t server)
{
    server_log_t *log = (server_log_t *) malloc (sizeof (server_log_t));
    log->server = server;
    link_list_append (&call_list, log, free);
}

static void
_call_order_list_remove ()
{
    if (call_list.head)
        link_list_delete_element (&call_list, call_list.head);
}

static bool
_call_order_list_head_is_server (thread_t server)
{
    list_node_t *head = call_list.head;

    if (head) {
        server_log_t *log = (server_log_t *)(head->data);
        if (log->server == server)
            return true;
    }

    return false;
}

/* Public functions. */
void
sync_queue_append_call_log (thread_t server)
{
    _call_order_list_append (server);
}

bool
sync_queue_allow_call (thread_t server)
{
    return _call_order_list_head_is_server (server);
}

void
sync_queue_remove_call_log ()
{
    _call_order_list_remove ();
}

