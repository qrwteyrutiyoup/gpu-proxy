#ifndef GPUPROCESS_DISPATCH_PRIVATE_H
#define GPUPROCESS_DISPATCH_PRIVATE_H

#include "config.h"

#include "compiler_private.h"

#ifdef HAS_GL
#include <GL/glx.h>
#endif

#ifdef HAS_GLES2
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

typedef void (*FunctionPointerType)(void);

typedef struct _gpuprocess_dispatch {
#ifdef HAS_GL
/* GLX */
    XVisualInfo* (*ChooseVisual) (Display *dpy, int screen,
                                  int *attribList);
    GLXContext (*CreateContext) (Display *dpy, XVisualInfo *vis,
                                 GLXContext shareList, Bool direct);
    void (*DestroyContext) (Display *dpy, GLXContext ctx);
    Bool (*MakeCurrent) (Display *dpy, GLXDrawable drawable,
                         GLXContext ctx);
    void (*CopyContext) (Display *dpy, GLXContext src, GLXContext dst,
                         unsigned long mask);
    void (*SwapBuffers) (Display *dpy, GLXDrawable drawable);
    GLXPixmap (*CreateGLXPixmap) (Display *dpy, XVisualInfo *visual,
                                  Pixmap pixmap);
    void (*DestroyGLXPixmap) (Display *dpy, GLXPixmap pixmap);
    Bool (*QueryExtension) (Display *dpy, int *errorb, int *event);
    Bool (*QueryVersion) (Display *dpy, int *maj, int *min);
    Bool (*IsDirect) (Display *dpy, GLXContext ctx);
    int (*GetConfig) (Display *dpy, XVisualInfo *visual,
                      int attrib, int *value);
    GLXContext (*GetCurrentContext) (void);
    GLXDrawable (*GetCurrentDrawable) (void);
    void (*WaitGL) (void);
    void (*WaitX) (void);
    void (*UseXFont) (Font font, int first, int count, int list);

/* GLX 1.1 and later */
    const char * (*QueryExtensionsString) (Display *dpy, int screen);
    const char * (*QueryServerString) (Display *dpy, int screen, int name);
    const char * (*GetClientString) (Display *dpy, int name);

/* GLX 1.2 and later */
    Display * (*GetCurrentDisplay) (void);


/* GLX 1.3 and later */
    GLXFBConfig * (*ChooseFBConfig) (Display *dpy, int screen,
                                     const int *attribList, int *nitems);
    int (*GetFBConfigAttrib) (Display *dpy, GLXFBConfig config,
                              int attribute, int *value);
    GLXFBConfig * (*GetFBConfigs) (Display *dpy, int screen,
                                   int *nelements);
    XVisualInfo * (*GetVisualFromFBConfig) (Display *dpy,
                                            GLXFBConfig config);
    GLXWindow (*CreateWindow) (Display *dpy, GLXFBConfig config,
                               Window win, const int *attribList);
    void (*DestroyWindow) (Display *dpy, GLXWindow window);
    GLXPixmap (*CreatePixmap) (Display *dpy, GLXFBConfig config,
                               Pixmap pixmap, const int *attribList);
    void (*DestroyPixmap) (Display *dpy, GLXPixmap pixmap);
    GLXPbuffer (*CreatePbuffer) (Display *dpy, GLXFBConfig config,
                                 const int *attribList);
    void (*DestroyPbuffer) (Display *dpy, GLXPbuffer pbuf);
    void (*QueryDrawable) (Display *dpy, GLXDrawable draw, int attribute,
                           unsigned int *value);
    GLXContext (*CreateNewContext) (Display *dpy, GLXFBConfig config,
                                    int renderType, GLXContext shareList,
                                    Bool direct);
    Bool (*MakeContextCurrent) (Display *dpy, GLXDrawable draw,
                                GLXDrawable read, GLXContext ctx);
    GLXDrawable (*GetCurrentReadDrawable) (void);
    int (*QueryContext) (Display *dpy, GLXContext ctx, int attribute,
                         int *value);
    void (*SelectEvent) (Display *dpy, GLXDrawable drawable,
                         unsigned long mask);
    void (*GetSelectedEvent) (Display *dpy, GLXDrawable drawable,
                              unsigned long *mask);

#ifdef GLX_EXT_texture_from_pixmap
    void (*BindTexImageEXT) (Display *display, GLXDrawable drawable, int buffer, const int *attrib_list);
    void (*ReleaseTexImageEXT) (Display *display, GLXDrawable drawable, int buffer);
#endif

#endif // HAS_GL

#ifdef HAS_GLES2
    EGLint (*eglGetError) (void);

    EGLDisplay (*eglGetDisplay) (EGLNativeDisplayType);
    EGLBoolean (*eglInitialize) (EGLDisplay, EGLint *, EGLint *);
    EGLBoolean (*eglTerminate) (EGLDisplay);

    const char * (*eglQueryString) (EGLDisplay, EGLint);

    EGLBoolean (*eglGetConfigs) (EGLDisplay, EGLConfig *,
                                 EGLint, EGLint *);
    EGLBoolean (*eglChooseConfig) (EGLDisplay, const EGLint *,
                                   EGLConfig *, EGLint,
                                   EGLint *);
    EGLBoolean (*eglGetConfigAttrib) (EGLDisplay, EGLConfig,
                                      EGLint, EGLint *);

    EGLSurface (*eglCreateWindowSurface) (EGLDisplay, EGLConfig,
                                          EGLNativeWindowType,
                                          const EGLint *);
    EGLSurface (*eglCreatePbufferSurface) (EGLDisplay, EGLConfig,
                                           const EGLint *);
    EGLSurface (*eglCreatePixmapSurface) (EGLDisplay, EGLConfig,
                                          EGLNativePixmapType,
                                          const EGLint *);
    EGLBoolean (*eglDestroySurface) (EGLDisplay, EGLSurface);
    EGLBoolean (*eglQuerySurface) (EGLDisplay, EGLSurface,
                                   EGLint, EGLint *);

    EGLBoolean (*eglBindAPI) (EGLenum);
    EGLenum (*eglQueryAPI) (void);

    EGLBoolean (*eglWaitClient) (void);

    EGLBoolean (*eglReleaseThread) (void);

    EGLSurface (*eglCreatePbufferFromClientBuffer) (
        EGLDisplay, EGLenum, EGLClientBuffer,
        EGLConfig, const EGLint *);

    EGLBoolean (*eglSurfaceAttrib) (EGLDisplay, EGLSurface,
                                    EGLint, EGLint);
    EGLBoolean (*eglBindTexImage) (EGLDisplay, EGLSurface, EGLint);
    EGLBoolean (*eglReleaseTexImage) (EGLDisplay, EGLSurface, EGLint);

    EGLBoolean (*eglSwapInterval) (EGLDisplay, EGLint);

    EGLContext (*eglCreateContext) (EGLDisplay, EGLConfig,
                                    EGLContext,
                                    const EGLint *);
    EGLBoolean (*eglDestroyContext) (EGLDisplay, EGLContext);
    EGLBoolean (*eglMakeCurrent) (EGLDisplay, EGLSurface,
                                  EGLSurface, EGLContext);

    EGLContext (*eglGetCurrentContext) (void);
    EGLSurface (*eglGetCurrentSurface) (EGLint);
    EGLDisplay (*eglGetCurrentDisplay) (void);
    EGLBoolean (*eglQueryContext) (EGLDisplay, EGLContext,
                                   EGLint, EGLint *);

    EGLBoolean (*eglWaitGL) (void);
    EGLBoolean (*eglWaitNative) (EGLint engine);
    EGLBoolean (*eglSwapBuffers) (EGLDisplay, EGLSurface);
    EGLBoolean (*eglCopyBuffers) (EGLDisplay, EGLSurface,
                                  EGLNativePixmapType);

#ifdef EGL_KHR_lock_surface
    EGLBoolean (*eglLockSurfaceKHR) (EGLDisplay, EGLSurface, const EGLint *);
    EGLBoolean (*eglUnlockSurfaceKHR) (EGLDisplay, EGLSurface);
#endif

#ifdef EGL_KHR_image
    EGLImageKHR (*eglCreateImageKHR) (EGLDisplay, EGLContext, EGLenum, EGLClientBuffer, const EGLint *);
    EGLBoolean (*eglDestroyImageKHR) (EGLDisplay, EGLImageKHR);
#endif

#ifdef EGL_KHR_reusable_sync
    EGLSyncKHR (*eglCreateSyncKHR) (EGLDisplay, EGLenum, const EGLint *);
    EGLBoolean (*eglDestroySyncKHR) (EGLDisplay, EGLSyncKHR);
    EGLint (*eglClientWaitSyncKHR) (EGLDisplay, EGLSyncKHR, EGLint, EGLTimeKHR);
    EGLBoolean (*eglSignalSyncKHR) (EGLDisplay, EGLSyncKHR, EGLenum);
    EGLBoolean (*eglGetSyncAttribKHR) (EGLDisplay, EGLSyncKHR, EGLint, EGLint *);
#endif

#ifdef EGL_NV_sync
    EGLSyncNV (*eglCreateFenceSyncNV) (EGLDisplay, EGLenum, const EGLint *);
    EGLBoolean (*eglDestroySyncNV) (EGLSyncNV);
    EGLBoolean (*eglFenceNV) (EGLSyncNV sync);
    EGLint (*eglClientWaitSyncNV) (EGLSyncNV sync, EGLint flags, EGLTimeNV);
    EGLBoolean (*eglSignalSyncNV) (EGLSyncNV sync, EGLenum mode);
    EGLBoolean (*eglGetSyncAttribNV) (EGLSyncNV sync, EGLint attribute, EGLint *value);
#endif

#ifdef EGL_HI_clientpixmap
    EGLSurface (*eglCreatePixmapSurfaceHI) (EGLDisplay, EGLConfig, struct EGLClientPixmapHI *);
#endif

#ifdef EGL_MESA_drm_image
    EGLImageKHR (*eglCreateDRMImageMESA) (EGLDisplay, const EGLint *);
    EGLBoolean (*eglExportDRMImageMESA) (EGLDisplay, EGLImageKHR, EGLint *, EGLint *, EGLint *);
#endif

#ifdef EGL_POST_SUB_BUFFER_SUPPORTED_NV
    EGLBoolean (*eglPostSubBufferNV) (EGLDisplay, EGLSurface, EGLint, EGLint, EGLint, EGLint);
#endif

#ifdef EGL_SEC_image_map
    void * (*eglMapImageSEC) (EGLDisplay, EGLImageKHR);
    EGLBoolean (*eglUnmapImageSEC) (EGLDisplay, EGLImageKHR);
    EGLBoolean (*eglGetImageAttribSEC) (EGLDisplay, EGLImageKHR, EGLint, EGLint *);
#endif

#ifdef GL_OES_EGL_image
    void (*EGLImageTargetTexture2DOES) (GLenum, GLeglImageOES);
    void (*EGLImageTargetRenderbufferStorageOES) (GLenum, GLeglImageOES);
#endif

#ifdef GL_OES_get_program_binary
    void (*GetProgramBinaryOES) (GLuint, GLsizei, GLsizei *, GLenum *, GLvoid *);
    void (*ProgramBinaryOES) (GLuint, GLenum, const GLvoid *, GLint);
#endif

#ifdef GL_OES_mapbuffer
    void* (*MapBufferOES) (GLenum, GLenum);
    GLboolean (*UnmapBufferOES) (GLenum);
    void (*GetBufferPointervOES) (GLenum, GLenum, GLvoid**);
#endif

#ifdef GL_OES_texture_3D
    void (*TexImage3DOES) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint,
                           GLenum, GLenum, const GLvoid*);
    void (*TexSubImage3DOES) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei,
                              GLsizei, GLenum, GLenum, const GLvoid*);
    void (*CopyTexSubImage3DOES) (GLenum, GLint, GLint, GLint, GLint, GLint, GLint,
                                  GLsizei, GLsizei height);
    void (*CompressedTexImage3DOES) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei,
                                     GLint, GLsizei, const GLvoid*);
    void (*CompressedTexSubImage3DOES) (GLenum, GLint, GLint, GLint, GLint, GLsizei,
                                        GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*);
    void (*FramebufferTexture3DOES) (GLenum, GLenum, GLenum, GLuint, GLint, GLint);
