#include "config.h"
#include "server.h"

#include "caching_server_private.h"
#include "ring_buffer.h"
#include "server_dispatch_table.h"
#include "thread_private.h"
#include <time.h>

extern __thread bool on_client_thread;

/* This method is auto-generated into server_autogen.c 
 * and included at the end of this file. */
static void
server_handle_command_autogen (server_t *server,
                               command_t *command);

static void
server_handle_set_token (server_t *server,
                         command_set_token_t *command)
{
    server->buffer->last_token = command->token;
}

static void
server_handle_glcreateshader (server_t *server,
                              command_glcreateshader_t *command)
{
    command->result = server->dispatch.glCreateShader (server,
                                                       command->shader_type);
}

static void
server_handle_command (server_t *server,
                       command_t *command)
{
    switch (command->id) {
        case COMMAND_NO_OP: break;
        case COMMAND_SET_TOKEN:
            server_handle_set_token (server, (command_set_token_t *)command);
            break;
        case COMMAND_GLCREATESHADER:
            server_handle_glcreateshader (server, (command_glcreateshader_t *)command);
            break;
        defualt:
            server_handle_command_autogen (server, command);
        }
}

static inline void
sleep_nanoseconds (int num_nanoseconds)
{
    struct timespec spec;
    spec.tv_sec = 0;
    spec.tv_nsec = num_nanoseconds;
    nanosleep (&spec, NULL);
}

void
server_start_work_loop (server_t *server)
{
    /* FIXME: initialize GL state and start consuming commands in the loop. */
    /* FIXME: add exit condition of the loop. */
    while (true) {
        size_t data_left_to_read;
        command_t *read_command = (command_t *) buffer_read_address (server->buffer,
                                                                     &data_left_to_read);

        /* The buffer is empty, so wait until there's something to read. */
        while (! read_command) {
            sleep_nanoseconds (100);
            read_command = (command_t *) buffer_read_address (server->buffer,
                                                              &data_left_to_read);
        }

        server_handle_command (server, read_command);

        buffer_read_advance (server->buffer, read_command->size);
    }
}

server_t *
server_new (buffer_t *buffer)
{
    server_t *server = malloc (sizeof (server_t));

    server_init (server, buffer);
    on_client_thread = false;

    return server;
}

void
server_init (server_t *server,
             buffer_t *buffer)
{
    server->buffer = buffer;
    server->dispatch = *server_dispatch_table_get_base();
}

bool
server_destroy (server_t *server)
{
    server->buffer = NULL;

    free (server);
    return true;
}

#include "server_autogen.c"
