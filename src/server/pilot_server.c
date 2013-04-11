#include "config.h"
#include "pilot_server.h"

#include "sync_queue.h"
#include "thread_private.h"

static void
pilot_server_handle_no_op (server_t *server,
                           command_t *command)
{
    return;
}


static void
pilot_server_handle_log (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    command_log_t *command = (command_log_t *)abstract_command;

    sync_queue_append_call_log (abstract_command->server_id,
                                abstract_command->timestamp);
    command->result = true;
}

void
pilot_server_init (server_t *server,
                   buffer_t *buffer)
{
    server->egl_display = EGL_NO_DISPLAY;
    server->buffer = buffer;
    server->command_post_hook = NULL;

    server->handler_table[COMMAND_NO_OP] = pilot_server_handle_no_op;
    server->handler_table[COMMAND_LOG] = pilot_server_handle_log;

    server->thread = pthread_self ();
}

void
pilot_server_start_work_loop (server_t *server)
{
    while (true) {
        size_t data_left_to_read;
        command_t *read_command = (command_t *) buffer_read_address (server->buffer,
                                                                     &data_left_to_read);

        /* We ran out of hot cycles, try a more lackadaisical approach. */
        while (! read_command) {
            sem_wait (server->server_signal);
            read_command = (command_t *) buffer_read_address (server->buffer,
                                                              &data_left_to_read);
        }

        if (read_command->type == COMMAND_SHUTDOWN)
            break;

        if (read_command->type != COMMAND_LOG)
            continue;

        server->handler_table[read_command->type](server, read_command);

        if (read_command->token) {
            server->buffer->last_token = read_command->token;
            buffer_read_advance (server->buffer, read_command->size);
            sem_post (server->client_signal);
        }
        else
            buffer_read_advance (server->buffer, read_command->size);
    }
}

server_t *
pilot_server_new (buffer_t *buffer)
{
    server_t *server = malloc (sizeof (server_t));
    pilot_server_init (server, buffer);
    return server;
}

