#ifndef GPUPROCESS_COMMAND_H
#define GPUPROCESS_COMMAND_H

#include "compiler_private.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct command {
    unsigned int id;
    size_t size;
} command_t;

typedef enum command_id {
    COMMAND_NO_OP,
    COMMAND_SET_TOKEN,

#include "generated/command_id_autogen.h"

    COMMAND_MAX_COMMAND
} command_id_t;

/* SetToken command, it writes a token in the command buffer, allowing
  the client to check when it is consumed in the server. */
typedef struct command_set_token {
    command_t header;
    unsigned int token;
} command_set_token_t;

typedef struct command_glcreateshader {
    command_t header;
    GLenum shader_type;
    GLuint result;
} command_glcreateshader_t;

private size_t
command_get_size_autogenerated (command_id_t command_id);

private size_t
command_get_size (command_id_t command_id);

private void
command_set_token_init (command_set_token_t *command,
                        unsigned int token);

private void
command_glcreateshader_init (command_t *abstract_command,
                             GLenum shader_type);

#include "generated/command_autogen.h"

#ifdef __cplusplus
}
#endif

#endif /* GPUPROCESS_COMMAND_H */
