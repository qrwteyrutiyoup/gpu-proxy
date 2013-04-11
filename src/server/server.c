#include "config.h"
#include "server.h"

#include "ring_buffer.h"
#include "dispatch_table.h"
#include "thread_private.h"
#include <time.h>

/* This method is auto-generated into server_autogen.c
 * and included at the end of this file. */
static void
server_fill_command_handler_table (server_t *server);

void
server_start_work_loop (server_t *server)
{
    while (true) {
        size_t data_left_to_read;
        command_t *read_command = (command_t *) buffer_read_address (server->buffer,
                                                                     &data_left_to_read);
        /* The buffer is empty, so wait until there's something to read. */
         /*int times_slept = 0;
         while (! read_command && times_slept < 20) {
             sleep_nanoseconds (500);
             read_command = (command_t *) buffer_read_address (server->buffer,
                                                           &data_left_to_read);
             times_slept++;
         }*/

        /* We ran out of hot cycles, try a more lackadaisical approach. */
        while (! read_command) {
            sem_wait (server->server_signal);
            read_command = (command_t *) buffer_read_address (server->buffer,
                                                              &data_left_to_read);
        }

        if (read_command->type == COMMAND_SHUTDOWN)
            break;

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
server_new (buffer_t *buffer)
{
    server_t *server = malloc (sizeof (server_t));
    server_init (server, buffer);
    return server;
}

static void
server_handle_no_op (server_t *server,
                     command_t *command)
{
    return;
}

mutex_static_init (name_mapping_mutex);
static HashTable *name_mapping_buffer = NULL;
static HashTable *name_mapping_renderbuffer = NULL;
static HashTable *name_mapping_shader_object = NULL;
static HashTable *name_mapping_texture = NULL;
static HashTable *name_mapping_framebuffer = NULL;

static void
server_handle_glbindbuffer (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();
    command_glbindbuffer_t *command =
            (command_glbindbuffer_t *)abstract_command;
    if (command->buffer) {
        mutex_lock (name_mapping_mutex);
        GLuint *buffer = hash_lookup (name_mapping_buffer, command->buffer);
        mutex_unlock (name_mapping_mutex);
        if (!buffer) {
            GLuint *data = (GLuint *) malloc (1 * sizeof (GLuint));
            *data = command->buffer;
            hash_insert (name_mapping_buffer, *data, data);
            buffer = data;
        }
        command->buffer = *buffer;
    }
    server->dispatch.glBindBuffer (server, command->target, command->buffer);
    command_glbindbuffer_destroy_arguments (command);
}
static void
server_handle_glbindtexture (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();
    command_glbindtexture_t *command =
            (command_glbindtexture_t *)abstract_command;
    if (command->texture) {
        mutex_lock (name_mapping_mutex);
        GLuint *texture = hash_lookup (name_mapping_texture, command->texture);
        mutex_unlock (name_mapping_mutex);
        if (!texture) {
            GLuint *data = (GLuint *) malloc (1 * sizeof (GLuint));
            *data = command->texture;
            hash_insert (name_mapping_texture, *data, data);
            texture = data;
        }
        command->texture = *texture;
    }
    server->dispatch.glBindTexture (server, command->target, command->texture);
    command_glbindtexture_destroy_arguments (command);
}

static void
server_handle_glbindframebuffer (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();
    command_glbindframebuffer_t *command =
            (command_glbindframebuffer_t *)abstract_command;
    if (command->framebuffer) {
        mutex_lock (name_mapping_mutex);
        GLuint *framebuffer =
            hash_lookup (name_mapping_framebuffer, command->framebuffer);
        mutex_unlock (name_mapping_mutex);
        if (!framebuffer) {
            GLuint *data = (GLuint *) malloc (1 * sizeof (GLuint));
            *data = command->framebuffer;
            hash_insert (name_mapping_framebuffer, *data, data);
            framebuffer = data;
        }
        command->framebuffer = *framebuffer;
    }
    server->dispatch.glBindFramebuffer (server, command->target, command->framebuffer);
    command_glbindframebuffer_destroy_arguments (command);
}

static void
server_handle_glbindrenderbuffer (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();
    command_glbindrenderbuffer_t *command =
            (command_glbindrenderbuffer_t *)abstract_command;
    if (command->renderbuffer) {
        mutex_lock (name_mapping_mutex);
        GLuint *renderbuffer =
            hash_lookup (name_mapping_renderbuffer, command->renderbuffer);
        mutex_unlock (name_mapping_mutex);
        if (!renderbuffer) {
            GLuint *data = (GLuint *) malloc (1 * sizeof (GLuint));
            *data = command->renderbuffer;
            hash_insert (name_mapping_renderbuffer, *data, data);
            renderbuffer = data;
        }
        command->renderbuffer = *renderbuffer;
    }
    server->dispatch.glBindRenderbuffer (server, command->target, command->renderbuffer);
    command_glbindrenderbuffer_destroy_arguments (command);
}

static void
server_handle_glgenbuffers (server_t *server,
                                   command_t *abstract_command)
{
    INSTRUMENT();

    command_glgenbuffers_t *command =
        (command_glgenbuffers_t *)abstract_command;

    GLuint *server_buffers = (GLuint *)malloc (command->n * sizeof (GLuint));
    server->dispatch.glGenBuffers (server, command->n, server_buffers);

    int i;
    mutex_lock (name_mapping_mutex);
    for (i = 0; i < command->n; i++) {
        GLuint *data = (GLuint *)malloc (sizeof (GLuint));
        *data = server_buffers[i];
        hash_insert (name_mapping_buffer, command->buffers[i], data);
    }
    mutex_unlock (name_mapping_mutex);

    free (server_buffers);
    command_glgenbuffers_destroy_arguments (command);
}

static void
server_handle_gldeletebuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    command_gldeletebuffers_t *command =
        (command_gldeletebuffers_t *)abstract_command;

    int i;
    mutex_lock (name_mapping_mutex);
    for (i = 0; i < command->n; i++) {
        GLuint *entry = hash_take (name_mapping_buffer, command->buffers[i]);
        if (entry) {
            command->buffers[i] = *entry;
            free (entry);
        }
    }
    mutex_unlock (name_mapping_mutex);

    server->dispatch.glDeleteBuffers (server, command->n, command->buffers);

    command_gldeletebuffers_destroy_arguments (command);
}

static void
server_handle_glgenframebuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    command_glgenframebuffers_t *command =
        (command_glgenframebuffers_t *)abstract_command;

    GLuint *server_framebuffers = (GLuint *)malloc (command->n * sizeof (GLuint));
    server->dispatch.glGenFramebuffers (server, command->n, server_framebuffers);

    int i;
    mutex_lock (name_mapping_mutex);
    for (i = 0; i < command->n; i++) {
        GLuint *data = (GLuint *)malloc (sizeof (GLuint));
        *data = server_framebuffers[i];
        hash_insert (name_mapping_framebuffer, command->framebuffers[i], data);
    }
    mutex_unlock (name_mapping_mutex);

    free (server_framebuffers);
    command_glgenframebuffers_destroy_arguments (command);
}

