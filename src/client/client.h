#ifndef CLIENT_H
#define CLIENT_H

#include "egl_states.h"
#include "name_handler.h"
#include "ring_buffer.h"
#include "server.h"
#include "types_private.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize the commands in the buffer using the following sequence
 * of calls:
 *   COMMAND_NAME_t *command =
 *       COMMAND_NAME_T(client_get_space_for_command (command_id));
 *   COMMAND_NAME_initialize (command, parameter1, parameter2, ...);
 *   client_write_command (command);
 */
typedef struct client {
    name_handler_t *name_handler;
    link_list_t *active_egl_state;

    buffer_t buffer;
    server_t *server;
    unsigned int token;
} client_t;

private client_t *
client_get_thread_local ();

private void
client_destroy_thread_local ();

private bool
client_active_egl_state_available ();

private egl_state_t *
client_get_active_egl_state ();

private void
client_set_active_egl_state (link_list_t* active_egl_state);

private int
client_get_unpack_alignment ();

private name_handler_t *
client_get_name_handler ();

private command_t *
client_get_space_for_command (command_id_t command_id);

private bool
client_write_command (command_t *command);

private unsigned int
client_insert_token (client_t *client);

private bool
client_wait_for_token (client_t *client,
                       unsigned int token);
private bool
client_flush (client_t *client);

#ifdef __cplusplus
}
#endif

#endif /* CLIENT_H */