#endif

#ifdef GL_OES_vertex_array_object
    void (*BindVertexArrayOES) (GLuint);
    void (*DeleteVertexArraysOES) (GLsizei, const GLuint *);
    void (*GenVertexArraysOES) (GLsizei, GLuint *);
    GLboolean (*IsVertexArrayOES) (GLuint);
#endif

#ifdef GL_AMD_performance_monitor
    void (*GetPerfMonitorGroupsAMD) (GLint *, GLsizei, GLuint *);
    void (*GetPerfMonitorCountersAMD) (GLuint, GLint *, GLint *, GLsizei, GLuint *);
    void (*GetPerfMonitorGroupStringAMD) (GLuint, GLsizei, GLsizei *, GLchar *);
    void (*GetPerfMonitorCounterStringAMD) (GLuint, GLuint, GLsizei, GLsizei *, GLchar *);
    void (*GetPerfMonitorCounterInfoAMD) (GLuint, GLuint, GLenum, GLvoid *);
    void (*GenPerfMonitorsAMD) (GLsizei, GLuint *);
    void (*DeletePerfMonitorsAMD) (GLsizei, GLuint *);
    void (*SelectPerfMonitorCountersAMD) (GLuint, GLboolean, GLuint, GLint, GLuint *);
    void (*BeginPerfMonitorAMD) (GLuint);
    void (*EndPerfMonitorAMD) (GLuint);
    void (*GetPerfMonitorCounterDataAMD) (GLuint, GLenum, GLsizei, GLuint *, GLint *);
#endif

#ifdef GL_ANGLE_framebuffer_blit
    void (*BlitFramebufferANGLE) (GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint,
                                  GLbitfield, GLenum);
#endif

#ifdef GL_ANGLE_framebuffer_multisample
    void (*RenderbufferStorageMultisampleANGLE) (GLenum, GLsizei, GLenum, GLsizei, GLsizei);
#endif

#ifdef GL_APPLE_framebuffer_multisample
    void (*RenderbufferStorageMultisampleAPPLE) (GLenum, GLsizei, GLenum, GLsizei, GLsizei);
    void (*ResolveMultisampleFramebufferAPPLE) (void);
#endif

#ifdef GL_IMG_multisampled_render_to_texture
    void (*RenderbufferStorageMultisampleIMG) (GLenum, GLsizei, GLenum, GLsizei, GLsizei);
    void (*FramebufferTexture2DMultisampleIMG) (GLenum, GLenum, GLenum, GLuint, GLint, GLsizei);
#endif

#ifdef GL_EXT_discard_framebuffer
    void (*DiscardFramebufferEXT) (GLenum, GLsizei, const GLenum *);
#endif

#ifdef GL_EXT_multisampled_render_to_texture
    void (*RenderbufferStorageMultisampleEXT) (GLenum, GLsizei, GLenum, GLsizei, GLsizei);
    void (*FramebufferTexture2DMultisampleEXT) (GLenum, GLenum, GLenum, GLuint, GLint, GLsizei);
#endif

#ifdef GL_EXT_multi_draw_arrays
    void (*MultiDrawArraysEXT) (GLenum, const GLint *, const GLsizei *, GLsizei);
    void (*MultiDrawElementsEXT) (GLenum, const GLsizei *, GLenum, const GLvoid **, GLsizei);
#endif

#ifdef GL_NV_fence
    void (*DeleteFencesNV) (GLsizei, const GLuint *);
    void (*GenFencesNV) (GLsizei, GLuint *);
    GLboolean (*IsFenceNV) (GLuint);
    GLboolean (*TestFenceNV) (GLuint);
    void (*GetFenceivNV) (GLuint, GLenum, GLint *);
    void (*FinishFenceNV) (GLuint);
    void (*SetFenceNV) (GLuint, GLenum);
#endif

#ifdef GL_NV_coverage_sample
    void (*CoverageMaskNV) (GLboolean mask);
    void (*CoverageOperationNV) (GLenum operation);
#endif

#ifdef GL_QCOM_driver_control
    void (*GetDriverControlsQCOM) (GLint *num, GLsizei size, GLuint *driverControls);
    void (*GetDriverControlStringQCOM) (GLuint driverControl, GLsizei bufSize, GLsizei *length, GLchar *driverControlString);
    void (*EnableDriverControlQCOM) (GLuint driverControl);
    void (*DisableDriverControlQCOM) (GLuint driverControl);
#endif

#ifdef GL_QCOM_extended_get
    void (*ExtGetTexturesQCOM) (GLuint *, GLint, GLint *);
    void (*ExtGetBuffersQCOM) (GLuint *, GLint, GLint *);
    void (*ExtGetRenderbuffersQCOM) (GLuint *, GLint, GLint *);
    void (*ExtGetFramebuffersQCOM) (GLuint *, GLint, GLint *);
    void (*ExtGetTexLevelParameterivQCOM) (GLuint, GLenum, GLint, GLenum, GLint *);
    void (*ExtTexObjectStateOverrideiQCOM) (GLenum, GLenum, GLint);
    void (*ExtGetTexSubImageQCOM) (GLenum, GLint, GLint, GLint, GLint, GLsizei,
                                   GLsizei, GLsizei, GLenum, GLenum, GLvoid *);
    void (*ExtGetBufferPointervQCOM) (GLenum, GLvoid **);
#endif

#ifdef GL_QCOM_extended_get2
    void (*ExtGetShadersQCOM) (GLuint *, GLint, GLint *);
    void (*ExtGetProgramsQCOM) (GLuint *, GLint, GLint *);
    GLboolean (*ExtIsProgramBinaryQCOM) (GLuint);
    void (*ExtGetProgramBinarySourceQCOM) (GLuint, GLenum, GLchar *, GLint *);
#endif

#ifdef GL_QCOM_tiled_rendering
    void (*StartTilingQCOM) (GLuint, GLuint, GLuint, GLuint, GLbitfield);
    void (*EndTilingQCOM) (GLbitfield);
#endif

#endif // HAS_GLES2