static void
server_handle_gldeleteframebuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    command_gldeleteframebuffers_t *command =
        (command_gldeleteframebuffers_t *)abstract_command;

    int i;
    mutex_lock (name_mapping_mutex);
    for (i = 0; i < command->n; i++) {
        GLuint *entry = hash_take (name_mapping_framebuffer, command->framebuffers[i]);
        if (entry) {
            command->framebuffers[i] = *entry;
            free (entry);
        }
    }
    mutex_unlock (name_mapping_mutex);

    server->dispatch.glDeleteFramebuffers (server, command->n, command->framebuffers);

    command_gldeleteframebuffers_destroy_arguments (command);
}

static void
server_handle_glgentextures (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    command_glgentextures_t *command =
        (command_glgentextures_t *)abstract_command;

    GLuint *server_textures = (GLuint *)malloc (command->n * sizeof (GLuint));
    server->dispatch.glGenTextures (server, command->n, server_textures);

    int i;
    mutex_lock (name_mapping_mutex);
    for (i = 0; i < command->n; i++) {
        GLuint *data = (GLuint *)malloc (sizeof (GLuint));
        *data = server_textures[i];
        hash_insert (name_mapping_texture, command->textures[i], data);
    }
    mutex_unlock (name_mapping_mutex);

    free (server_textures);
    command_glgentextures_destroy_arguments (command);
}

static void
server_handle_gldeletetextures (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    command_gldeletetextures_t *command =
        (command_gldeletetextures_t *)abstract_command;

    int i;
    mutex_lock (name_mapping_mutex);
    for (i = 0; i < command->n; i++) {
        GLuint *entry = hash_take (name_mapping_texture, command->textures[i]);
        if (entry) {
            command->textures[i] = *entry;
            free (entry);
        }
    }
    mutex_unlock (name_mapping_mutex);

    server->dispatch.glDeleteTextures (server, command->n, command->textures);

    command_gldeletetextures_destroy_arguments (command);
}

