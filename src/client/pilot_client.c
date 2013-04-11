#define _GNU_SOURCE

#include "config.h"
#include "pilot_client.h"

#include "pilot_server.h"

#include "types_private.h"
#include "thread_private.h"

#include <pthread.h>
#include <sched.h>

mutex_static_init (pilot_command_mutex);
mutex_static_init (pilot_thread_mutex);

/* static for pilot server */
static buffer_t pilot_buffer;
static bool pilot_buffer_created = false;
static unsigned int pilot_token = 0;
static sem_t pilot_server_signal;
static sem_t pilot_client_signal;

static thread_t
pilot_server ()
{
    static thread_t pilot_thread = 0;
    return pilot_thread;
}

static void *
start_pilot_server_thread_func (void *ptr)
{
    server_t *server = pilot_server_new (&pilot_buffer);

    server->server_signal = &pilot_server_signal;
    server->client_signal = &pilot_client_signal;

    pthread_t id = pthread_self ();

    int online_cpus = sysconf (_SC_NPROCESSORS_ONLN);
    int available_cpus = sysconf (_SC_NPROCESSORS_CONF);

    if (online_cpus > 1) {
        cpu_set_t cpu_set;
        CPU_ZERO (&cpu_set);
        if (pthread_getaffinity_np (id, sizeof (cpu_set_t), &cpu_set) == 0) {

            /* find first cpu to run on */
            int cpu = 0;
            int i;
            for (i = 1; i < available_cpus; i++) {
                if (CPU_ISSET (i, &cpu_set)) {
                    cpu = i;
                    break;
                }
            }
            /* force server to run on cpu2 */
            if (available_cpus > 2) {
                if (cpu == 0 || cpu == 1)
                    cpu = 2;
            } else {
                if (cpu == 0)
                    cpu = 1;
            }
            if (cpu != 0) {
                for (i = 0; i < available_cpus; i++) {
                    if (i != cpu)
                        CPU_CLR (i, &cpu_set);
                }
                CPU_SET (cpu, &cpu_set);
                pthread_setaffinity_np (id, sizeof (cpu_set_t), &cpu_set);
            }
        }
    }

    pilot_server_start_work_loop (server);

    server_destroy(server);
    return NULL;
}

static void
client_start_pilot_server ()
{
    thread_t pilot_thread = pilot_server ();
    if (pilot_thread == 0) {
        sem_init (&pilot_server_signal, 0, 0);
        sem_init (&pilot_client_signal, 0, 0);

        pthread_create (&pilot_thread, NULL, start_pilot_server_thread_func, NULL);
    }
}

void
client_log_buffer_create ()
{
    mutex_lock (pilot_thread_mutex);
    if (pilot_buffer_created == false) {
        buffer_create (&pilot_buffer, 512, "pilot");
        pilot_buffer_created = true;
        client_start_pilot_server ();
    }
    mutex_unlock (pilot_thread_mutex);
}

command_t *
client_get_space_for_log_size (client_t *client, size_t size)
{
    size_t available_space;
    command_t *write_location;

    write_location = (command_t *)buffer_write_address (&pilot_buffer,
                                                        &available_space);
    while (! write_location || available_space < size) {
        sched_yield ();
        write_location = (command_t *)buffer_write_address (&pilot_buffer,
                                                            &available_space);
    }

    return write_location;
}

command_t *
client_get_space_for_log_command (command_type_t command_type)
{
    assert (command_type >= 0 && command_type < COMMAND_MAX_COMMAND);

    client_t *client = client_get_thread_local ();
    size_t command_size = command_get_size (command_type);
    command_t *command = client_get_space_for_log_size (client, command_size);
    /* Command size is never bigger than the buffer, no NULL check. */
    command->type = command_type;
    command->size = command_size;
    command->token = 0;
    command->server_id = client->server_thread;
    return command;
}

void
client_run_log_command_async (command_t *command)
{
    buffer_write_advance (&pilot_buffer, command->size);

    if (pilot_buffer.fill_count == command->size) {
        sem_post (&pilot_server_signal);
    }
}

void
client_run_log_command (command_t *command)
{
    pilot_token ++;

    /* Overflow case */
    if (pilot_token == 0)
        pilot_token = 1;

    command->token = pilot_token;
    client_run_log_command_async (command);

    while (pilot_buffer.last_token < pilot_token) {
        sem_wait (&pilot_client_signal);
    }
}

void
client_send_log (void)
{
    mutex_lock (pilot_command_mutex);
    command_t *command = client_get_space_for_log_command (COMMAND_LOG);

    client_run_log_command (command);
    mutex_unlock (pilot_command_mutex);
}
