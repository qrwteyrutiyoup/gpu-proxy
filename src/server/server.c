#include "config.h"
#include "server.h"

#include "ring_buffer.h"
#include "dispatch_table.h"
#include "thread_private.h"
#include "server_state.h"
#include "sync_queue.h"
#include <time.h>
#include <X11/Xlib.h>

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

        sem_post (server->server_read_signal);
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
server_handle_glgenbuffers (server_t *server,
                                   command_t *abstract_command)
{
    INSTRUMENT();

    sync_queue_allow_call (abstract_command, server->thread);

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

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_gldeletebuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    sync_queue_allow_call (abstract_command, server->thread);

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

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_glgenframebuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    sync_queue_allow_call (abstract_command, server->thread);

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

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_gldeleteframebuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    sync_queue_allow_call (abstract_command, server->thread);

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

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_glgentextures (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    sync_queue_allow_call (abstract_command, server->thread);

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

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_gldeletetextures (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    sync_queue_allow_call (abstract_command, server->thread);

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

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_glgenrenderbuffers (server_t *server,
                                         command_t *abstract_command)
{
    INSTRUMENT();

    sync_queue_allow_call (abstract_command, server->thread);

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

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_gldeleterenderbuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT();

    sync_queue_allow_call (abstract_command, server->thread);


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

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_glcreateprogram (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_glcreateprogram_t *command =
            (command_glcreateprogram_t *)abstract_command;
    GLuint *program = (GLuint *)malloc (sizeof (GLuint));

    *program = server->dispatch.glCreateProgram (server);

    mutex_lock (name_mapping_mutex);
    hash_insert (name_mapping_shader_object, command->result, program);
    mutex_unlock (name_mapping_mutex);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_gldeleteprogram (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_gldeleteprogram_t *command =
            (command_gldeleteprogram_t *)abstract_command;

    mutex_lock (name_mapping_mutex);
    GLuint *program = hash_take (name_mapping_shader_object, command->program);
    mutex_unlock (name_mapping_mutex);

    if (program)
        server->dispatch.glDeleteProgram (server, *program);

    command_gldeleteprogram_destroy_arguments (command);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_glcreateshader (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_glcreateshader_t *command =
            (command_glcreateshader_t *)abstract_command;
    GLuint *shader = (GLuint *)malloc (sizeof (GLuint));

    *shader = server->dispatch.glCreateShader (server, command->type);

    mutex_lock (name_mapping_mutex);
    hash_insert (name_mapping_shader_object, command->result, shader);
    mutex_unlock (name_mapping_mutex);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_gldeleteshader (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_gldeleteshader_t *command =
            (command_gldeleteshader_t *)abstract_command;

    mutex_lock (name_mapping_mutex);
    GLuint *shader = hash_take (name_mapping_shader_object, command->shader);
    mutex_unlock (name_mapping_mutex);

    if (shader)
        server->dispatch.glDeleteShader (server, *shader);
    else
        /*XXX: This call should return INVALID_VALUE */
        server->dispatch.glDeleteShader (server, 0xffffffff);

    command_gldeleteshader_destroy_arguments (command);

    sync_queue_remove_call_log (abstract_command);
}

/* server handles in order requests */
static void
server_handle_eglterminate (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglterminate_t *command =
        (command_eglterminate_t *)abstract_command;

    command->result = server->dispatch.eglTerminate (server, command->dpy);
    /* call server state */
    if (command->result == EGL_TRUE)
        _server_display_remove (server->egl_display);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglreleasethread (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglreleasethread_t *command =
        (command_eglreleasethread_t *) abstract_command;

    command->result = server->dispatch.eglReleaseThread (server);

    if (command->result == EGL_TRUE)
        _server_display_remove (server->egl_display);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglmakecurrent (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    command_eglmakecurrent_t *command =
       (command_eglmakecurrent_t *)abstract_command;

    sync_queue_allow_call (abstract_command, server->thread);

    command->result = server->dispatch.eglMakeCurrent (server, command->dpy,
                                                       command->draw,
                                                       command->read,
                                                       command->ctx);
    
    if (command->result == EGL_TRUE) {
        _server_display_reference (command->dpy);
        server->egl_display = command->dpy;
        _server_display_remove (server->egl_display);
    }
    

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_glflush (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    server->dispatch.glFlush (server);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_glfinish (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    server->dispatch.glFinish (server);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglwaitnative (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglwaitnative_t *command =
        (command_eglwaitnative_t *) abstract_command;

    command->result = server->dispatch.eglWaitNative (server, command->engine);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglwaitgl (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglwaitgl_t *command =
        (command_eglwaitgl_t *) abstract_command;

    command->result = server->dispatch.eglWaitGL (server);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglwaitclient (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglwaitclient_t *command =
        (command_eglwaitclient_t *) abstract_command;

    command->result = server->dispatch.eglWaitClient (server);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglswapbuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglswapbuffers_t *command =
        (command_eglswapbuffers_t *) abstract_command;

    command->result = server->dispatch.eglSwapBuffers (server, command->dpy,                                                       command->surface);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglgetdisplay (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglgetdisplay_t *command =
            (command_eglgetdisplay_t *)abstract_command;

    list_node_t *head = _server_displays ()->head;

    while (head) {
        server_display_list_t *display = (server_display_list_t *)head->data;
        if (display->client_display == command->display_id) {
            command->result = display->egl_display;
            goto FINISH;
        }
        head = head->next;
    }

    Display *client_display = command->display_id;
    Display *server_display = XOpenDisplay (DisplayString(client_display));
    if (! server_display) {
        command->result = EGL_NO_DISPLAY;
        goto FINISH;
    }

    command->display_id = server_display;
    command->result = server->dispatch.eglGetDisplay (server, command->display_id);
    
    if (command->result != EGL_NO_DISPLAY) 
        _server_display_add (server_display, client_display, command->result);
    else
        XCloseDisplay (server_display);

FINISH:
    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglinitialize (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglinitialize_t *command =
            (command_eglinitialize_t *)abstract_command;
    command->result = server->dispatch.eglInitialize (server, command->dpy, command->major, command->minor);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcreatewindowsurface (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcreatewindowsurface_t *command =
            (command_eglcreatewindowsurface_t *)abstract_command;
    command->result = server->dispatch.eglCreateWindowSurface (server, command->dpy, command->config, command->win, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcreatepbuffersurface (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcreatepbuffersurface_t *command =
            (command_eglcreatepbuffersurface_t *)abstract_command;
    command->result = server->dispatch.eglCreatePbufferSurface (server, command->dpy, command->config, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcreatepixmapsurface (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcreatepixmapsurface_t *command =
            (command_eglcreatepixmapsurface_t *)abstract_command;
    command->result = server->dispatch.eglCreatePixmapSurface (server, command->dpy, command->config, command->pixmap, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_egldestroysurface (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_egldestroysurface_t *command =
            (command_egldestroysurface_t *)abstract_command;
    command->result = server->dispatch.eglDestroySurface (server, command->dpy, command->surface);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglbindapi (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglbindapi_t *command =
            (command_eglbindapi_t *)abstract_command;
    command->result = server->dispatch.eglBindAPI (server, command->api);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcreatepbufferfromclientbuffer (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcreatepbufferfromclientbuffer_t *command =
            (command_eglcreatepbufferfromclientbuffer_t *)abstract_command;
    command->result = server->dispatch.eglCreatePbufferFromClientBuffer (server, command->dpy, command->buftype, command->buffer, command->config, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglbindteximage (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglbindteximage_t *command =
            (command_eglbindteximage_t *)abstract_command;
    command->result = server->dispatch.eglBindTexImage (server, command->dpy, command->surface, command->buffer);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglreleaseteximage (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglreleaseteximage_t *command =
            (command_eglreleaseteximage_t *)abstract_command;
    command->result = server->dispatch.eglReleaseTexImage (server, command->dpy, command->surface, command->buffer);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcreatecontext (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcreatecontext_t *command =
            (command_eglcreatecontext_t *)abstract_command;
    command->result = server->dispatch.eglCreateContext (server, command->dpy, command->config, command->share_context, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_egldestroycontext (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_egldestroycontext_t *command =
            (command_egldestroycontext_t *)abstract_command;
    command->result = server->dispatch.eglDestroyContext (server, command->dpy, command->ctx);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcopybuffers (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcopybuffers_t *command =
            (command_eglcopybuffers_t *)abstract_command;
    command->result = server->dispatch.eglCopyBuffers (server, command->dpy, command->surface, command->target);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_egllocksurfacekhr (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_egllocksurfacekhr_t *command =
            (command_egllocksurfacekhr_t *)abstract_command;
    command->result = server->dispatch.eglLockSurfaceKHR (server, command->display, command->surface, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglunlocksurfacekhr (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglunlocksurfacekhr_t *command =
            (command_eglunlocksurfacekhr_t *)abstract_command;
    command->result = server->dispatch.eglUnlockSurfaceKHR (server, command->display, command->surface);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcreateimagekhr (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcreateimagekhr_t *command =
            (command_eglcreateimagekhr_t *)abstract_command;
    command->result = server->dispatch.eglCreateImageKHR (server, command->dpy, command->ctx, command->target, command->buffer, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_egldestroyimagekhr (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_egldestroyimagekhr_t *command =
            (command_egldestroyimagekhr_t *)abstract_command;
    command->result = server->dispatch.eglDestroyImageKHR (server, command->dpy, command->image);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcreatesynckhr (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcreatesynckhr_t *command =
            (command_eglcreatesynckhr_t *)abstract_command;
    command->result = server->dispatch.eglCreateSyncKHR (server, command->dpy, command->type, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_egldestroysynckhr (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_egldestroysynckhr_t *command =
            (command_egldestroysynckhr_t *)abstract_command;
    command->result = server->dispatch.eglDestroySyncKHR (server, command->dpy, command->sync);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglclientwaitsynckhr (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglclientwaitsynckhr_t *command =
            (command_eglclientwaitsynckhr_t *)abstract_command;
    command->result = server->dispatch.eglClientWaitSyncKHR (server, command->dpy, command->sync, command->flags, command->timeout);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglsignalsynckhr (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglsignalsynckhr_t *command =
            (command_eglsignalsynckhr_t *)abstract_command;
    command->result = server->dispatch.eglSignalSyncKHR (server, command->dpy, command->sync, command->mode);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglcreatedrmimagemesa (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglcreatedrmimagemesa_t *command =
            (command_eglcreatedrmimagemesa_t *)abstract_command;
    command->result = server->dispatch.eglCreateDRMImageMESA (server, command->dpy, command->attrib_list);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglexportdrmimagemesa (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglexportdrmimagemesa_t *command =
            (command_eglexportdrmimagemesa_t *)abstract_command;
    command->result = server->dispatch.eglExportDRMImageMESA (server, command->dpy, command->image, command->name, command->handle, command->stride);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglmapimagesec (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglmapimagesec_t *command =
            (command_eglmapimagesec_t *)abstract_command;
    command->result = server->dispatch.eglMapImageSEC (server, command->display, command->image);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_eglunmapimagesec (server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_eglunmapimagesec_t *command =
            (command_eglunmapimagesec_t *)abstract_command;
    command->result = server->dispatch.eglUnmapImageSEC (server, command->display, command->image);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_gleglimagetargettexture2does (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_gleglimagetargettexture2does_t *command =
            (command_gleglimagetargettexture2does_t *)abstract_command;
    server->dispatch.glEGLImageTargetTexture2DOES (server, command->target, command->image);
    command_gleglimagetargettexture2does_destroy_arguments (command);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_gleglimagetargetrenderbufferstorageoes (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_gleglimagetargetrenderbufferstorageoes_t *command =
            (command_gleglimagetargetrenderbufferstorageoes_t *)abstract_command;
    server->dispatch.glEGLImageTargetRenderbufferStorageOES (server, command->target, command->image);
    command_gleglimagetargetrenderbufferstorageoes_destroy_arguments (command);

    sync_queue_remove_call_log (abstract_command);
}

static void
server_handle_glmapbufferoes (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_glmapbufferoes_t *command =
            (command_glmapbufferoes_t *)abstract_command;
    command->result = server->dispatch.glMapBufferOES (server, command->target, command->access);

    sync_queue_remove_call_log (abstract_command);
}

static void 
server_handle_glunmapbufferoes (
    server_t *server, command_t *abstract_command)
{
    INSTRUMENT ();

    sync_queue_allow_call (abstract_command, server->thread);

    command_glunmapbufferoes_t *command =
            (command_glunmapbufferoes_t *)abstract_command;
    command->result = server->dispatch.glUnmapBufferOES (server, command->target);

    sync_queue_remove_call_log (abstract_command);
}

void
server_init (server_t *server,
             buffer_t *buffer)
{
    server->egl_display = EGL_NO_DISPLAY;
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

    server->handler_table[COMMAND_EGLTERMINATE] = 
        server_handle_eglterminate;
    server->handler_table[COMMAND_EGLRELEASETHREAD] =
        server_handle_eglreleasethread;
    server->handler_table[COMMAND_EGLMAKECURRENT] =
        server_handle_eglmakecurrent;
    server->handler_table[COMMAND_GLFLUSH] =
        server_handle_glflush;
    server->handler_table[COMMAND_GLFINISH] =
        server_handle_glfinish;
    server->handler_table[COMMAND_EGLWAITNATIVE] =
        server_handle_eglwaitnative;
    server->handler_table[COMMAND_EGLWAITGL] =
        server_handle_eglwaitgl;
    server->handler_table[COMMAND_EGLWAITCLIENT] =
        server_handle_eglwaitclient;
    server->handler_table[COMMAND_EGLSWAPBUFFERS] =
        server_handle_eglswapbuffers;
    server->handler_table[COMMAND_EGLGETDISPLAY] =
        server_handle_eglgetdisplay;
    server->handler_table[COMMAND_EGLINITIALIZE] =
        server_handle_eglinitialize;
    server->handler_table[COMMAND_EGLCREATEWINDOWSURFACE] =
        server_handle_eglcreatewindowsurface;
    server->handler_table[COMMAND_EGLCREATEPBUFFERSURFACE] =
        server_handle_eglcreatepbuffersurface;
    server->handler_table[COMMAND_EGLCREATEPIXMAPSURFACE] =
        server_handle_eglcreatepixmapsurface;
    server->handler_table[COMMAND_EGLDESTROYSURFACE] =
        server_handle_egldestroysurface;
    server->handler_table[COMMAND_EGLBINDAPI] =
        server_handle_eglbindapi;
    server->handler_table[COMMAND_EGLCREATEPBUFFERFROMCLIENTBUFFER] =
        server_handle_eglcreatepbufferfromclientbuffer;
    server->handler_table[COMMAND_EGLBINDTEXIMAGE] =
        server_handle_eglbindteximage;
    server->handler_table[COMMAND_EGLRELEASETEXIMAGE] =
        server_handle_eglreleaseteximage;
    server->handler_table[COMMAND_EGLCREATECONTEXT] =
        server_handle_eglcreatecontext;
    server->handler_table[COMMAND_EGLDESTROYCONTEXT] =
        server_handle_egldestroycontext;
    server->handler_table[COMMAND_EGLCOPYBUFFERS] =
        server_handle_eglcopybuffers;
    server->handler_table[COMMAND_EGLLOCKSURFACEKHR] =
        server_handle_egllocksurfacekhr;
    server->handler_table[COMMAND_EGLUNLOCKSURFACEKHR] =
        server_handle_eglunlocksurfacekhr;
    server->handler_table[COMMAND_EGLCREATEIMAGEKHR] =
        server_handle_eglcreateimagekhr;
    server->handler_table[COMMAND_EGLDESTROYIMAGEKHR] =
        server_handle_egldestroyimagekhr;
    server->handler_table[COMMAND_EGLCREATESYNCKHR] =
        server_handle_eglcreatesynckhr;
    server->handler_table[COMMAND_EGLDESTROYSYNCKHR] =
        server_handle_egldestroysynckhr;
    server->handler_table[COMMAND_EGLCLIENTWAITSYNCKHR] =
        server_handle_eglclientwaitsynckhr;
    server->handler_table[COMMAND_EGLSIGNALSYNCKHR] =
        server_handle_eglsignalsynckhr;
    server->handler_table[COMMAND_EGLCREATEDRMIMAGEMESA] =
        server_handle_eglcreatedrmimagemesa;
    server->handler_table[COMMAND_EGLEXPORTDRMIMAGEMESA] =
        server_handle_eglexportdrmimagemesa;
    server->handler_table[COMMAND_EGLMAPIMAGESEC] =
        server_handle_eglmapimagesec;
    server->handler_table[COMMAND_EGLUNMAPIMAGESEC] =
        server_handle_eglunmapimagesec;
    server->handler_table[COMMAND_GLEGLIMAGETARGETTEXTURE2DOES] =
        server_handle_gleglimagetargettexture2does;
    server->handler_table[COMMAND_GLEGLIMAGETARGETRENDERBUFFERSTORAGEOES] =
        server_handle_gleglimagetargetrenderbufferstorageoes;

    server->thread = pthread_self ();
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