static void
server_handle_glgenrenderbuffers (server_t *server,
                                         command_t *abstract_command)
{
    INSTRUMENT();

    command_glgenrenderbuffers_t *command =
        (command_glgenrenderbuffers_t *)abstract_command;

    GLuint *server_renderbuffers = (GLuint *)malloc (command->n * sizeof (GLuint));
    server->dispatch.glGenRenderbuffers (server, command->n, server_renderbuffers);

    int i;
    mutex_lock (name_mapping_mutex);
    for (i = 0; i < command->n; i++) {
        GLuint *data = (GLuint *)malloc (sizeof (GLuint));
        *data = server_renderbuffers[i];
        hash_insert (name_mapping_renderbuffer, command->renderbuffers[i], data);
    }
    mutex_unlock (name_mapping_mutex);

    free (server_renderbuffers);
    command_glgenrenderbuffers_destroy_arguments (command);
}

static void
server_handle_gldeleterenderbuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    command_gldeleterenderbuffers_t *command =
        (command_gldeleterenderbuffers_t *)abstract_command;

    int i;
    mutex_lock (name_mapping_mutex);
    for (i = 0; i < command->n; i++) {
        GLuint *entry = hash_take (name_mapping_renderbuffer, command->renderbuffers[i]);
        if (entry) {
            command->renderbuffers[i] = *entry;
            free (entry);
        }
    }
    mutex_unlock (name_mapping_mutex);

    server->dispatch.glDeleteBuffers (server, command->n, command->renderbuffers);
    command_gldeleterenderbuffers_destroy_arguments (command);
}

static void
server_handle_glcreateprogram (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    command_glcreateprogram_t *command =
            (command_glcreateprogram_t *)abstract_command;
    GLuint *program = (GLuint *)malloc (sizeof (GLuint));

    *program = server->dispatch.glCreateProgram (server);

    mutex_lock (name_mapping_mutex);
    hash_insert (name_mapping_shader_object, command->result, program);
    mutex_unlock (name_mapping_mutex);

}

static void
server_handle_gldeleteprogram (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    command_gldeleteprogram_t *command =
            (command_gldeleteprogram_t *)abstract_command;

    mutex_lock (name_mapping_mutex);
    GLuint *program = hash_take (name_mapping_shader_object, command->program);
    
    if (program) {
        GLuint program_value = *program;
        mutex_unlock (name_mapping_mutex);
        server->dispatch.glDeleteProgram (server, program_value);
    } else
        mutex_unlock (name_mapping_mutex);

    command_gldeleteprogram_destroy_arguments (command);
}

static void
server_handle_glcreateshader (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    command_glcreateshader_t *command =
            (command_glcreateshader_t *)abstract_command;
    GLuint *shader = (GLuint *)malloc (sizeof (GLuint));

    *shader = server->dispatch.glCreateShader (server, command->type);

    mutex_lock (name_mapping_mutex);
    hash_insert (name_mapping_shader_object, command->result, shader);
    mutex_unlock (name_mapping_mutex);
}

static void
server_handle_gldeleteshader (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    command_gldeleteshader_t *command =
            (command_gldeleteshader_t *)abstract_command;

    mutex_lock (name_mapping_mutex);
    GLuint *shader = hash_take (name_mapping_shader_object, command->shader);
    mutex_unlock (name_mapping_mutex);

    if (shader) {
        GLuint shader_value = *shader;
        mutex_unlock (name_mapping_mutex);
        server->dispatch.glDeleteShader (server, shader_value);
    }
    else {
        mutex_unlock (name_mapping_mutex);
        /*XXX: This call should return INVALID_VALUE */
        server->dispatch.glDeleteShader (server, 0xffffffff);
    }

    command_gldeleteshader_destroy_arguments (command);
}

static void
server_handle_glgetprogrambinaryoes (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();
    command_glgetprogrambinaryoes_t *command =
            (command_glgetprogrambinaryoes_t *)abstract_command;
    
    mutex_lock (name_mapping_mutex);
    GLuint *program = hash_take (name_mapping_shader_object, command->program);
    
    if (program) {
        GLuint program_value = *program;
        mutex_unlock (name_mapping_mutex);
        server->dispatch.glGetProgramBinaryOES (server, program_value, command->bufSize, command->length, command->binaryFormat, command->binary);
    } else
        mutex_unlock (name_mapping_mutex);

}

