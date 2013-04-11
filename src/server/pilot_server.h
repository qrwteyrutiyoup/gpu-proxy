#ifndef GPUPROCESS_PILOT_SERVER_H
#define GPUPROCESS_PILOT_SERVER_H

#include "ring_buffer.h"
#include "server.h"

private void
pilot_server_init (server_t *server,
                   buffer_t *buffer);

private server_t *
pilot_server_new (buffer_t *buffer);

private void
pilot_server_start_work_loop (server_t *server);

#endif /* GPUPROCESS_PILOT_SERVER_H */
