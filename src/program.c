#include "config.h"
#include "program.h"
#include <stdlib.h>

program_t*
program_new (GLuint id)
{
    program_t *new_program = (program_t *)malloc (sizeof (program_t));
    new_program->base.id = id;
    new_program->base.type = SHADER_OBJECT_PROGRAM;
    new_program->mark_for_deletion = false;
    new_program->uniform_location_cache = id ? new_hash_table(free) : NULL;
    new_program->attrib_location_cache = id ? new_hash_table(free) : NULL;
    new_program->location_cache = id ? new_hash_table(free) : NULL;
    new_program->attached_shaders.head = NULL;
    new_program->attached_shaders.tail = NULL;
    return new_program;
}

void
program_destroy (void *abstract_program)
{
    shader_object_t *shader_object = abstract_program;

    if (shader_object->type == SHADER_OBJECT_SHADER) {
        free (shader_object);
        return;
    }

    program_t *program = abstract_program;
    delete_hash_table (program->attrib_location_cache);
    delete_hash_table (program->uniform_location_cache);
    delete_hash_table (program->location_cache);
    link_list_clear (&program->attached_shaders);
}
