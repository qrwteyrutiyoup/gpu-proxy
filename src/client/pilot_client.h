#ifndef PILOT_CLIENT_H
#define PILOT_CLIENT_H

#include "client.h"
#include "command.h"

private void
client_log_buffer_create ();

private command_t *
client_get_space_for_log_size (client_t *client,
                               size_t size);

private command_t *
client_get_space_for_log_command (command_type_t command_type);

private void
client_run_log_command_async (command_t *command);

private void
client_run_log_command (command_t *command);

private void
client_send_log (void);

#endif /* PILOT_CLIENT_H */
