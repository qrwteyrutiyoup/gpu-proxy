
#ifndef GPUPROCESS_COMMAND_BUFFER_H
#define GPUPROCESS_COMMAND_BUFFER_H

#include "gpuprocess_command.h"
#include "gpuprocess_command_buffer_service.h"
#include "gpuprocess_ring_buffer.h"
#include "gpuprocess_types_private.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize the commands in the buffer using the following sequence
 * of calls:
 *   COMMAND_NAME_t *command =
 *       COMMAND_NAME_T(command_buffer_get_command(command_buffer, command_id));
 *   COMMAND_NAME_initialize(command, parameter1, parameter2, ...);
 *   command_buffer_write_command(command_buffer, command);
 */

/* Each command should have this header struct as the first element in
 * their structs.
 */
typedef struct command_buffer {
    buffer_t *buffer;
    command_buffer_service_t *service;
} command_buffer_t;

command_buffer_t *
command_buffer_create();
v_bool_t
command_buffer_destroy(command_buffer_t *command_buffer);
command_t *
command_buffer_get_command(command_buffer_t *command_buffer, command_id_t command_id);
v_bool_t
command_buffer_write_command(command_buffer_t *command_buffer, command_t *command);
v_bool_t
command_buffer_flush(command_buffer_t *command_buffer);
v_bool_t
command_buffer_wait(command_buffer_t *command_buffer);

#ifdef __cplusplus
}
#endif

#endif /* GPUPROCESS_COMMAND_BUFFER_H */
