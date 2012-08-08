#ifndef GLES2_STATE_H
#define GLES2_STATE_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_EMBEDDED 32

/* these are client state */
typedef struct v_vertex_attrib
{
    GLuint	index;
    GLint	array_buffer_binding;   /* initial is 0 */
    GLboolean   array_enabled;		/* initial GL_FALSE */
    GLint 	size;			/* initial 4 */
    GLint	stride;			/* initial 0 */
    GLenum	type;			/* initial GL_FLOAT */
    GLvoid	*pointer;		/* initial is 0 */
    GLboolean	array_normalized;	/* initial is GL_FALSE */
    GLfloat	current_attrib[4];	/* initial is (0, 0, 0, 1) */
} v_vertex_attrib_t;

typedef struct v_vertex_attrib_list
{
    int 		count;			/* initial 0 */
    v_vertex_attrib_t	embedded_attribs[NUM_EMBEDDED];
    v_vertex_attrib_t	*attribs;
} v_vertex_attrib_list_t;

typedef struct v_program_status {
    GLboolean	delete_status;
    GLboolean	link_status;
    GLboolean	validate_status;
    GLint	info_log_length; 		/* initial 0 */
    GLint	attached_shaders;
    GLint	active_attributes;
    GLint	active_attribute_max_length;	/* longest name + NULL */
    GLint	active_uniforms;
    GLint	active_uniform_max_length;	/* longest name + NULL */
} v_program_status_t;

/* this struct for attribute */
typedef struct v_program_attrib {
    GLuint	location;
    GLuint	index;
    GLchar 	*name;
} v_program_attrib_t;

typedef struct v_program_attrib_list {
    int 		count;
    v_program_attrib_t	*attribs;
} v_program_attrib_list_t;

typedef struct v_program_uniform {
    GLuint	location;
    GLchar	*name;
    GLchar 	*value;
    GLint	num_bytes;	/* number of bytes occupied by value */
} v_program_uniform_t;

typedef struct v_program_uniform_list {
    int			count;
    v_program_uniform_t *uniforms;
} v_program_uniform_list_t;

typedef struct v_program {
    GLuint 			program;
    v_program_status_t  	status;
    v_program_attrib_list_t 	attribs;
    v_program_uniform_list_t 	uniforms;
} v_program_t;

