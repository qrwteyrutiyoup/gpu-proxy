#ifndef GPUPROCESS_SYNC_QUEUE_H
#define GPUPROCESS_SYNC_QUEUE_H

#include "compiler_private.h"
#include "thread_private.h"
#include "types_private.h"

/***************************************************************
 * Functions for call orders.  We need to make sure the critical
 * egl/gl calls are kept in order. Any other egl/gl call is issued
 * immediately after these calls. These calls include:
 *
 * eglMakeCurrent
 * eglGetDisplay
 * eglInitialize,
 * eglCreateWindowSurface
 * eglCreatePbufferSurface
 * eglCreatePixmapSurface
 * eglBindAPI
 * eglCreatePixmapSurfaceHI
 * eglCreateContext
 * eglCreatePbufferFromClientBuffer
 * eglCreateImageKHR
 * eglCreateDRMImageMESA
 * eglLockSurfaceKHR
 * eglCreateSyncKHR
 * eglTerminate
 * eglWaitNative
 * eglWaitGL
 * eglWaitClient
 * eglSwapBuffers
 * eglReleaseThread
 * glFinish
 * glFlush
 *
 ***************************************************************/

typedef struct _server_log {
    thread_t            server;
} server_log_t;

private void
sync_queue_append_call_log (thread_t server);

private bool
sync_queue_allow_call (thread_t server);

private void
sync_queue_remove_call_log ();

#endif
