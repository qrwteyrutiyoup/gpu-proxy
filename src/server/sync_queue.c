
#include "sync_queue.h"

#include <stdlib.h>

mutex_static_init (sync_queue_mutex);
signal_static_init (sync_queue_signal);

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
    mutex_lock (sync_queue_mutex);
    _call_order_list_append (server);
    mutex_unlock (sync_queue_mutex);
}

void
sync_queue_allow_call (command_t *command,
                       thread_t server)
{
    if (command->use_timestamp) {
        mutex_lock (sync_queue_mutex);
        while (! _call_order_list_head_is_server (server))
            wait_signal (sync_queue_signal, sync_queue_mutex);
    }
}

void
sync_queue_remove_call_log (command_t *command)
{
    if (command->use_timestamp) {
        _call_order_list_remove ();
        broadcast (sync_queue_signal);
        mutex_unlock (sync_queue_mutex);
    }
}