typedef struct gl_state {
    GLenum	error;			/* initial is GL_NO_ERROR */

/* GL states from glGet () */
    GLint	active_texture;			/* initial GL_TEXTURE0 */
    GLfloat	aliased_line_width_range[2];	/* must include 1 */
    GLfloat	aliased_point_size_range[2];	/* must include 1 */
    GLint	bits[4];			/* alpha, red, green and
						 * blue bits 
						 */	
    GLint	array_buffer_binding;		/* initial 0 */
    
    GLboolean	blend;				/* initial GL_FALSE */
    GLfloat	blend_color[4];			/* initial 0, 0, 0, 0 */
    GLfloat	blend_dst_alpha;		/* initial GL_ZERO */
    GLfloat 	blend_dst_rgb;			/* initial GL_ZERO */
    GLfloat	blend_src_alpha;		/* initial GL_ONE */
    GLfloat	blend_src_rgb;			/* initial GL_ONE */
    GLint	blend_equation[2];		/* 1 rgb, 2 alpha, initial
						 * GL_FUNC_ADD
						 */
    
    GLfloat     color_clear_value[4]; 		/* initial 0.0 */
    GLboolean	color_writemask[4];		/* initial all GL_TRUE */

    /* XXX: save GL_COMPRESSED_TEXTURE_FORMATS ? */
    GLboolean	cull_face;			/* initial GL_FALSE */
    GLint	cull_face_mode;			/* initial GL_BACK */

    GLint	current_program;		/* initial 0 */

    GLint	depth_bits;
    GLfloat	depth_clear_value;		/* initial 1 */
    GLint	depth_func;			/* initial GL_LESS */
    GLfloat	depth_range[2];			/* initial 0, 1 */
    GLboolean	depth_test;			/* initial GL_FALSE */
    GLboolean	depth_writemask;		/* initial GL_TRUE */

    GLboolean	dither;				/* intitial GL_TRUE */

    GLint	element_array_buffer_binding;	/* initial 0 */

    GLint 	framebuffer_binding;		/* initial 0 */

    GLint	front_face;			/* initial GL_CCW */

    GLint	generate_mipmap_hint;		/* initial GL_DONT_CARE */

    GLint	implementation_color_read_format;/* GL_UNSIGNED_BYTE is 
						  * always allowed 
						  */
    GLint	implementation_color_read_type;	/* GL_RGBA is always 
						 * allowed 
						 */
    GLfloat	line_width;			/* initial 1 */

    GLint	max_combined_texture_image_units;/* at least 8 */
    GLint	max_cube_map_texture_size;	/* at least 16 */
    GLint	max_fragment_uniform_vectors;	/* at least 16 */
    GLint	max_renderbuffer_size;		/* at least 1 */
    GLint	max_texture_image_units;	/* at least 8 */
    GLint	max_texture_size;		/* at least 64 */
    GLint	max_varying_vectors;		/* at least 8 */
    GLint	max_vertex_attribs;		/* at least 8 */
    GLint	max_vertex_texture_image_units;	/* may be 0 */
    GLint	max_viewport_dims;		/* as large as visible */

    GLint	num_compressed_texture_formats;	/* min is 0 */
    GLint	num_shader_binary_formats;	/* min is 0 */

    GLint	pack_alignment;			/* initial is 4 */
    GLint	unpack_alignment;		/* initial is 4 */

    GLfloat	polygon_offset_factor;		/* initial 0 */
    GLboolean 	polygon_offset_fill;		/* initial GL_FALSE */
    GLfloat	polygon_offset_units;		/* initial is 0 */   

    GLint	renderbuffer_binding;		/* initial 0 */
    
    GLboolean	sample_alpha_to_coverage;	/* initial GL_FALSE */
    GLint	sample_buffers;	
    GLboolean	sample_coverage;		/* initial GL_FALSE */
    GLboolean	sample_coverage_invert;		/* initial ? */
    GLfloat	sample_coverage_value;		/* positive float */
    GLint	samples;

    GLint	scissor_box[4];			/* initial (0, 0, 0, 0) */
    GLboolean	scissor_test;			/* initial GL_FALSE */
    
    GLint	shader_binary_formats;	
    GLboolean	shader_compiler;		

    GLint	stencil_back_fail;		/* initial GL_KEEP */
    GLint	stencil_back_func;		/* initial GL_ALWAYS */
    GLint 	stencil_back_pass_depth_fail;	/* initial GL_KEEP */
    GLint	stencil_back_pass_depth_pass;	/* initial GL_KEEP */
    GLint	stencil_back_ref;		/* initial is 0 */
    GLint	stencil_back_value_mask;	/* initial 0xffffffff */
    GLint	stencil_bits;
    GLint	stencil_clear_value;		/* initial 0 */
    GLint	stencil_fail;			/* initial GL_KEEP */
    GLint	stencil_func;			/* initial GL_ALWAYS */
    GLint	stencil_pass_depth_fail;	/* initial GL_KEEP */
    GLint	stencil_pass_depth_pass;	/* initial GL_KEEP */
    GLint	stencil_ref;			/* initial is 0 */
    GLboolean	stencil_test;			/* intitial GL_FALSE */
    GLint	stencil_value_mask;		/* initial 0xffffffff */
    GLint	stencil_writemask;		/* initial 0xffffffff */
    
    GLint	subpixel_bits;			/* at least 4 */
    
    GLint	texture_binding[2];		/* 2D, cube_map, initial 0 */
    GLint	viewport[4];			/* initial (0, 0, 0, 0) */
    
    /* glGetString () */
    GLubyte 	vendor[256];
    GLubyte	renderer[256];
    GLubyte	extensions[4096]; 	/* too large or short? */
    GLubyte	shading_language_version[128];

    /* glGetTextureParameter() */
    GLint	texture_mag_filter[32][2];	/* initial GL_LINEAR */
    GLint	texture_min_filter[32][2];	/* initial GL_NEAREST_MIPMAP_LINEAR */
    GLint	texture_wrap_s[32][2];		/* initial GL_REPEAT */
    GLint	texture_wrap_t[32][2];		/* initial GL_REPEAT */

    /* glGetBufferParameter() */
    GLint	buffer_size[2];			/* initial 0 */	
    GLint	buffer_usage[2];		/* initial GL_STATIC_DRAW */

    /* glGetFramebufferAttachmentParameter() */
    /* XXX: do we need to cache them? */
    
    /* glGetRenderbufferParameter() */
    /* XXX: do we need to cache them ? */

#ifdef GL_OES_compressed_ETC1_RGB8_texture
#endif

#ifdef GL_OES_compressed_paletted_texture
#endif

#ifdef GL_OES_depth24
#endif

#ifdef GL_OES_depth32
#endif

#ifdef GL_OES_depth_texture
#endif

#ifdef GL_OES_EGL_image
#endif

#ifdef GL_OES_EGL_image_external
#endif

#ifdef GL_OES_element_index_uint
#endif

#ifdef GL_OES_get_program_binary
    /* reuse num_program_binary_formats, program_binary_formats */
#endif 

#ifdef GL_OES_mapbuffer
    /* XXX: anything worth to save ? */
#endif

#ifdef GL_OES_packed_depth_stencil
#endif

#ifdef GL_OES_rgb8_rgba8
#endif

#ifdef GL_OES_standard_derivatives
#endif

#ifdef GL_OES_stencil1
#endif

#ifdef GL_OES_stencil4
#endif

#ifdef GL_OES_texture_3D
    GLint	texture_binding_3d;
    GLint	max_3d_texture_size;
    GLint	texture_3d_wrap[3];
    GLint	texture_3d_min_filter;
    GLint	texture_3d_mag_filter;
#endif

#ifdef GL_OES_texture_float
#endif

#ifdef GL_OES_texture_float_linear
#endif

#ifdef GL_OES_texture_half_float
#endif

#ifdef GL_OES_texture_half_float_linear
#endif

#ifdef GL_OES_texture_npot
#endif

#ifdef GL_OES_vertex_array_object
    GLint	vertex_array_binding;
#endif

#ifdef GL_OES_vertex_half_float
#endif

#ifdef GL_OES_vertex_type_10_10_10_2
#endif

#ifdef GL_AMD_compressed_3DC_texture
#endif

#ifdef GL_AMD_compressed_ATC_texture
#endif

#ifdef GL_AMD_performance_monitor
#endif

#ifdef GL_AMD_program_binary_Z400
#endif

#ifdef GL_ANGLE_framebuffer_blit
    GLint	draw_framebuffer_binding;		/* initial 0 ? */
    GLint	read_framebuffer_binding;		/* initial 0 ? */
#endif

#ifdef GL_ANGLE_framebuffer_multisample
    GLint	max_samples;
#endif

#ifdef GL_APPLE_rgb_422
#endif

#ifdef GL_APPLE_framebuffer_multisample
	/* reuse GL_ANGLE_framebuffer_blit */
#endif

#ifdef GL_APPLE_texture_format_BGRA8888
#endif

#ifdef GL_APPLE_texture_max_level
    GLint	texture_max_level;
#endif

#ifdef GL_ARM_mali_shader_bindary
#endif

#ifdef GL_ARM_rgba8
#endif

#ifdef GL_EXT_blend_minmax
    /* reuse blend_equation */
#endif

#ifdef GL_EXT_discard_framebuffer
#endif

#ifdef GL_EXT_multi_draw_arrays
#endif

#ifdef GL_EXT_read_format_bgra
#endif

#ifdef GL_EXT_texture_filter_anisotropic
#endif

#ifdef GL_EXT_texture_format_BGRA8888
#endif

#ifdef GL_EXT_texture_compression_dxt1
#endif

#ifdef GL_IMG_program_binary
#endif

#ifdef GL_IMG_read_format
#endif

#ifdef GL_IMG_shader_binary
#endif

#ifdef GL_IMG_texture_compression_pvrtc
#endif

#ifdef GL_IMG_multisampled_render_to_texture
    /* reuse max_samples */
#endif

#ifdef GL_NV_fence
#endif

#ifdef GL_NV_coverage_sample
#endif

#ifdef GL_NV_depth_nonlinear
#endif

#ifdef GL_QCOM_extended_get
#endif

#ifdef GL_QCOM_extended_get2
#endif

#ifdef GL_QCOM_perfmon_global_mode
#endif

#ifdef GL_QCOM_writeonly_rendering
#endif

#ifdef GL_QCOM_tiled_rendering
#endif

#ifdef GL_VIV_shader_bindary
#endif

#ifdef GL_EXT_multisampled_render_to_texture
    /* re-use max_samples */
#endif

} gles2_state_t;
#ifdef __cplusplus
}
#endif

#endif /* GLES2_STATE_H */