#if defined(HAS_GL) || defined(HAS_GLES2)
    FunctionPointerType
    (*GetProcAddress) (const char *procname);

    void  (*ActiveTexture)(GLenum);
    void  (*AttachShader)(GLuint, GLuint);
    void  (*BindAttribLocation)(GLuint, GLuint, const GLchar*);
    void  (*BindBuffer)(GLenum, GLuint);
    void  (*BindFramebuffer)(GLenum, GLuint);
    void  (*BindRenderbuffer)(GLenum, GLuint);
    void  (*BindTexture)(GLenum, GLuint);
    void  (*BlendColor)(GLclampf, GLclampf, GLclampf, GLclampf);
    void  (*BlendEquation)(GLenum);
    void  (*BlendEquationSeparate)(GLenum, GLenum);
    void  (*BlendFunc)(GLenum, GLenum);
    void  (*BlendFuncSeparate)(GLenum, GLenum, GLenum, GLenum);
    void  (*BufferData)(GLenum, GLsizeiptr, const GLvoid*, GLenum);
    void  (*BufferSubData)(GLenum, GLintptr, GLsizeiptr, const GLvoid*);
    GLenum  (*CheckFramebufferStatus)(GLenum);
    void  (*Clear)(GLbitfield);
    void  (*ClearColor)(GLclampf, GLclampf, GLclampf, GLclampf);
    void  (*ClearDepthf)(GLclampf);
    void  (*ClearStencil)(GLint);
    void  (*ColorMask)(GLboolean, GLboolean, GLboolean, GLboolean);
    void  (*CompileShader)(GLuint);
    void  (*CompressedTexImage2D)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*);
    void  (*CompressedTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*);
    void  (*CopyTexImage2D)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
    void  (*CopyTexSubImage2D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
    GLuint (*CreateProgram)(void);
    GLuint (*CreateShader)(GLenum);
    void  (*CullFace)(GLenum);
    void  (*DeleteBuffers)(GLsizei, const GLuint*);
    void  (*DeleteFramebuffers)(GLsizei, const GLuint*);
    void  (*DeleteProgram)(GLuint);
    void  (*DeleteRenderbuffers)(GLsizei, const GLuint*);
    void  (*DeleteShader)(GLuint);
    void  (*DeleteTextures)(GLsizei, const GLuint*);
    void  (*DepthFunc)(GLenum);
    void  (*DepthMask)(GLboolean);
    void  (*DepthRangef)(GLclampf, GLclampf);
    void  (*DetachShader)(GLuint, GLuint);
    void  (*Disable)(GLenum);
    void  (*DisableVertexAttribArray)(GLuint index);
    void  (*DrawArrays)(GLenum mode, GLint first, GLsizei count);
    void  (*DrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
    void  (*Enable)(GLenum cap);
    void  (*EnableVertexAttribArray)(GLuint index);
    void  (*Finish)(void);
    void  (*Flush)(void);
    void  (*FramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    void  (*FramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    void  (*FrontFace)(GLenum mode);
    void  (*GenBuffers)(GLsizei n, GLuint* buffers);
    void  (*GenerateMipmap)(GLenum target);
    void  (*GenFramebuffers)(GLsizei n, GLuint* framebuffers);
    void  (*GenRenderbuffers)(GLsizei n, GLuint* renderbuffers);
    void  (*GenTextures)(GLsizei n, GLuint* textures);
    void  (*GetActiveAttrib)(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
    void  (*GetActiveUniform)(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
    void  (*GetAttachedShaders)(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
    int   (*GetAttribLocation)(GLuint program, const GLchar* name);
    void  (*GetBooleanv)(GLenum pname, GLboolean* params);
    void  (*GetBufferParameteriv)(GLenum target, GLenum pname, GLint* params);
    GLenum (*GetError)(void);
    void  (*GetFloatv)(GLenum pname, GLfloat* params);
    void  (*GetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint* params);
    void  (*GetIntegerv)(GLenum pname, GLint* params);
    void  (*GetProgramiv)(GLuint program, GLenum pname, GLint* params);
    void  (*GetProgramInfoLog)(GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog);
    void  (*GetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint* params);
    void  (*GetShaderiv)(GLuint shader, GLenum pname, GLint* params);
    void  (*GetShaderInfoLog)(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog);
    void  (*GetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
    void  (*GetShaderSource)(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source);
    const GLubyte* (*GetString)(GLenum name);
    void  (*GetTexParameterfv)(GLenum target, GLenum pname, GLfloat* params);
    void  (*GetTexParameteriv)(GLenum target, GLenum pname, GLint* params);
    void  (*GetUniformfv)(GLuint program, GLint location, GLfloat* params);
    void  (*GetUniformiv)(GLuint program, GLint location, GLint* params);
    int   (*GetUniformLocation)(GLuint program, const GLchar* name);
    void  (*GetVertexAttribfv)(GLuint index, GLenum pname, GLfloat* params);
    void  (*GetVertexAttribiv)(GLuint index, GLenum pname, GLint* params);
    void  (*GetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid** pointer);
    void  (*Hint)(GLenum target, GLenum mode);
    GLboolean  (*IsBuffer)(GLuint buffer);
    GLboolean  (*IsEnabled)(GLenum cap);
    GLboolean  (*IsFramebuffer)(GLuint framebuffer);
    GLboolean  (*IsProgram)(GLuint program);
    GLboolean  (*IsRenderbuffer)(GLuint renderbuffer);
    GLboolean  (*IsShader)(GLuint shader);
    GLboolean  (*IsTexture)(GLuint texture);
    void  (*LineWidth)(GLfloat width);
    void  (*LinkProgram)(GLuint program);
    void  (*PixelStorei)(GLenum pname, GLint param);
    void  (*PolygonOffset)(GLfloat factor, GLfloat units);
    void  (*ReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
    void  (*ReleaseShaderCompiler)(void);
    void  (*RenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    void  (*SampleCoverage)(GLclampf value, GLboolean invert);
    void  (*Scissor)(GLint x, GLint y, GLsizei width, GLsizei height);
    void  (*ShaderBinary)(GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length);
    void  (*ShaderSource)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
    void  (*StencilFunc)(GLenum func, GLint ref, GLuint mask);
    void  (*StencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
    void  (*StencilMask)(GLuint mask);
    void  (*StencilMaskSeparate)(GLenum face, GLuint mask);
    void  (*StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
    void  (*StencilOpSeparate)(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
    void  (*TexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
    void  (*TexParameterf)(GLenum target, GLenum pname, GLfloat param);
    void  (*TexParameterfv)(GLenum, GLenum, const GLfloat*);
    void  (*TexParameteri)(GLenum, GLenum, GLint);
    void  (*TexParameteriv)(GLenum, GLenum, const GLint*);
    void  (*TexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum format, GLenum, const GLvoid*);
    void  (*Uniform1f)(GLint, GLfloat);
    void  (*Uniform1fv)(GLint, GLsizei, const GLfloat*);
    void  (*Uniform1i)(GLint, GLint);
    void  (*Uniform1iv)(GLint, GLsizei count, const GLint*);
    void  (*Uniform2f)(GLint, GLfloat, GLfloat);
    void  (*Uniform2fv)(GLint, GLsizei, const GLfloat*);
    void  (*Uniform2i)(GLint, GLint x, GLint);
    void  (*Uniform2iv)(GLint, GLsizei, const GLint*);
    void  (*Uniform3f)(GLint, GLfloat, GLfloat, GLfloat);
    void  (*Uniform3fv)(GLint, GLsizei, const GLfloat*);
    void  (*Uniform3i)(GLint, GLint, GLint, GLint);
    void  (*Uniform3iv)(GLint, GLsizei, const GLint*);
    void  (*Uniform4f)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*Uniform4fv)(GLint, GLsizei, const GLfloat*);
    void  (*Uniform4i)(GLint, GLint, GLint, GLint, GLint);
    void  (*Uniform4iv)(GLint, GLsizei, const GLint*);
    void  (*UniformMatrix2fv)(GLint, GLsizei, GLboolean, const GLfloat*);
    void  (*UniformMatrix3fv)(GLint, GLsizei, GLboolean, const GLfloat*);
    void  (*UniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat*);
    void  (*UseProgram)(GLuint);
    void  (*ValidateProgram)(GLuint);
    void  (*VertexAttrib1f)(GLuint, GLfloat);
    void  (*VertexAttrib1fv)(GLuint, const GLfloat*);
    void  (*VertexAttrib2f)(GLuint, GLfloat, GLfloat);
    void  (*VertexAttrib2fv)(GLuint, const GLfloat*);
    void  (*VertexAttrib3f)(GLuint, GLfloat, GLfloat, GLfloat);
    void  (*VertexAttrib3fv)(GLuint, const GLfloat*);
    void  (*VertexAttrib4f)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*VertexAttrib4fv)(GLuint, const GLfloat*);
    void  (*VertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
    void  (*Viewport)(GLint, GLint, GLsizei, GLsizei);
#endif

#if HAS_GL
    void  (*NewList)(GLuint, GLenum);
    void  (*EndList)(void);
    void  (*CallList)(GLuint);
    void  (*CallLists)(GLsizei, GLenum, const GLvoid *);
    void  (*DeleteLists)(GLuint, GLsizei);
    GLuint  (*GenLists)(GLsizei);
    void  (*ListBase)(GLuint);
    void  (*Begin)(GLenum);
    void  (*Bitmap)(GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte *);
    void  (*Color3b)(GLbyte, GLbyte, GLbyte);
    void  (*Color3bv)(const GLbyte *);
    void  (*Color3d)(GLdouble, GLdouble, GLdouble);
    void  (*Color3dv)(const GLdouble *);
    void  (*Color3f)(GLfloat, GLfloat, GLfloat);
    void  (*Color3fv)(const GLfloat *);
    void  (*Color3i)(GLint, GLint, GLint);
    void  (*Color3iv)(const GLint *);
    void  (*Color3s)(GLshort, GLshort, GLshort);
    void  (*Color3sv)(const GLshort *);
    void  (*Color3ub)(GLubyte, GLubyte, GLubyte);
    void  (*Color3ubv)(const GLubyte *);
    void  (*Color3ui)(GLuint, GLuint, GLuint);
    void  (*Color3uiv)(const GLuint *);
    void  (*Color3us)(GLushort, GLushort, GLushort);
    void  (*Color3usv)(const GLushort *);
    void  (*Color4b)(GLbyte, GLbyte, GLbyte, GLbyte);
    void  (*Color4bv)(const GLbyte *);
    void  (*Color4d)(GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*Color4dv)(const GLdouble *);
    void  (*Color4f)(GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*Color4fv)(const GLfloat *);
    void  (*Color4i)(GLint, GLint, GLint, GLint);
    void  (*Color4iv)(const GLint *);
    void  (*Color4s)(GLshort, GLshort, GLshort, GLshort);
    void  (*Color4sv)(const GLshort *);
    void  (*Color4ub)(GLubyte, GLubyte, GLubyte, GLubyte);
    void  (*Color4ubv)(const GLubyte *);
    void  (*Color4ui)(GLuint, GLuint, GLuint, GLuint);
    void  (*Color4uiv)(const GLuint *);
    void  (*Color4us)(GLushort, GLushort, GLushort, GLushort);
    void  (*Color4usv)(const GLushort *);
    void  (*EdgeFlag)(GLboolean);
    void  (*EdgeFlagv)(const GLboolean *);
    void  (*End)(void);
    void  (*Indexd)(GLdouble);
    void  (*Indexdv)(const GLdouble *);
    void  (*Indexf)(GLfloat);
    void  (*Indexfv)(const GLfloat *);
    void  (*Indexi)(GLint);
    void  (*Indexiv)(const GLint *);
    void  (*Indexs)(GLshort);
    void  (*Indexsv)(const GLshort *);
    void  (*Normal3b)(GLbyte, GLbyte, GLbyte);
    void  (*Normal3bv)(const GLbyte *);
    void  (*Normal3d)(GLdouble, GLdouble, GLdouble);
    void  (*Normal3dv)(const GLdouble *);
    void  (*Normal3f)(GLfloat, GLfloat, GLfloat);
    void  (*Normal3fv)(const GLfloat *);
    void  (*Normal3i)(GLint, GLint, GLint);
    void  (*Normal3iv)(const GLint *);
    void  (*Normal3s)(GLshort, GLshort, GLshort);
    void  (*Normal3sv)(const GLshort *);
    void  (*RasterPos2d)(GLdouble, GLdouble);
    void  (*RasterPos2dv)(const GLdouble *);
    void  (*RasterPos2f)(GLfloat, GLfloat);
    void  (*RasterPos2fv)(const GLfloat *);
    void  (*RasterPos2i)(GLint, GLint);
    void  (*RasterPos2iv)(const GLint *);
    void  (*RasterPos2s)(GLshort, GLshort);
    void  (*RasterPos2sv)(const GLshort *);
    void  (*RasterPos3d)(GLdouble, GLdouble, GLdouble);
    void  (*RasterPos3dv)(const GLdouble *);
    void  (*RasterPos3f)(GLfloat, GLfloat, GLfloat);
    void  (*RasterPos3fv)(const GLfloat *);
    void  (*RasterPos3i)(GLint, GLint, GLint);
    void  (*RasterPos3iv)(const GLint *);
    void  (*RasterPos3s)(GLshort, GLshort, GLshort);
    void  (*RasterPos3sv)(const GLshort *);
    void  (*RasterPos4d)(GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*RasterPos4dv)(const GLdouble *);
    void  (*RasterPos4f)(GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*RasterPos4fv)(const GLfloat *);
    void  (*RasterPos4i)(GLint, GLint, GLint, GLint);
    void  (*RasterPos4iv)(const GLint *);
    void  (*RasterPos4s)(GLshort, GLshort, GLshort, GLshort);
    void  (*RasterPos4sv)(const GLshort *);
    void  (*Rectd)(GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*Rectdv)(const GLdouble *, const GLdouble *);
    void  (*Rectf)(GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*Rectfv)(const GLfloat *, const GLfloat *);
    void  (*Recti)(GLint, GLint, GLint, GLint);
    void  (*Rectiv)(const GLint *, const GLint *);
    void  (*Rects)(GLshort, GLshort, GLshort, GLshort);
    void  (*Rectsv)(const GLshort *, const GLshort *);
    void  (*TexCoord1d)(GLdouble);
    void  (*TexCoord1dv)(const GLdouble *);
    void  (*TexCoord1f)(GLfloat);
    void  (*TexCoord1fv)(const GLfloat *);
    void  (*TexCoord1i)(GLint);
    void  (*TexCoord1iv)(const GLint *);
    void  (*TexCoord1s)(GLshort);
    void  (*TexCoord1sv)(const GLshort *);
    void  (*TexCoord2d)(GLdouble, GLdouble);
    void  (*TexCoord2dv)(const GLdouble *);
    void  (*TexCoord2f)(GLfloat, GLfloat);
    void  (*TexCoord2fv)(const GLfloat *);
    void  (*TexCoord2i)(GLint, GLint);
    void  (*TexCoord2iv)(const GLint *);
    void  (*TexCoord2s)(GLshort, GLshort);
    void  (*TexCoord2sv)(const GLshort *);
    void  (*TexCoord3d)(GLdouble, GLdouble, GLdouble);
    void  (*TexCoord3dv)(const GLdouble *);
    void  (*TexCoord3f)(GLfloat, GLfloat, GLfloat);
    void  (*TexCoord3fv)(const GLfloat *);
    void  (*TexCoord3i)(GLint, GLint, GLint);
    void  (*TexCoord3iv)(const GLint *);
    void  (*TexCoord3s)(GLshort, GLshort, GLshort);
    void  (*TexCoord3sv)(const GLshort *);
    void  (*TexCoord4d)(GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*TexCoord4dv)(const GLdouble *);
    void  (*TexCoord4f)(GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*TexCoord4fv)(const GLfloat *);
    void  (*TexCoord4i)(GLint, GLint, GLint, GLint);
    void  (*TexCoord4iv)(const GLint *);
    void  (*TexCoord4s)(GLshort, GLshort, GLshort, GLshort);
    void  (*TexCoord4sv)(const GLshort *);
    void  (*Vertex2d)(GLdouble, GLdouble);
    void  (*Vertex2dv)(const GLdouble *);
    void  (*Vertex2f)(GLfloat, GLfloat);
    void  (*Vertex2fv)(const GLfloat *);
    void  (*Vertex2i)(GLint, GLint);
    void  (*Vertex2iv)(const GLint *);
    void  (*Vertex2s)(GLshort, GLshort);
    void  (*Vertex2sv)(const GLshort *);
    void  (*Vertex3d)(GLdouble, GLdouble, GLdouble);
    void  (*Vertex3dv)(const GLdouble *);
    void  (*Vertex3f)(GLfloat, GLfloat, GLfloat);
    void  (*Vertex3fv)(const GLfloat *);
    void  (*Vertex3i)(GLint, GLint, GLint);
    void  (*Vertex3iv)(const GLint *);
    void  (*Vertex3s)(GLshort, GLshort, GLshort);
    void  (*Vertex3sv)(const GLshort *);
    void  (*Vertex4d)(GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*Vertex4dv)(const GLdouble *);
    void  (*Vertex4f)(GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*Vertex4fv)(const GLfloat *);
    void  (*Vertex4i)(GLint, GLint, GLint, GLint);
    void  (*Vertex4iv)(const GLint *);
    void  (*Vertex4s)(GLshort, GLshort, GLshort, GLshort);
    void  (*Vertex4sv)(const GLshort *);
    void  (*ClipPlane)(GLenum, const GLdouble *);
    void  (*ColorMaterial)(GLenum, GLenum);
    void  (*Fogf)(GLenum, GLfloat);
    void  (*Fogfv)(GLenum, const GLfloat *);
    void  (*Fogi)(GLenum, GLint);
    void  (*Fogiv)(GLenum, const GLint *);
    void  (*Lightf)(GLenum, GLenum, GLfloat);
    void  (*Lightfv)(GLenum, GLenum, const GLfloat *);
    void  (*Lighti)(GLenum, GLenum, GLint);
    void  (*Lightiv)(GLenum, GLenum, const GLint *);
    void  (*LightModelf)(GLenum, GLfloat);
    void  (*LightModelfv)(GLenum, const GLfloat *);
    void  (*LightModeli)(GLenum, GLint);
    void  (*LightModeliv)(GLenum, const GLint *);
    void  (*LineStipple)(GLint, GLushort);
    void  (*Materialf)(GLenum, GLenum, GLfloat);
    void  (*Materialfv)(GLenum, GLenum, const GLfloat *);
    void  (*Materiali)(GLenum, GLenum, GLint);
    void  (*Materialiv)(GLenum, GLenum, const GLint *);
    void  (*PointSize)(GLfloat);
    void  (*PolygonMode)(GLenum, GLenum);
    void  (*PolygonStipple)(const GLubyte *);
    void  (*ShadeModel)(GLenum);
    void  (*TexImage1D)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
    void  (*TexEnvf)(GLenum, GLenum, GLfloat);
    void  (*TexEnvfv)(GLenum, GLenum, const GLfloat *);
    void  (*TexEnvi)(GLenum, GLenum, GLint);
    void  (*TexEnviv)(GLenum, GLenum, const GLint *);
    void  (*TexGend)(GLenum, GLenum, GLdouble);
    void  (*TexGendv)(GLenum, GLenum, const GLdouble *);
    void  (*TexGenf)(GLenum, GLenum, GLfloat);
    void  (*TexGenfv)(GLenum, GLenum, const GLfloat *);
    void  (*TexGeni)(GLenum, GLenum, GLint);
    void  (*TexGeniv)(GLenum, GLenum, const GLint *);
    void  (*FeedbackBuffer)(GLsizei, GLenum, GLfloat *);
    void  (*SelectBuffer)(GLsizei, GLuint *);
    GLint  (*RenderMode)(GLenum);
    void  (*InitNames)(void);
    void  (*LoadName)(GLuint);
    void  (*PassThrough)(GLfloat);
    void  (*PopName)(void);
    void  (*PushName)(GLuint);
    void  (*DrawBuffer)(GLenum);
    void  (*ClearAccum)(GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*ClearIndex)(GLfloat);
    void  (*ClearDepth)(GLclampd);
    void  (*IndexMask)(GLuint);
    void  (*Accum)(GLenum, GLfloat);
    void  (*PopAttrib)(void);
    void  (*PushAttrib)(GLbitfield);
    void  (*Map1d)(GLenum, GLdouble, GLdouble, GLint, GLint, const GLdouble *);
    void  (*Map1f)(GLenum, GLfloat, GLfloat, GLint, GLint, const GLfloat *);
    void  (*Map2d)(GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *);
    void  (*Map2f)(GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *);
    void  (*MapGrid1d)(GLint, GLdouble, GLdouble);
    void  (*MapGrid1f)(GLint, GLfloat, GLfloat);
    void  (*MapGrid2d)(GLint, GLdouble, GLdouble, GLint, GLdouble, GLdouble);
    void  (*MapGrid2f)(GLint, GLfloat, GLfloat, GLint, GLfloat, GLfloat);
    void  (*EvalCoord1d)(GLdouble);
    void  (*EvalCoord1dv)(const GLdouble *);
    void  (*EvalCoord1f)(GLfloat);
    void  (*EvalCoord1fv)(const GLfloat *);
    void  (*EvalCoord2d)(GLdouble u, GLdouble v);
    void  (*EvalCoord2dv)(const GLdouble *);
    void  (*EvalCoord2f)(GLfloat, GLfloat);
    void  (*EvalCoord2fv)(const GLfloat *);
    void  (*EvalMesh1)(GLenum, GLint, GLint);
    void  (*EvalPoint1)(GLint);
    void  (*EvalMesh2)(GLenum, GLint, GLint, GLint, GLint);
    void  (*EvalPoint2)(GLint, GLint);
    void  (*AlphaFunc)(GLenum, GLclampf);
    void  (*LogicOp)(GLenum);
    void  (*PixelZoom)(GLfloat, GLfloat);
    void  (*PixelTransferf)(GLenum, GLfloat);
    void  (*PixelTransferi)(GLenum, GLint);
    void  (*PixelStoref)(GLenum, GLfloat);
    void  (*PixelMapfv)(GLenum, GLsizei, const GLfloat *);
    void  (*PixelMapuiv)(GLenum, GLsizei, const GLuint *);
    void  (*PixelMapusv)(GLenum, GLsizei, const GLushort *);
    void  (*ReadBuffer)(GLenum);
    void  (*CopyPixels)(GLint, GLint, GLsizei, GLsizei, GLenum);
    void  (*DrawPixels)(GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
    void  (*GetClipPlane)(GLenum, GLdouble *);
    void  (*GetDoublev)(GLenum, GLdouble *);
    void  (*GetLightfv)(GLenum, GLenum, GLfloat *);
    void  (*GetLightiv)(GLenum, GLenum, GLint *);
    void  (*GetMapdv)(GLenum, GLenum, GLdouble *);
    void  (*GetMapfv)(GLenum, GLenum, GLfloat *);
    void  (*GetMapiv)(GLenum, GLenum, GLint *);
    void  (*GetMaterialfv)(GLenum, GLenum, GLfloat *);
    void  (*GetMaterialiv)(GLenum, GLenum, GLint *);
    void  (*GetPixelMapfv)(GLenum, GLfloat *);
    void  (*GetPixelMapuiv)(GLenum, GLuint *);
    void  (*GetPixelMapusv)(GLenum, GLushort *);
    void  (*GetPolygonStipple)(GLubyte *);
    void  (*GetTexEnvfv)(GLenum, GLenum, GLfloat *);
    void  (*GetTexEnviv)(GLenum, GLenum, GLint *);
    void  (*GetTexGendv)(GLenum, GLenum, GLdouble *);
    void  (*GetTexGenfv)(GLenum, GLenum, GLfloat *);
    void  (*GetTexGeniv)(GLenum, GLenum, GLint *);
    void  (*GetTexImage)(GLenum, GLint, GLenum, GLenum, GLvoid *);
    void  (*GetTexLevelParameterfv)(GLenum, GLint, GLenum, GLfloat *);
    void  (*GetTexLevelParameteriv)(GLenum, GLint, GLenum, GLint *);
    GLboolean  (*IsList)(GLuint);
    void  (*DepthRange)(GLclampd, GLclampd);
    void  (*Frustum)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*LoadIdentity)(void);
    void  (*LoadMatrixf)(const GLfloat *);
    void  (*LoadMatrixd)(const GLdouble *);
    void  (*MatrixMode)(GLenum);
    void  (*MultMatrixf)(const GLfloat *);
    void  (*MultMatrixd)(const GLdouble *);
    void  (*Ortho)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*PopMatrix)(void);
    void  (*PushMatrix)(void);
    void  (*Rotated)(GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*Rotatef)(GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*Scaled)( GLdouble, GLdouble y, GLdouble z);
    void  (*Scalef)(GLfloat, GLfloat, GLfloat);
    void  (*Translated)(GLdouble, GLdouble, GLdouble);
    void  (*Translatef)(GLfloat, GLfloat, GLfloat);
    void  (*ArrayElement)(GLint);
    void  (*ColorPointer)(GLint, GLenum, GLsizei, const GLvoid *);
    void  (*DisableClientState)(GLenum);
    void  (*EdgeFlagPointer)(GLsizei, const GLvoid *);
    void  (*EnableClientState)(GLenum);
    void  (*IndexPointer)(GLenum, GLsizei, const GLvoid *);
    void  (*Indexub)(GLubyte);
    void  (*Indexubv)(const GLubyte *);
    void  (*InterleavedArrays)(GLenum, GLsizei, const GLvoid *);
    void  (*NormalPointer)(GLenum, GLsizei, const GLvoid *);
    void  (*TexCoordPointer)(GLint, GLenum, GLsizei, const GLvoid *);
    void  (*VertexPointer)(GLint, GLenum, GLsizei, const GLvoid *);
    GLboolean  (*AreTexturesResident)(GLsizei, const GLuint *, GLboolean *);
    void  (*CopyTexImage1D)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
    void  (*CopyTexSubImage1D)(GLenum, GLint, GLint, GLint, GLint, GLsizei);
    void  (*GetPointerv)(GLenum, GLvoid **);
    void  (*PrioritizeTextures)(GLsizei, const GLuint *, const GLclampf *);
    void  (*TexSubImage1D)(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *);
    void  (*PopClientAttrib)(void);
    void  (*PushClientAttrib)(GLbitfield);
    void  (*DrawRangeElements)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *);
    void  (*ColorTable)(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
    void  (*ColorTableParameterfv)(GLenum, GLenum, const GLfloat *);
    void  (*ColorTableParameteriv)(GLenum, GLenum, const GLint *);
    void  (*CopyColorTable)(GLenum, GLenum, GLint, GLint, GLsizei);
    void  (*GetColorTable)(GLenum, GLenum, GLenum, GLvoid *);
    void  (*GetColorTableParameterfv)(GLenum, GLenum, GLfloat *);
    void  (*GetColorTableParameteriv)(GLenum, GLenum, GLint *);
    void  (*ColorSubTable)(GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
    void  (*CopyColorSubTable)(GLenum, GLsizei, GLint, GLint, GLsizei);
    void  (*ConvolutionFilter1D)(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
    void  (*ConvolutionFilter2D)(GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
    void  (*ConvolutionParameterf)(GLenum, GLenum, GLfloat);
    void  (*ConvolutionParameterfv)(GLenum, GLenum, const GLfloat *);
    void  (*ConvolutionParameteri)(GLenum, GLenum, GLint);
    void  (*ConvolutionParameteriv)(GLenum, GLenum, const GLint *);
    void  (*CopyConvolutionFilter1D)(GLenum, GLenum, GLint, GLint, GLsizei);
    void  (*CopyConvolutionFilter2D)(GLenum, GLenum, GLint, GLint, GLsizei, GLsizei);
    void  (*GetConvolutionFilter)(GLenum, GLenum, GLenum, GLvoid *);
    void  (*GetConvolutionParameterfv)(GLenum, GLenum, GLfloat *);
    void  (*GetConvolutionParameteriv)(GLenum, GLenum, GLint *);
    void  (*GetSeparableFilter)(GLenum, GLenum, GLenum, GLvoid *, GLvoid *, GLvoid *);
    void  (*SeparableFilter2D)(GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *, const GLvoid *);
    void  (*GetHistogram)(GLenum, GLboolean, GLenum, GLenum, GLvoid *);
    void  (*GetHistogramParameterfv)(GLenum, GLenum, GLfloat *);
    void  (*GetHistogramParameteriv)(GLenum, GLenum, GLint *);
    void  (*GetMinmax)(GLenum, GLboolean, GLenum, GLenum, GLvoid *);
    void  (*GetMinmaxParameterfv)(GLenum, GLenum, GLfloat *);
    void  (*GetMinmaxParameteriv)(GLenum, GLenum, GLint *);
    void  (*Histogram)(GLenum, GLsizei, GLenum, GLboolean);
    void  (*Minmax)(GLenum, GLenum, GLboolean);
    void  (*ResetHistogram)(GLenum);
    void  (*ResetMinmax)(GLenum);
    void  (*TexImage3D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
    void  (*TexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
    void  (*CopyTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
    void  (*ClientActiveTextureARB)(GLenum);
    void  (*MultiTexCoord1dARB)(GLenum, GLdouble);
    void  (*MultiTexCoord1dvARB)(GLenum, const GLdouble *);
    void  (*MultiTexCoord1fARB)(GLenum, GLfloat);
    void  (*MultiTexCoord1fvARB)(GLenum, const GLfloat *);
    void  (*MultiTexCoord1iARB)(GLenum, GLint);
    void  (*MultiTexCoord1ivARB)(GLenum, const GLint *);
    void  (*MultiTexCoord1sARB)(GLenum, GLshort);
    void  (*MultiTexCoord1svARB)(GLenum, const GLshort *);
    void  (*MultiTexCoord2dARB)(GLenum, GLdouble, GLdouble);
    void  (*MultiTexCoord2dvARB)(GLenum, const GLdouble *);
    void  (*MultiTexCoord2fARB)(GLenum, GLfloat, GLfloat);
    void  (*MultiTexCoord2fvARB)(GLenum, const GLfloat *);
    void  (*MultiTexCoord2iARB)(GLenum, GLint, GLint);
    void  (*MultiTexCoord2ivARB)(GLenum, const GLint *);
    void  (*MultiTexCoord2sARB)(GLenum, GLshort, GLshort);
    void  (*MultiTexCoord2svARB)(GLenum, const GLshort *);
    void  (*MultiTexCoord3dARB)(GLenum, GLdouble, GLdouble, GLdouble);
    void  (*MultiTexCoord3dvARB)(GLenum, const GLdouble *);
    void  (*MultiTexCoord3fARB)(GLenum, GLfloat, GLfloat, GLfloat);
    void  (*MultiTexCoord3fvARB)(GLenum, const GLfloat *);
    void  (*MultiTexCoord3iARB)(GLenum, GLint, GLint, GLint);
    void  (*MultiTexCoord3ivARB)(GLenum, const GLint *);
    void  (*MultiTexCoord3sARB)(GLenum, GLshort, GLshort, GLshort);
    void  (*MultiTexCoord3svARB)(GLenum, const GLshort *);
    void  (*MultiTexCoord4dARB)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*MultiTexCoord4dvARB)(GLenum, const GLdouble *);
    void  (*MultiTexCoord4fARB)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*MultiTexCoord4fvARB)(GLenum, const GLfloat *);
    void  (*MultiTexCoord4iARB)(GLenum, GLint, GLint, GLint, GLint);
    void  (*MultiTexCoord4ivARB)(GLenum, const GLint *);
    void  (*MultiTexCoord4sARB)(GLenum, GLshort, GLshort, GLshort, GLshort);
    void  (*MultiTexCoord4svARB)(GLenum, const GLshort *);
    void  (*UniformMatrix2x3fv)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*UniformMatrix2x4fv)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*UniformMatrix3x2fv)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*UniformMatrix3x4fv)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*UniformMatrix4x2fv)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*UniformMatrix4x3fv)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*ClampColor)(GLenum, GLenum);
    void  (*ClearBufferfi)(GLenum, GLint, GLfloat, GLint);
    void  (*ClearBufferfv)(GLenum, GLint, const GLfloat *);
    void  (*ClearBufferiv)(GLenum, GLint, const GLint *);
    void  (*ClearBufferuiv)(GLenum, GLint, const GLuint *);
    const GLubyte *  (*GetStringi)(GLenum, GLuint);
    void  (*FramebufferTexture)(GLenum, GLenum, GLuint, GLint);
    void  (*GetBufferParameteri64v)(GLenum, GLenum, GLint64 *);
    void  (*GetInteger64i_v)(GLenum, GLuint, GLint64 *);
    void  (*VertexAttribDivisor)(GLuint, GLuint);
    void  (*LoadTransposeMatrixdARB)(const GLdouble *);
    void  (*LoadTransposeMatrixfARB)(const GLfloat *);
    void  (*MultTransposeMatrixdARB)(const GLdouble *);
    void  (*MultTransposeMatrixfARB)(const GLfloat *);
    void  (*SampleCoverageARB)(GLclampf, GLboolean);
    void  (*CompressedTexImage1DARB)(GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *);
    void  (*CompressedTexImage3DARB)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
    void  (*CompressedTexSubImage1DARB)(GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *);
    void  (*CompressedTexSubImage3DARB)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
    void  (*GetCompressedTexImageARB)(GLenum, GLint, GLvoid *);
    void  (*DisableVertexAttribArrayARB)(GLuint);
    void  (*EnableVertexAttribArrayARB)(GLuint);
    void  (*GetProgramEnvParameterdvARB)(GLenum, GLuint, GLdouble *);
    void  (*GetProgramEnvParameterfvARB)(GLenum, GLuint, GLfloat *);
    void  (*GetProgramLocalParameterdvARB)(GLenum, GLuint, GLdouble *);
    void  (*GetProgramLocalParameterfvARB)(GLenum, GLuint, GLfloat *);
    void  (*GetProgramStringARB)(GLenum, GLenum, GLvoid *);
    void  (*GetProgramivARB)(GLenum, GLenum, GLint *);
    void  (*GetVertexAttribdvARB)(GLuint, GLenum, GLdouble *);
    void  (*GetVertexAttribfvARB)(GLuint, GLenum, GLfloat *);
    void  (*GetVertexAttribivARB)(GLuint, GLenum, GLint *);
    void  (*ProgramEnvParameter4dARB)(GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*ProgramEnvParameter4dvARB)(GLenum, GLuint, const GLdouble *);
    void  (*ProgramEnvParameter4fARB)(GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*ProgramEnvParameter4fvARB)(GLenum, GLuint, const GLfloat *);
    void  (*ProgramLocalParameter4dARB)(GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*ProgramLocalParameter4dvARB)(GLenum, GLuint, const GLdouble *);
    void  (*ProgramLocalParameter4fARB)(GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*ProgramLocalParameter4fvARB)(GLenum, GLuint, const GLfloat *);
    void  (*ProgramStringARB)(GLenum, GLenum, GLsizei, const GLvoid *);
    void  (*VertexAttrib1dARB)(GLuint, GLdouble);
    void  (*VertexAttrib1dvARB)(GLuint, const GLdouble *);
    void  (*VertexAttrib1fARB)(GLuint, GLfloat);
    void  (*VertexAttrib1fvARB)(GLuint, const GLfloat *);
    void  (*VertexAttrib1sARB)(GLuint, GLshort);
    void  (*VertexAttrib1svARB)(GLuint, const GLshort *);
    void  (*VertexAttrib2dARB)(GLuint, GLdouble, GLdouble);
    void  (*VertexAttrib2dvARB)(GLuint, const GLdouble *);
    void  (*VertexAttrib2fARB)(GLuint, GLfloat, GLfloat);
    void  (*VertexAttrib2fvARB)(GLuint, const GLfloat *);
    void  (*VertexAttrib2sARB)(GLuint, GLshort, GLshort);
    void  (*VertexAttrib2svARB)(GLuint, const GLshort *);
    void  (*VertexAttrib3dARB)(GLuint, GLdouble, GLdouble, GLdouble);
    void  (*VertexAttrib3dvARB)(GLuint, const GLdouble *);
    void  (*VertexAttrib3fARB)(GLuint, GLfloat, GLfloat, GLfloat);
    void  (*VertexAttrib3fvARB)(GLuint, const GLfloat *);
    void  (*VertexAttrib3sARB)(GLuint, GLshort, GLshort, GLshort);
    void  (*VertexAttrib3svARB)(GLuint, const GLshort *);
    void  (*VertexAttrib4NbvARB)(GLuint, const GLbyte *);
    void  (*VertexAttrib4NivARB)(GLuint, const GLint *);
    void  (*VertexAttrib4NsvARB)(GLuint, const GLshort *);
    void  (*VertexAttrib4NubARB)(GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
    void  (*VertexAttrib4NubvARB)(GLuint, const GLubyte *);
    void  (*VertexAttrib4NuivARB)(GLuint, const GLuint *);
    void  (*VertexAttrib4NusvARB)(GLuint, const GLushort *);
    void  (*VertexAttrib4bvARB)(GLuint, const GLbyte *);
    void  (*VertexAttrib4dARB)(GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*VertexAttrib4dvARB)(GLuint, const GLdouble *);
    void  (*VertexAttrib4fARB)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*VertexAttrib4fvARB)(GLuint, const GLfloat *);
    void  (*VertexAttrib4ivARB)(GLuint, const GLint *);
    void  (*VertexAttrib4sARB)(GLuint, GLshort, GLshort, GLshort, GLshort);
    void  (*VertexAttrib4svARB)(GLuint, const GLshort *);
    void  (*VertexAttrib4ubvARB)(GLuint, const GLubyte *);
    void  (*VertexAttrib4uivARB)(GLuint, const GLuint *);
    void  (*VertexAttrib4usvARB)(GLuint, const GLushort *);
    void  (*VertexAttribPointerARB)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
    void  (*BufferDataARB)(GLenum, GLsizeiptrARB, const GLvoid *, GLenum);
    void  (*BufferSubDataARB)(GLenum, GLintptrARB, GLsizeiptrARB, const GLvoid *);
    void  (*GenBuffersARB)(GLsizei, GLuint *);
    void  (*GetBufferParameterivARB)(GLenum, GLenum, GLint *);
    void  (*GetBufferPointervARB)(GLenum, GLenum, GLvoid **);
    void  (*GetBufferSubDataARB)(GLenum, GLintptrARB, GLsizeiptrARB, GLvoid *);
    GLboolean  (*IsBufferARB)(GLuint);
    GLvoid *  (*MapBufferARB)(GLenum, GLenum);
    GLboolean  (*UnmapBufferARB)(GLenum);
    void  (*BeginQueryARB)(GLenum, GLuint);
    void  (*DeleteQueriesARB)(GLsizei, const GLuint *);
    void  (*EndQueryARB)(GLenum);
    void  (*GenQueriesARB)(GLsizei, GLuint *);
    void  (*GetQueryObjectivARB)(GLuint, GLenum, GLint *);
    void  (*GetQueryObjectuivARB)(GLuint, GLenum, GLuint *);
    void  (*GetQueryivARB)(GLenum, GLenum, GLint *);
    GLboolean  (*IsQueryARB)(GLuint);
    void  (*AttachObjectARB)(GLhandleARB, GLhandleARB);
    void  (*CompileShaderARB)(GLhandleARB);
    GLhandleARB  (*CreateProgramObjectARB)(void);
    GLhandleARB  (*CreateShaderObjectARB)(GLenum);
    void  (*DeleteObjectARB)(GLhandleARB);
    void  (*DetachObjectARB)(GLhandleARB, GLhandleARB);
    void  (*GetActiveUniformARB)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
    void  (*GetAttachedObjectsARB)(GLhandleARB, GLsizei, GLsizei *, GLhandleARB *);
    GLhandleARB  (*GetHandleARB)(GLenum);
    void  (*GetInfoLogARB)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *);
    void  (*GetObjectParameterfvARB)(GLhandleARB, GLenum, GLfloat *);
    void  (*GetObjectParameterivARB)(GLhandleARB, GLenum, GLint *);
    void  (*GetShaderSourceARB)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *);
    GLint  (*GetUniformLocationARB)(GLhandleARB, const GLcharARB *);
    void  (*GetUniformfvARB)(GLhandleARB, GLint, GLfloat *);
    void  (*GetUniformivARB)(GLhandleARB, GLint, GLint *);
    void  (*LinkProgramARB)(GLhandleARB);
    void  (*ShaderSourceARB)(GLhandleARB, GLsizei, const GLcharARB **, const GLint *);
    void  (*Uniform1fARB)(GLint, GLfloat);
    void  (*Uniform1fvARB)(GLint, GLsizei, const GLfloat *);
    void  (*Uniform1iARB)(GLint, GLint);
    void  (*Uniform1ivARB)(GLint, GLsizei, const GLint *);
    void  (*Uniform2fARB)(GLint, GLfloat, GLfloat);
    void  (*Uniform2fvARB)(GLint, GLsizei, const GLfloat *);
    void  (*Uniform2iARB)(GLint, GLint, GLint);
    void  (*Uniform2ivARB)(GLint, GLsizei, const GLint *);
    void  (*Uniform3fARB)(GLint, GLfloat, GLfloat, GLfloat);
    void  (*Uniform3fvARB)(GLint, GLsizei, const GLfloat *);
    void  (*Uniform3iARB)(GLint, GLint, GLint, GLint);
    void  (*Uniform3ivARB)(GLint, GLsizei, const GLint *);
    void  (*Uniform4fARB)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*Uniform4fvARB)(GLint, GLsizei, const GLfloat *);
    void  (*Uniform4iARB)(GLint, GLint, GLint, GLint, GLint);
    void  (*Uniform4ivARB)(GLint, GLsizei, const GLint *);
    void  (*UniformMatrix2fvARB)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*UniformMatrix3fvARB)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*UniformMatrix4fvARB)(GLint, GLsizei, GLboolean, const GLfloat *);
    void  (*UseProgramObjectARB)(GLhandleARB);
    void  (*ValidateProgramARB)(GLhandleARB);
    void  (*BindAttribLocationARB)(GLhandleARB, GLuint, const GLcharARB *);
    void  (*GetActiveAttribARB)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
    GLint  (*GetAttribLocationARB)(GLhandleARB, const GLcharARB *);
    void  (*DrawBuffersARB)(GLsizei, const GLenum *);
    void  (*ClampColorARB)(GLenum, GLenum);
    void  (*DrawArraysInstancedARB)(GLenum, GLint, GLsizei, GLsizei);
    void  (*DrawElementsInstancedARB)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei);
    void  (*RenderbufferStorageMultisample)(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
    void  (*FramebufferTextureARB)(GLenum, GLenum, GLuint, GLint);
    void  (*FramebufferTextureFaceARB)(GLenum, GLenum, GLuint, GLint, GLenum);
    void  (*ProgramParameteriARB)(GLuint, GLenum, GLint);
    void  (*VertexAttribDivisorARB)(GLuint, GLuint);
    void  (*FlushMappedBufferRange)(GLenum, GLintptr, GLsizeiptr);
    GLvoid *  (*MapBufferRange)(GLenum, GLintptr, GLsizeiptr, GLbitfield);
    void  (*TexBufferARB)(GLenum, GLenum, GLuint);
    void  (*BindVertexArray)(GLuint);
    void  (*GenVertexArrays)(GLsizei, GLuint *);
    void  (*GetActiveUniformBlockName)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *);
    void  (*GetActiveUniformBlockiv)(GLuint, GLuint, GLenum, GLint *);
    void  (*GetActiveUniformName)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *);
    void  (*GetActiveUniformsiv)(GLuint, GLsizei, const GLuint *, GLenum, GLint *);
    GLuint  (*GetUniformBlockIndex)(GLuint, const GLchar *);
    void  (*GetUniformIndices)(GLuint, GLsizei, const GLchar * const *, GLuint *);
    void  (*UniformBlockBinding)(GLuint, GLuint, GLuint);
    void  (*CopyBufferSubData)(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr);
    GLenum  (*ClientWaitSync)(GLsync, GLbitfield, GLuint64);
    void  (*DeleteSync)(GLsync);
    GLsync  (*FenceSync)(GLenum, GLbitfield);
    void  (*GetInteger64v)(GLenum, GLint64 *);
    void  (*GetSynciv)(GLsync, GLenum, GLsizei, GLsizei *, GLint *);
    GLboolean  (*IsSync)(GLsync);
    void  (*WaitSync)(GLsync, GLbitfield, GLuint64);
    void  (*DrawElementsBaseVertex)(GLenum, GLsizei, GLenum, const GLvoid *, GLint);
    void  (*DrawElementsInstancedBaseVertex)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei, GLint);
    void  (*DrawRangeElementsBaseVertex)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *, GLint);
    void  (*MultiDrawElementsBaseVertex)(GLenum, const GLsizei *, GLenum, const GLvoid * const *, GLsizei, const GLint *);
    void  (*BlendEquationSeparateiARB)(GLuint, GLenum, GLenum);
    void  (*BlendEquationiARB)(GLuint, GLenum);
    void  (*BlendFuncSeparateiARB)(GLuint, GLenum, GLenum, GLenum, GLenum);
    void  (*BlendFunciARB)(GLuint, GLenum, GLenum);
    void  (*BindFragDataLocationIndexed)(GLuint, GLuint, GLuint, const GLchar *);
    GLint  (*GetFragDataIndex)(GLuint, const GLchar *);
    void  (*BindSampler)(GLuint, GLuint);
    void  (*DeleteSamplers)(GLsizei, const GLuint *);
    void  (*GenSamplers)(GLsizei, GLuint *);
    void  (*GetSamplerParameterIiv)(GLuint, GLenum, GLint *);
    void  (*GetSamplerParameterIuiv)(GLuint, GLenum, GLuint *);
    void  (*GetSamplerParameterfv)(GLuint, GLenum, GLfloat *);
    void  (*GetSamplerParameteriv)(GLuint, GLenum, GLint *);
    GLboolean  (*IsSampler)(GLuint);
    void  (*SamplerParameterIiv)(GLuint, GLenum, const GLint *);
    void  (*SamplerParameterIuiv)(GLuint, GLenum, const GLuint *);
    void  (*SamplerParameterf)(GLuint, GLenum, GLfloat);
    void  (*SamplerParameterfv)(GLuint, GLenum, const GLfloat *);
    void  (*SamplerParameteri)(GLuint, GLenum, GLint);
    void  (*SamplerParameteriv)(GLuint, GLenum, const GLint *);
    void  (*QueryCounter)(GLuint, GLenum);
    void  (*ColorP3ui)(GLenum, GLuint);
    void  (*ColorP3uiv)(GLenum, const GLuint *);
    void  (*ColorP4ui)(GLenum, GLuint);
    void  (*ColorP4uiv)(GLenum, const GLuint *);
    void  (*MultiTexCoordP1ui)(GLenum, GLenum, GLuint);
    void  (*MultiTexCoordP1uiv)(GLenum, GLenum, const GLuint *);
    void  (*MultiTexCoordP2ui)(GLenum, GLenum, GLuint);
    void  (*MultiTexCoordP2uiv)(GLenum, GLenum, const GLuint *);
    void  (*MultiTexCoordP3ui)(GLenum, GLenum, GLuint);
    void  (*MultiTexCoordP3uiv)(GLenum, GLenum, const GLuint *);
    void  (*MultiTexCoordP4ui)(GLenum, GLenum, GLuint);
    void  (*MultiTexCoordP4uiv)(GLenum, GLenum, const GLuint *);
    void  (*NormalP3ui)(GLenum, GLuint);
    void  (*NormalP3uiv)(GLenum, const GLuint *);
    void  (*SecondaryColorP3ui)(GLenum, GLuint);
    void  (*SecondaryColorP3uiv)(GLenum, const GLuint *);
    void  (*TexCoordP1ui)(GLenum, GLuint);
    void  (*TexCoordP1uiv)(GLenum, const GLuint *);
    void  (*TexCoordP2ui)(GLenum, GLuint);
    void  (*TexCoordP2uiv)(GLenum, const GLuint *);
    void  (*TexCoordP3ui)(GLenum, GLuint);
    void  (*TexCoordP3uiv)(GLenum, const GLuint *);
    void  (*TexCoordP4ui)(GLenum, GLuint);
    void  (*TexCoordP4uiv)(GLenum, const GLuint *);
    void  (*VertexAttribP1ui)(GLuint, GLenum, GLboolean, GLuint);
    void  (*VertexAttribP1uiv)(GLuint, GLenum, GLboolean, const GLuint *);
    void  (*VertexAttribP2ui)(GLuint, GLenum, GLboolean, GLuint);
    void  (*VertexAttribP2uiv)(GLuint, GLenum, GLboolean, const GLuint *);
    void  (*VertexAttribP3ui)(GLuint, GLenum, GLboolean, GLuint);
    void  (*VertexAttribP3uiv)(GLuint, GLenum, GLboolean, const GLuint *);
    void  (*VertexAttribP4ui)(GLuint, GLenum, GLboolean, GLuint);
    void  (*VertexAttribP4uiv)(GLuint, GLenum, GLboolean, const GLuint *);
    void  (*VertexP2ui)(GLenum, GLuint);
    void  (*VertexP2uiv)(GLenum, const GLuint *);
    void  (*VertexP3ui)(GLenum, GLuint);
    void  (*VertexP3uiv)(GLenum, const GLuint *);
    void  (*VertexP4ui)(GLenum, GLuint);
    void  (*VertexP4uiv)(GLenum, const GLuint *);
    void  (*BindTransformFeedback)(GLenum, GLuint);
    void  (*DeleteTransformFeedbacks)(GLsizei, const GLuint *);
    void  (*DrawTransformFeedback)(GLenum, GLuint);
    void  (*GenTransformFeedbacks)(GLsizei, GLuint *);
    GLboolean  (*IsTransformFeedback)(GLuint);
    void  (*PauseTransformFeedback)(void);
    void  (*ResumeTransformFeedback)(void);
    void  (*BeginQueryIndexed)(GLenum, GLuint, GLuint);
    void  (*DrawTransformFeedbackStream)(GLenum, GLuint, GLuint);
    void  (*EndQueryIndexed)(GLenum, GLuint);
    void  (*GetQueryIndexediv)(GLenum, GLuint, GLenum, GLint *);
    void  (*DebugMessageCallbackARB)(GLDEBUGPROCARB, const GLvoid *);
    void  (*DebugMessageControlARB)(GLenum, GLenum, GLenum, GLsizei, const GLuint *, GLboolean);
    void  (*DebugMessageInsertARB)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLcharARB *);
    GLuint  (*GetDebugMessageLogARB)(GLuint, GLsizei, GLenum *, GLenum *, GLuint *, GLenum *, GLsizei *, GLcharARB *);
    GLenum  (*GetGraphicsResetStatusARB)(void);
    void  (*GetnColorTableARB)(GLenum, GLenum, GLenum, GLsizei, GLvoid *);
    void  (*GetnCompressedTexImageARB)(GLenum, GLint, GLsizei, GLvoid *);
    void  (*GetnConvolutionFilterARB)(GLenum, GLenum, GLenum, GLsizei, GLvoid *);
    void  (*GetnHistogramARB)(GLenum, GLboolean, GLenum, GLenum, GLsizei, GLvoid *);
    void  (*GetnMapdvARB)(GLenum, GLenum, GLsizei, GLdouble *);
    void  (*GetnMapfvARB)(GLenum, GLenum, GLsizei, GLfloat *);
    void  (*GetnMapivARB)(GLenum, GLenum, GLsizei, GLint *);
    void  (*GetnMinmaxARB)(GLenum, GLboolean, GLenum, GLenum, GLsizei, GLvoid *);
    void  (*GetnPixelMapfvARB)(GLenum, GLsizei, GLfloat *);
    void  (*GetnPixelMapuivARB)(GLenum, GLsizei, GLuint *);
    void  (*GetnPixelMapusvARB)(GLenum, GLsizei, GLushort *);
    void  (*GetnPolygonStippleARB)(GLsizei, GLubyte *);
    void  (*GetnSeparableFilterARB)(GLenum, GLenum, GLenum, GLsizei, GLvoid *, GLsizei, GLvoid *, GLvoid *);
    void  (*GetnTexImageARB)(GLenum, GLint, GLenum, GLenum, GLsizei, GLvoid *);
    void  (*GetnUniformdvARB)(GLhandleARB, GLint, GLsizei, GLdouble *);
    void  (*GetnUniformfvARB)(GLhandleARB, GLint, GLsizei, GLfloat *);
    void  (*GetnUniformivARB)(GLhandleARB, GLint, GLsizei, GLint *);
    void  (*GetnUniformuivARB)(GLhandleARB, GLint, GLsizei, GLuint *);
    void  (*ReadnPixelsARB)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLsizei, GLvoid *);
    void  (*DrawArraysInstancedBaseInstance)(GLenum, GLint, GLsizei, GLsizei, GLuint);
    void  (*DrawElementsInstancedBaseInstance)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei, GLuint);
    void  (*DrawElementsInstancedBaseVertexBaseInstance)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei, GLint, GLuint);
    void  (*DrawTransformFeedbackInstanced)(GLenum, GLuint, GLsizei);
    void  (*DrawTransformFeedbackStreamInstanced)(GLenum, GLuint, GLuint, GLsizei);
    void  (*TexStorage1D)(GLenum, GLsizei, GLenum, GLsizei);
    void  (*TexStorage2D)(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
    void  (*TexStorage3D)(GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei);
    void  (*TextureStorage1DEXT)(GLuint, GLenum, GLsizei, GLenum, GLsizei);
    void  (*TextureStorage2DEXT)(GLuint, GLenum, GLsizei, GLenum, GLsizei, GLsizei);
    void  (*TextureStorage3DEXT)(GLuint, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei);
    void  (*PolygonOffsetEXT)(GLfloat, GLfloat);
    void  (*SampleMaskSGIS)(GLclampf, GLboolean);
    void  (*SamplePatternSGIS)(GLenum);
    void  (*ColorPointerEXT)(GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
    void  (*EdgeFlagPointerEXT)(GLsizei, GLsizei, const GLboolean *);
    void  (*IndexPointerEXT)(GLenum, GLsizei, GLsizei, const GLvoid *);
    void  (*NormalPointerEXT)(GLenum, GLsizei, GLsizei, const GLvoid *);
    void  (*TexCoordPointerEXT)(GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
    void  (*VertexPointerEXT)(GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
    void  (*PointParameterfEXT)(GLenum, GLfloat);
    void  (*PointParameterfvEXT)(GLenum, const GLfloat *);
    void  (*LockArraysEXT)(GLint, GLsizei);
    void  (*UnlockArraysEXT)(void);
    void  (*SecondaryColor3bEXT)(GLbyte, GLbyte, GLbyte);
    void  (*SecondaryColor3bvEXT)(const GLbyte *);
    void  (*SecondaryColor3dEXT)(GLdouble, GLdouble, GLdouble);
    void  (*SecondaryColor3dvEXT)(const GLdouble *);
    void  (*SecondaryColor3fEXT)(GLfloat, GLfloat, GLfloat);
    void  (*SecondaryColor3fvEXT)(const GLfloat *);
    void  (*SecondaryColor3iEXT)(GLint, GLint, GLint);
    void  (*SecondaryColor3ivEXT)(const GLint *);
    void  (*SecondaryColor3sEXT)(GLshort, GLshort, GLshort);
    void  (*SecondaryColor3svEXT)(const GLshort *);
    void  (*SecondaryColor3ubEXT)(GLubyte, GLubyte, GLubyte);
    void  (*SecondaryColor3ubvEXT)(const GLubyte *);
    void  (*SecondaryColor3uiEXT)(GLuint, GLuint, GLuint);
    void  (*SecondaryColor3uivEXT)(const GLuint *);
    void  (*SecondaryColor3usEXT)(GLushort, GLushort, GLushort);
    void  (*SecondaryColor3usvEXT)(const GLushort *);
    void  (*SecondaryColorPointerEXT)(GLint, GLenum, GLsizei, const GLvoid *);
    void  (*MultiDrawArraysEXT)(GLenum, const GLint *, const GLsizei *, GLsizei);
    void  (*MultiDrawElementsEXT)(GLenum, const GLsizei *, GLenum, const GLvoid **, GLsizei);
    void  (*FogCoordPointerEXT)(GLenum, GLsizei, const GLvoid *);
    void  (*FogCoorddEXT)(GLdouble);
    void  (*FogCoorddvEXT)(const GLdouble *);
    void  (*FogCoordfEXT)(GLfloat);
    void  (*FogCoordfvEXT)(const GLfloat *);
    void  (*ResizeBuffersMESA)(void);
    void  (*WindowPos2dMESA)(GLdouble, GLdouble);
    void  (*WindowPos2dvMESA)(const GLdouble *);
    void  (*WindowPos2fMESA)(GLfloat, GLfloat);
    void  (*WindowPos2fvMESA)(const GLfloat *);
    void  (*WindowPos2iMESA)(GLint, GLint);
    void  (*WindowPos2ivMESA)(const GLint *);
    void  (*WindowPos2sMESA)(GLshort, GLshort);
    void  (*WindowPos2svMESA)(const GLshort *);
    void  (*WindowPos3dMESA)(GLdouble, GLdouble, GLdouble);
    void  (*WindowPos3dvMESA)(const GLdouble *);
    void  (*WindowPos3fMESA)(GLfloat, GLfloat, GLfloat);
    void  (*WindowPos3fvMESA)(const GLfloat *);
    void  (*WindowPos3iMESA)(GLint, GLint, GLint);
    void  (*WindowPos3ivMESA)(const GLint *);
    void  (*WindowPos3sMESA)(GLshort, GLshort, GLshort);
    void  (*WindowPos3svMESA)(const GLshort *);
    void  (*WindowPos4dMESA)(GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*WindowPos4dvMESA)(const GLdouble *);
    void  (*WindowPos4fMESA)(GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*WindowPos4fvMESA)(const GLfloat *);
    void  (*WindowPos4iMESA)(GLint, GLint, GLint, GLint);
    void  (*WindowPos4ivMESA)(const GLint *);
    void  (*WindowPos4sMESA)(GLshort, GLshort, GLshort, GLshort);
    void  (*WindowPos4svMESA)(const GLshort *);
    void  (*MultiModeDrawArraysIBM)(const GLenum *, const GLint *, const GLsizei *, GLsizei, GLint);
    void  (*MultiModeDrawElementsIBM)(const GLenum *, const GLsizei *, GLenum, const GLvoid * const *, GLsizei, GLint);
    GLboolean  (*AreProgramsResidentNV)(GLsizei, const GLuint *, GLboolean *);
    void  (*BindProgramNV)(GLenum, GLuint);
    void  (*DeleteProgramsNV)(GLsizei, const GLuint *);
    void  (*ExecuteProgramNV)(GLenum, GLuint, const GLfloat *);
    void  (*GenProgramsNV)(GLsizei, GLuint *);
    void  (*GetProgramParameterdvNV)(GLenum, GLuint, GLenum, GLdouble *);
    void  (*GetProgramParameterfvNV)(GLenum, GLuint, GLenum, GLfloat *);
    void  (*GetProgramStringNV)(GLuint, GLenum, GLubyte *);
    void  (*GetProgramivNV)(GLuint, GLenum, GLint *);
    void  (*GetTrackMatrixivNV)(GLenum, GLuint, GLenum, GLint *);
    void  (*GetVertexAttribPointervNV)(GLuint, GLenum, GLvoid **);
    void  (*GetVertexAttribdvNV)(GLuint, GLenum, GLdouble *);
    void  (*GetVertexAttribfvNV)(GLuint, GLenum, GLfloat *);
    void  (*GetVertexAttribivNV)(GLuint, GLenum, GLint *);
    GLboolean  (*IsProgramNV)(GLuint);
    void  (*LoadProgramNV)(GLenum, GLuint, GLsizei, const GLubyte *);
    void  (*ProgramParameters4dvNV)(GLenum, GLuint, GLsizei, const GLdouble *);
    void  (*ProgramParameters4fvNV)(GLenum, GLuint, GLsizei, const GLfloat *);
    void  (*RequestResidentProgramsNV)(GLsizei, const GLuint *);
    void  (*TrackMatrixNV)(GLenum, GLuint, GLenum, GLenum);
    void  (*VertexAttrib1dNV)(GLuint, GLdouble);
    void  (*VertexAttrib1dvNV)(GLuint, const GLdouble *);
    void  (*VertexAttrib1fNV)(GLuint, GLfloat);
    void  (*VertexAttrib1fvNV)(GLuint, const GLfloat *);
    void  (*VertexAttrib1sNV)(GLuint, GLshort);
    void  (*VertexAttrib1svNV)(GLuint, const GLshort *);
    void  (*VertexAttrib2dNV)(GLuint, GLdouble, GLdouble);
    void  (*VertexAttrib2dvNV)(GLuint, const GLdouble *);
    void  (*VertexAttrib2fNV)(GLuint, GLfloat, GLfloat);
    void  (*VertexAttrib2fvNV)(GLuint, const GLfloat *);
    void  (*VertexAttrib2sNV)(GLuint, GLshort, GLshort);
    void  (*VertexAttrib2svNV)(GLuint, const GLshort *);
    void  (*VertexAttrib3dNV)(GLuint, GLdouble, GLdouble, GLdouble);
    void  (*VertexAttrib3dvNV)(GLuint, const GLdouble *);
    void  (*VertexAttrib3fNV)(GLuint, GLfloat, GLfloat, GLfloat);
    void  (*VertexAttrib3fvNV)(GLuint, const GLfloat *);
    void  (*VertexAttrib3sNV)(GLuint, GLshort, GLshort, GLshort);
    void  (*VertexAttrib3svNV)(GLuint, const GLshort *);
    void  (*VertexAttrib4dNV)(GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*VertexAttrib4dvNV)(GLuint, const GLdouble *);
    void  (*VertexAttrib4fNV)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*VertexAttrib4fvNV)(GLuint, const GLfloat *);
    void  (*VertexAttrib4sNV)(GLuint, GLshort, GLshort, GLshort, GLshort);
    void  (*VertexAttrib4svNV)(GLuint, const GLshort *);
    void  (*VertexAttrib4ubNV)(GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
    void  (*VertexAttrib4ubvNV)(GLuint, const GLubyte *);
    void  (*VertexAttribPointerNV)(GLuint, GLint, GLenum, GLsizei, const GLvoid *);
    void  (*VertexAttribs1dvNV)(GLuint, GLsizei, const GLdouble *);
    void  (*VertexAttribs1fvNV)(GLuint, GLsizei, const GLfloat *);
    void  (*VertexAttribs1svNV)(GLuint, GLsizei, const GLshort *);
    void  (*VertexAttribs2dvNV)(GLuint, GLsizei, const GLdouble *);
    void  (*VertexAttribs2fvNV)(GLuint, GLsizei, const GLfloat *);
    void  (*VertexAttribs2svNV)(GLuint, GLsizei, const GLshort *);
    void  (*VertexAttribs3dvNV)(GLuint, GLsizei, const GLdouble *);
    void  (*VertexAttribs3fvNV)(GLuint, GLsizei, const GLfloat *);
    void  (*VertexAttribs3svNV)(GLuint, GLsizei, const GLshort *);
    void  (*VertexAttribs4dvNV)(GLuint, GLsizei, const GLdouble *);
    void  (*VertexAttribs4fvNV)(GLuint, GLsizei, const GLfloat *);
    void  (*VertexAttribs4svNV)(GLuint, GLsizei, const GLshort *);
    void  (*VertexAttribs4ubvNV)(GLuint, GLsizei, const GLubyte *);
    void  (*GetTexBumpParameterfvATI)(GLenum, GLfloat *);
    void  (*GetTexBumpParameterivATI)(GLenum, GLint *);
    void  (*TexBumpParameterfvATI)(GLenum, const GLfloat *);
    void  (*TexBumpParameterivATI)(GLenum, const GLint *);
    void  (*AlphaFragmentOp1ATI)(GLenum, GLuint, GLuint, GLuint, GLuint, GLuint);
    void  (*AlphaFragmentOp2ATI)(GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
    void  (*AlphaFragmentOp3ATI)(GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
    void  (*BeginFragmentShaderATI)(void);
    void  (*BindFragmentShaderATI)(GLuint);
    void  (*ColorFragmentOp1ATI)(GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
    void  (*ColorFragmentOp2ATI)(GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
    void  (*ColorFragmentOp3ATI)(GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
    void  (*DeleteFragmentShaderATI)(GLuint);
    void  (*EndFragmentShaderATI)(void);
    GLuint  (*GenFragmentShadersATI)(GLuint);
    void  (*PassTexCoordATI)(GLuint, GLuint, GLenum);
    void  (*SampleMapATI)(GLuint, GLuint, GLenum);
    void  (*SetFragmentShaderConstantATI)(GLuint, const GLfloat *);
    void  (*PointParameteriNV)(GLenum, GLint);
    void  (*PointParameterivNV)(GLenum, const GLint *);
    void  (*ActiveStencilFaceEXT)(GLenum);
    void  (*BindVertexArrayAPPLE)(GLuint);
    void  (*DeleteVertexArraysAPPLE)(GLsizei, const GLuint *);
    void  (*GenVertexArraysAPPLE)(GLsizei, GLuint *);
    GLboolean  (*IsVertexArrayAPPLE)(GLuint);
    void  (*GetProgramNamedParameterdvNV)(GLuint, GLsizei, const GLubyte *, GLdouble *);
    void  (*GetProgramNamedParameterfvNV)(GLuint, GLsizei, const GLubyte *, GLfloat *);
    void  (*ProgramNamedParameter4dNV)(GLuint, GLsizei, const GLubyte *, GLdouble, GLdouble, GLdouble, GLdouble);
    void  (*ProgramNamedParameter4dvNV)(GLuint, GLsizei, const GLubyte *, const GLdouble *);
    void  (*ProgramNamedParameter4fNV) (GLuint, GLsizei, const GLubyte *, GLfloat, GLfloat, GLfloat, GLfloat);
    void  (*ProgramNamedParameter4fvNV)(GLuint, GLsizei, const GLubyte *, const GLfloat *);
    void  (*PrimitiveRestartIndexNV)(GLuint);
    void  (*PrimitiveRestartNV)(void);
    void  (*DepthBoundsEXT)(GLclampd, GLclampd);
    void  (*FramebufferRenderbufferEXT)(GLenum, GLenum, GLenum, GLuint);
    void  (*FramebufferTexture1DEXT)(GLenum, GLenum, GLenum, GLuint, GLint);
    void  (*FramebufferTexture2DEXT)(GLenum, GLenum, GLenum, GLuint, GLint);
    void  (*FramebufferTexture3DEXT)(GLenum, GLenum, GLenum, GLuint, GLint, GLint);
    void  (*GenFramebuffersEXT)(GLsizei, GLuint *);
    void  (*GenRenderbuffersEXT)(GLsizei, GLuint *);
    void  (*GenerateMipmapEXT)(GLenum);
    void  (*GetFramebufferAttachmentParameterivEXT)(GLenum, GLenum, GLenum, GLint *);
    void  (*GetRenderbufferParameterivEXT)(GLenum, GLenum, GLint *);
    GLboolean (*IsFramebufferEXT)(GLuint);
    GLboolean  (*IsRenderbufferEXT)(GLuint);
    void  (*RenderbufferStorageEXT)(GLenum, GLenum, GLsizei, GLsizei);
    void  (*BlitFramebufferEXT)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
    void  (*BufferParameteriAPPLE)(GLenum, GLenum, GLint);
    void  (*FlushMappedBufferRangeAPPLE)(GLenum, GLintptr, GLsizeiptr);
    void  (*BindFragDataLocationEXT)(GLuint, GLuint, const GLchar *);
    GLint  (*GetFragDataLocationEXT)(GLuint, const GLchar *);
    void  (*GetUniformuivEXT)(GLuint, GLint, GLuint *);
    void  (*GetVertexAttribIivEXT)(GLuint, GLenum, GLint *);
    void  (*GetVertexAttribIuivEXT)(GLuint, GLenum, GLuint *);
    void  (*Uniform1uiEXT)(GLint, GLuint);
    void  (*Uniform1uivEXT)(GLint, GLsizei, const GLuint *);
    void  (*Uniform2uiEXT)(GLint, GLuint, GLuint);
    void  (*Uniform2uivEXT)(GLint, GLsizei, const GLuint *);
    void  (*Uniform3uiEXT)(GLint, GLuint, GLuint, GLuint);
    void  (*Uniform3uivEXT)(GLint, GLsizei, const GLuint *);
    void  (*Uniform4uiEXT)(GLint, GLuint, GLuint, GLuint, GLuint);
    void  (*Uniform4uivEXT)(GLint, GLsizei, const GLuint *);
    void  (*VertexAttribI1iEXT)(GLuint, GLint);
    void  (*VertexAttribI1ivEXT)(GLuint, const GLint *);
    void  (*VertexAttribI1uiEXT)(GLuint, GLuint);
    void  (*VertexAttribI1uivEXT)(GLuint, const GLuint *);
    void  (*VertexAttribI2iEXT)(GLuint, GLint, GLint);
    void  (*VertexAttribI2ivEXT)(GLuint, const GLint *);
    void  (*VertexAttribI2uiEXT)(GLuint, GLuint, GLuint);
    void  (*VertexAttribI2uivEXT)(GLuint, const GLuint *);
    void  (*VertexAttribI3iEXT)(GLuint, GLint, GLint, GLint);
    void  (*VertexAttribI3uiEXT)(GLuint, GLuint, GLuint, GLuint);
    void  (*VertexAttribI3ivEXT)(GLuint, const GLint *);
    void  (*VertexAttribI3uivEXT)(GLuint, const GLuint *);
    void  (*VertexAttribI4bvEXT)(GLuint, const GLbyte *);
    void  (*VertexAttribI4iEXT)(GLuint, GLint, GLint, GLint, GLint);
    void  (*VertexAttribI4ivEXT)(GLuint, const GLint *);
    void  (*VertexAttribI4svEXT)(GLuint, const GLshort *);
    void  (*VertexAttribI4ubvEXT)(GLuint, const GLubyte *);
    void  (*VertexAttribI4uiEXT)(GLuint, GLuint, GLuint, GLuint, GLuint);
    void  (*VertexAttribI4uivEXT)(GLuint, const GLuint *);
    void  (*VertexAttribI4usvEXT)(GLuint, const GLushort *);
    void  (*VertexAttribIPointerEXT)(GLuint, GLint, GLenum, GLsizei, const GLvoid *);
    void  (*FramebufferTextureLayerEXT)(GLenum, GLenum, GLuint, GLint, GLint);
    void  (*ColorMaskIndexedEXT)(GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
    void  (*DisableIndexedEXT)(GLenum, GLuint);
    void  (*EnableIndexedEXT)(GLenum, GLuint);
    void  (*GetBooleanIndexedvEXT)(GLenum, GLuint, GLboolean *);
    void  (*GetIntegerIndexedvEXT)(GLenum, GLuint, GLint *);
    GLboolean  (*IsEnabledIndexedEXT)(GLenum, GLuint);
    void  (*ClearColorIiEXT)(GLint, GLint, GLint, GLint);
    void  (*ClearColorIuiEXT)(GLuint, GLuint, GLuint, GLuint);
    void  (*GetTexParameterIuivEXT)(GLenum, GLenum, GLuint *);
    void  (*GetTexParameterIivEXT)(GLenum, GLenum, GLint *);
    void  (*TexParameterIivEXT)(GLenum, GLenum, const GLint *);
    void  (*TexParameterIuivEXT)(GLenum, GLenum, const GLuint *);
    void  (*BeginConditionalRenderNV)(GLuint, GLenum);
    void  (*EndConditionalRenderNV)(void);
    void  (*BeginTransformFeedbackEXT)(GLenum);
    void  (*BindBufferBaseEXT)(GLenum, GLuint, GLuint);
    void  (*BindBufferOffsetEXT)(GLenum, GLuint, GLuint, GLintptr);
    void  (*BindBufferRangeEXT)(GLenum, GLuint, GLuint, GLintptr, GLsizeiptr);
    void  (*EndTransformFeedbackEXT)(void);
    void  (*GetTransformFeedbackVaryingEXT)(GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *);
    void  (*TransformFeedbackVaryingsEXT)(GLuint, GLsizei, const char **, GLenum);
    void  (*ProvokingVertexEXT)(GLenum);
    void  (*GetObjectParameterivAPPLE)(GLenum, GLuint, GLenum, GLint *);
    GLenum  (*ObjectPurgeableAPPLE)(GLenum, GLuint, GLenum);
    GLenum  (*ObjectUnpurgeableAPPLE)(GLenum, GLuint, GLenum);
    void  (*ActiveProgramEXT)(GLuint);
    GLuint  (*CreateShaderProgramEXT)(GLenum, const GLchar *);
    void  (*UseShaderProgramEXT)(GLenum, GLuint);
    void  (*TextureBarrierNV)(void);
    void  (*StencilFuncSeparateATI)(GLenum, GLenum, GLint, GLuint);
    void  (*ProgramEnvParameters4fvEXT)(GLenum, GLuint, GLsizei, const GLfloat *);
    void  (*ProgramLocalParameters4fvEXT)(GLenum, GLuint, GLsizei, const GLfloat *);
    void  (*GetQueryObjecti64vEXT)(GLuint, GLenum, GLint64EXT *);
    void  (*GetQueryObjectui64vEXT)(GLuint, GLenum, GLuint64EXT *);
    void  (*EGLImageTargetRenderbufferStorageOES)(GLenum, GLvoid *);
#endif
} gpuprocess_dispatch_t;

gpuprocess_private void
gpuprocess_dispatch_init(gpuprocess_dispatch_t *dispatch);

#endif