static void
server_handle_glprogrambinaryoes (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();
    command_glprogrambinaryoes_t *command =
            (command_glprogrambinaryoes_t *)abstract_command;
    
    mutex_lock (name_mapping_mutex);
    GLuint *program = hash_take (name_mapping_shader_object, command->program);
    
    if (program) {
        GLuint program_value = *program;
        mutex_unlock (name_mapping_mutex);
        server->dispatch.glProgramBinaryOES (server, program_value, command->binaryFormat, command->binary, command->length);
    } else
        mutex_unlock (name_mapping_mutex);

    command_glprogrambinaryoes_destroy_arguments (command);
}

static void
server_handle_glreadpixels (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();
    command_glreadpixels_t *command =
            (command_glreadpixels_t *)abstract_command;
    server->dispatch.glReadPixels (server, command->x, command->y, command->width, command->height, command->format, command->type, command->pixels);

    /* FIXME:  Do we need this */
    /*
    GLenum error = server->dispatch.glGetError (server);
    while (error == GL_OUT_OF_MEMORY) {
        server->dispatch.glReadPixels (server, command->x, command->y,
                                       command->width, command->height,
                                       command->format, command->type,
                                       command->pixels);
        error = server->dispatch.glGetError (server);
    }
    */
}

static void
server_handle_glgeterror (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();
    command_glgeterror_t *command =
            (command_glgeterror_t *)abstract_command;
    command->result = server->dispatch.glGetError (server);
    GLenum error = command->result;
    while (error != GL_NO_ERROR)
        error = server->dispatch.glGetError (server);
}

void
server_init (server_t *server,
             buffer_t *buffer)
{
    server->buffer = buffer;
    server->dispatch = *dispatch_table_get_base();
    server->command_post_hook = NULL;

    server->handler_table[COMMAND_NO_OP] = server_handle_no_op;
    server_fill_command_handler_table (server);

    server->handler_table[COMMAND_GLGENBUFFERS] =
        server_handle_glgenbuffers;
    server->handler_table[COMMAND_GLDELETEBUFFERS] =
        server_handle_gldeletebuffers;
    server->handler_table[COMMAND_GLDELETEFRAMEBUFFERS] =
        server_handle_gldeleteframebuffers;
    server->handler_table[COMMAND_GLGENFRAMEBUFFERS] =
        server_handle_glgenframebuffers;
    server->handler_table[COMMAND_GLGENTEXTURES] =
        server_handle_glgentextures;
    server->handler_table[COMMAND_GLDELETETEXTURES] =
        server_handle_gldeletetextures;
    server->handler_table[COMMAND_GLGENRENDERBUFFERS] =
        server_handle_glgenrenderbuffers;
    server->handler_table[COMMAND_GLDELETERENDERBUFFERS] =
        server_handle_gldeleterenderbuffers;
    server->handler_table[COMMAND_GLCREATEPROGRAM] =
        server_handle_glcreateprogram;
    server->handler_table[COMMAND_GLDELETEPROGRAM] =
        server_handle_gldeleteprogram;
    server->handler_table[COMMAND_GLCREATESHADER] =
        server_handle_glcreateshader;
    server->handler_table[COMMAND_GLDELETESHADER] =
        server_handle_gldeleteshader;
    server->handler_table[COMMAND_GLGETPROGRAMBINARYOES] =
        server_handle_glgetprogrambinaryoes;
    server->handler_table[COMMAND_GLPROGRAMBINARYOES] =
        server_handle_glprogrambinaryoes;
    server->handler_table[COMMAND_GLGETERROR] =
        server_handle_glgeterror;
    server->handler_table[COMMAND_GLREADPIXELS] = 
        server_handle_glreadpixels;

    mutex_lock (name_mapping_mutex);
    if (name_mapping_buffer
        && name_mapping_renderbuffer
        && name_mapping_shader_object
        && name_mapping_texture
        && name_mapping_framebuffer) {
	mutex_unlock (name_mapping_mutex);
        return;
    }
    name_mapping_buffer = new_hash_table(free);
    name_mapping_renderbuffer = new_hash_table(free);
    name_mapping_shader_object = new_hash_table(free);
    name_mapping_texture = new_hash_table(free);
    name_mapping_framebuffer = new_hash_table(free);
    mutex_unlock (name_mapping_mutex);
}

bool
server_destroy (server_t *server)
{
    free (server);
    return true;
}

#include "server_autogen.c"
