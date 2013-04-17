import json
import os
import os.path
import sys
import re
from abc import ABCMeta, abstractmethod
from optparse import OptionParser
from functionutils import *
from writer import *

FUNCTIONS_GENERATING_ERRORS = [
 'glAttachShader',
 'glBindAttribLocation',
 'glBindBuffer',
 'glBufferData',
 'glBufferSubData',
 'glCompileShader',
 'glCompressedTexImage2D',
 'glCompressedTexSubImage2D',
 'glCopyTexImage2D',
 'glCopyTexSubImage2D',
 'glDetachShader',
 'glFramebufferRenderbuffer',
 'glFramebufferTexture2D',
 'glGenerateMipmap',
 'glGetBufferParameteriv',
 'glGetIntegerv',
 'glGetProgramInfoLog',
 'glGetShaderInfoLog',
 'glGetShaderiv',
 'glGetShaderPrecisionFormat',
 'glGetShaderSource',
 'glLinkProgram',
 'glReadPixels',
 'glReleaseShaderCompiler',
 'glRenderbufferStorage',
 'glShaderBinary',
 'glValidateProgram',
 'glVertexAttribPointer',
 'glBindVertexArrayOES',
 'glEGLImageTargetTexture2DOES',
 'glGetProgramBinaryOES',
 'glProgramBinaryOES',
 'glGetBufferPointervOES',
 'glTexImage3DOES',
 'glTexSubImage3DOES',
 'glCopyTexSubImage3DOES',
 'glCompressedTexImage3DOES',
 'glCompressedTexSubImage3DOES',
 'glFramebufferTexture2DMultisampleEXT',
 'glFramebufferTexture2DMultisampleIMG',
 'glFramebufferTexture3DOES',
 'glBeginPerfMonitorAMD',
 'glGetPerfMonitorGroupsAMD',
 'glGetPerfMonitorCountersAMD',
 'glGetPerfMonitorGroupStringAMD',
 'glGetPerfMonitorCounterStringAMD',
 'glGetPerfMonitorCounterInfoAMD',
 'glGenPerfMonitorsAMD',
 'glEndPerfMonitorAMD',
 'glDeletePerfMonitorsAMD',
 'glSelectPerfMonitorCountersAMD',
 'glGetPerfMonitorCounterDataAMD',
 'glBlitFramebufferANGLE',
 'glRenderbufferStorageMultisampleANGLE',
 'glRenderbufferStorageMultisampleAPPLE',
 'glResolveMultisampleFramebufferAPPLE',
 'glRenderbufferStorageMultisampleEXT',
 'glRenderbufferStorageMultisampleIMG',
 'glSetFenceNV',
 'glFinishFenceNV',
 'glCoverageMaskNV',
 'glGetDriverControlsQCOM',
 'glEnableDriverControlQCOM',
 'glDisableDriverControlQCOM',
 'glExtTexObjectStateOverrideiQCOM',
 'glGetDriverControlStringQCOM',
 'glExtGetTexLevelParameterivQCOM',
 'glExtGetTexSubImageQCOM',
 'glExtGetBufferPointervQCOM',
 'glExtIsProgramBinaryQCOM',
 'glExtGetProgramBinarySourceQCOM',
 'glStartTilingQCOM',
 'glEndTilingQCOM',
]

# This is a list of enum names and their valid values. It is used to map
# GLenum arguments to a specific set of valid values.
_ENUM_LISTS = {
  'BlitFilter': {
    'type': 'GLenum',
    'valid': [
      'GL_NEAREST',
      'GL_LINEAR',
    ],
    'invalid': [
      'GL_LINEAR_MIPMAP_LINEAR',
    ],
  },
  'FrameBufferTarget': {
    'type': 'GLenum',
    'valid': [
      'GL_FRAMEBUFFER',
    ],
    'invalid': [
      'GL_DRAW_FRAMEBUFFER' ,
      'GL_READ_FRAMEBUFFER' ,
    ],
  },
  'RenderBufferTarget': {
    'type': 'GLenum',
    'valid': [
      'GL_RENDERBUFFER',
    ],
    'invalid': [
      'GL_FRAMEBUFFER',
    ],
  },
  'BufferTarget': {
    'type': 'GLenum',
    'valid': [
      'GL_ARRAY_BUFFER',
      'GL_ELEMENT_ARRAY_BUFFER',
    ],
    'invalid': [
      'GL_RENDERBUFFER',
    ],
  },
  'BufferUsage': {
    'type': 'GLenum',
    'valid': [
      'GL_STREAM_DRAW',
      'GL_STATIC_DRAW',
      'GL_DYNAMIC_DRAW',
    ],
    'invalid': [
      'GL_STATIC_READ',
    ],
  },
  'GLState': {
    'type': 'GLenum',
    'valid': [
      'GL_ACTIVE_TEXTURE',
      'GL_ALIASED_LINE_WIDTH_RANGE',
      'GL_ALIASED_POINT_SIZE_RANGE',
      'GL_ALPHA_BITS',
      'GL_ARRAY_BUFFER_BINDING',
      'GL_BLEND',
      'GL_BLEND_COLOR',
      'GL_BLEND_DST_ALPHA',
      'GL_BLEND_DST_RGB',
      'GL_BLEND_EQUATION_ALPHA',
      'GL_BLEND_EQUATION_RGB',
      'GL_BLEND_SRC_ALPHA',
      'GL_BLEND_SRC_RGB',
      'GL_BLUE_BITS',
      'GL_COLOR_CLEAR_VALUE',
      'GL_COLOR_WRITEMASK',
      'GL_COMPRESSED_TEXTURE_FORMATS',
      'GL_CULL_FACE',
      'GL_CULL_FACE_MODE',
      'GL_CURRENT_PROGRAM',
      'GL_DEPTH_BITS',
      'GL_DEPTH_CLEAR_VALUE',
      'GL_DEPTH_FUNC',
      'GL_DEPTH_RANGE',
      'GL_DEPTH_TEST',
      'GL_DEPTH_WRITEMASK',
      'GL_DITHER',
      'GL_ELEMENT_ARRAY_BUFFER_BINDING',
      'GL_FRAMEBUFFER_BINDING',
      'GL_FRONT_FACE',
      'GL_GENERATE_MIPMAP_HINT',
      'GL_GREEN_BITS',
      'GL_IMPLEMENTATION_COLOR_READ_FORMAT',
      'GL_IMPLEMENTATION_COLOR_READ_TYPE',
      'GL_LINE_WIDTH',
      'GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS',
      'GL_MAX_CUBE_MAP_TEXTURE_SIZE',
      'GL_MAX_FRAGMENT_UNIFORM_VECTORS',
      'GL_MAX_RENDERBUFFER_SIZE',
      'GL_MAX_TEXTURE_IMAGE_UNITS',
      'GL_MAX_TEXTURE_SIZE',
      'GL_MAX_VARYING_VECTORS',
      'GL_MAX_VERTEX_ATTRIBS',
      'GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS',
      'GL_MAX_VERTEX_UNIFORM_VECTORS',
      'GL_MAX_VIEWPORT_DIMS',
      'GL_NUM_COMPRESSED_TEXTURE_FORMATS',
      'GL_NUM_SHADER_BINARY_FORMATS',
      'GL_PACK_ALIGNMENT',
      'GL_POLYGON_OFFSET_FACTOR',
      'GL_POLYGON_OFFSET_FILL',
      'GL_POLYGON_OFFSET_UNITS',
      'GL_RED_BITS',
      'GL_RENDERBUFFER_BINDING',
      'GL_SAMPLE_BUFFERS',
      'GL_SAMPLE_COVERAGE_INVERT',
      'GL_SAMPLE_COVERAGE_VALUE',
      'GL_SAMPLES',
      'GL_SCISSOR_BOX',
      'GL_SCISSOR_TEST',
      'GL_SHADER_BINARY_FORMATS',
      'GL_SHADER_COMPILER',
      'GL_STENCIL_BACK_FAIL',
      'GL_STENCIL_BACK_FUNC',
      'GL_STENCIL_BACK_PASS_DEPTH_FAIL',
      'GL_STENCIL_BACK_PASS_DEPTH_PASS',
      'GL_STENCIL_BACK_REF',
      'GL_STENCIL_BACK_VALUE_MASK',
      'GL_STENCIL_BACK_WRITEMASK',
      'GL_STENCIL_BITS',
      'GL_STENCIL_CLEAR_VALUE',
      'GL_STENCIL_FAIL',
      'GL_STENCIL_FUNC',
      'GL_STENCIL_PASS_DEPTH_FAIL',
      'GL_STENCIL_PASS_DEPTH_PASS',
      'GL_STENCIL_REF',
      'GL_STENCIL_TEST',
      'GL_STENCIL_VALUE_MASK',
      'GL_STENCIL_WRITEMASK',
      'GL_SUBPIXEL_BITS',
      'GL_TEXTURE_BINDING_2D',
      'GL_TEXTURE_BINDING_CUBE_MAP',
      'GL_UNPACK_ALIGNMENT',
      'GL_VIEWPORT',
    ],
    'invalid': [
      'GL_FOG_HINT',
    ],
  },
  'GetTexParamTarget': {
    'type': 'GLenum',
    'valid': [
      'GL_TEXTURE_2D',
      'GL_TEXTURE_CUBE_MAP',
      'GL_TEXTURE_3D_OES',
    ],
    'invalid': [
      'GL_PROXY_TEXTURE_CUBE_MAP',
    ]
  },
  'TextureTarget': {
    'type': 'GLenum',
    'valid': [
      'GL_TEXTURE_2D',
      'GL_TEXTURE_CUBE_MAP_POSITIVE_X',
      'GL_TEXTURE_CUBE_MAP_NEGATIVE_X',
      'GL_TEXTURE_CUBE_MAP_POSITIVE_Y',
      'GL_TEXTURE_CUBE_MAP_NEGATIVE_Y',
      'GL_TEXTURE_CUBE_MAP_POSITIVE_Z',
      'GL_TEXTURE_CUBE_MAP_NEGATIVE_Z',
    ],
    'invalid': [
      'GL_PROXY_TEXTURE_CUBE_MAP',
    ]
  },
  'TextureBindTarget': {
    'type': 'GLenum',
    'valid': [
      'GL_TEXTURE_2D',
      'GL_TEXTURE_CUBE_MAP',
      'GL_TEXTURE_3D_OES',
    ],
    'invalid': [
      'GL_TEXTURE_1D',
      'GL_TEXTURE_3D',
    ],
  },
  'ShaderType': {
    'type': 'GLenum',
    'valid': [
      'GL_VERTEX_SHADER',
      'GL_FRAGMENT_SHADER',
    ],
    'invalid': [
      'GL_GEOMETRY_SHADER',
    ],
  },
  'FaceType': {
    'type': 'GLenum',
    'valid': [
      'GL_FRONT',
      'GL_BACK',
      'GL_FRONT_AND_BACK',
    ],
  },
  'FaceMode': {
    'type': 'GLenum',
    'valid': [
      'GL_CW',
      'GL_CCW',
    ],
  },
  'CmpFunction': {
    'type': 'GLenum',
    'valid': [
      'GL_NEVER',
      'GL_LESS',
      'GL_EQUAL',
      'GL_LEQUAL',
      'GL_GREATER',
      'GL_NOTEQUAL',
      'GL_GEQUAL',
      'GL_ALWAYS',
    ],
  },
  'Equation': {
    'type': 'GLenum',
    'valid': [
      'GL_FUNC_ADD',
      'GL_FUNC_SUBTRACT',
      'GL_FUNC_REVERSE_SUBTRACT',
    ],
    'invalid': [
      'GL_MIN',
      'GL_MAX',
    ],
  },
  'SrcBlendFactor': {
    'type': 'GLenum',
    'valid': [
      'GL_ZERO',
      'GL_ONE',
      'GL_SRC_COLOR',
      'GL_ONE_MINUS_SRC_COLOR',
      'GL_DST_COLOR',
      'GL_ONE_MINUS_DST_COLOR',
      'GL_SRC_ALPHA',
      'GL_ONE_MINUS_SRC_ALPHA',
      'GL_DST_ALPHA',
      'GL_ONE_MINUS_DST_ALPHA',
      'GL_CONSTANT_COLOR',
      'GL_ONE_MINUS_CONSTANT_COLOR',
      'GL_CONSTANT_ALPHA',
      'GL_ONE_MINUS_CONSTANT_ALPHA',
      'GL_SRC_ALPHA_SATURATE',
    ],
  },
  'DstBlendFactor': {
    'type': 'GLenum',
    'valid': [
      'GL_ZERO',
      'GL_ONE',
      'GL_SRC_COLOR',
      'GL_ONE_MINUS_SRC_COLOR',
      'GL_DST_COLOR',
      'GL_ONE_MINUS_DST_COLOR',
      'GL_SRC_ALPHA',
      'GL_ONE_MINUS_SRC_ALPHA',
      'GL_DST_ALPHA',
      'GL_ONE_MINUS_DST_ALPHA',
      'GL_CONSTANT_COLOR',
      'GL_ONE_MINUS_CONSTANT_COLOR',
      'GL_CONSTANT_ALPHA',
      'GL_ONE_MINUS_CONSTANT_ALPHA',
    ],
  },
  'Capability': {
    'type': 'GLenum',
    'valid': [
      'GL_DITHER',  # 1st one is a non-cached value so autogen unit tests work.
      'GL_BLEND',
      'GL_CULL_FACE',
      'GL_DEPTH_TEST',
      'GL_POLYGON_OFFSET_FILL',
      'GL_SAMPLE_ALPHA_TO_COVERAGE',
      'GL_SAMPLE_COVERAGE',
      'GL_SCISSOR_TEST',
      'GL_STENCIL_TEST',
    ],
    'invalid': [
      'GL_CLIP_PLANE0',
      'GL_POINT_SPRITE',
    ],
  },
  'DrawMode': {
    'type': 'GLenum',
    'valid': [
      'GL_POINTS',
      'GL_LINE_STRIP',
      'GL_LINE_LOOP',
      'GL_LINES',
      'GL_TRIANGLE_STRIP',
      'GL_TRIANGLE_FAN',
      'GL_TRIANGLES',
    ],
    'invalid': [
      'GL_QUADS',
      'GL_POLYGON',
    ],
  },
  'IndexType': {
    'type': 'GLenum',
    'valid': [
      'GL_UNSIGNED_BYTE',
      'GL_UNSIGNED_SHORT',
    ],
    'invalid': [
      'GL_UNSIGNED_INT',
      'GL_INT',
    ],
  },
  'GetMaxIndexType': {
    'type': 'GLenum',
    'valid': [
      'GL_UNSIGNED_BYTE',
      'GL_UNSIGNED_SHORT',
      'GL_UNSIGNED_INT',
    ],
    'invalid': [
      'GL_INT',
    ],
  },
  'Attachment': {
    'type': 'GLenum',
    'valid': [
      'GL_COLOR_ATTACHMENT0',
      'GL_DEPTH_ATTACHMENT',
      'GL_STENCIL_ATTACHMENT',
       'GL_COLOR_EXT',
       'GL_DEPTH_EXT',
       'GL_STENCIL_EXT',
    ],
  },
  'BufferParameter': {
    'type': 'GLenum',
    'valid': [
      'GL_BUFFER_SIZE',
      'GL_BUFFER_USAGE',
    ],
    'invalid': [
      'GL_PIXEL_PACK_BUFFER',
    ],
  },
  'FrameBufferParameter': {
    'type': 'GLenum',
    'valid': [
      'GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE',
      'GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME',
      'GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL',
      'GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE',
    ],
  },
  'ProgramParameter': {
    'type': 'GLenum',
    'valid': [
      'GL_DELETE_STATUS',
      'GL_LINK_STATUS',
      'GL_VALIDATE_STATUS',
      'GL_INFO_LOG_LENGTH',
      'GL_ATTACHED_SHADERS',
      'GL_ACTIVE_ATTRIBUTES',
      'GL_ACTIVE_ATTRIBUTE_MAX_LENGTH',
      'GL_ACTIVE_UNIFORMS',
      'GL_ACTIVE_UNIFORM_MAX_LENGTH',
    ],
  },
  'QueryObjectParameter': {
    'type': 'GLenum',
    'valid': [
      'GL_QUERY_RESULT_EXT',
      'GL_QUERY_RESULT_AVAILABLE_EXT',
    ],
  },
  'QueryParameter': {
    'type': 'GLenum',
    'valid': [
      'GL_CURRENT_QUERY_EXT',
    ],
  },
  'QueryTarget': {
    'type': 'GLenum',
    'valid': [
      'GL_ANY_SAMPLES_PASSED_EXT',
      'GL_ANY_SAMPLES_PASSED_CONSERVATIVE_EXT',
    ],
  },
  'RenderBufferParameter': {
    'type': 'GLenum',
    'valid': [
      'GL_RENDERBUFFER_RED_SIZE',
      'GL_RENDERBUFFER_GREEN_SIZE',
      'GL_RENDERBUFFER_BLUE_SIZE',
      'GL_RENDERBUFFER_ALPHA_SIZE',
      'GL_RENDERBUFFER_DEPTH_SIZE',
      'GL_RENDERBUFFER_STENCIL_SIZE',
      'GL_RENDERBUFFER_WIDTH',
      'GL_RENDERBUFFER_HEIGHT',
      'GL_RENDERBUFFER_INTERNAL_FORMAT',
    ],
  },
  'ShaderParameter': {
    'type': 'GLenum',
    'valid': [
      'GL_SHADER_TYPE',
      'GL_DELETE_STATUS',
      'GL_COMPILE_STATUS',
      'GL_INFO_LOG_LENGTH',
      'GL_SHADER_SOURCE_LENGTH',
      'GL_TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE',
    ],
  },
  'ShaderPrecision': {
    'type': 'GLenum',
    'valid': [
      'GL_LOW_FLOAT',
      'GL_MEDIUM_FLOAT',
      'GL_HIGH_FLOAT',
      'GL_LOW_INT',
      'GL_MEDIUM_INT',
      'GL_HIGH_INT',
    ],
  },
  'StringType': {
    'type': 'GLenum',
    'valid': [
      'GL_VENDOR',
      'GL_RENDERER',
      'GL_VERSION',
      'GL_SHADING_LANGUAGE_VERSION',
      'GL_EXTENSIONS',
    ],
  },
  'TextureParameter': {
    'type': 'GLenum',
    'valid': [
      'GL_TEXTURE_MAG_FILTER',
      'GL_TEXTURE_MIN_FILTER',
      'GL_TEXTURE_WRAP_S',
      'GL_TEXTURE_WRAP_T',
      'GL_TEXTURE_WRAP_R_OES',
      'GL_TEXTURE_MAX_ANISOTROPY_EXT',
    ],
    'invalid': [
      'GL_GENERATE_MIPMAP',
    ],
  },
  'TextureWrapMode': {
    'type': 'GLenum',
    'valid': [
      'GL_CLAMP_TO_EDGE',
      'GL_MIRRORED_REPEAT',
      'GL_REPEAT',
    ],
  },
  'TextureMinFilterMode': {
    'type': 'GLenum',
    'valid': [
      'GL_NEAREST',
      'GL_LINEAR',
      'GL_NEAREST_MIPMAP_NEAREST',
      'GL_LINEAR_MIPMAP_NEAREST',
      'GL_NEAREST_MIPMAP_LINEAR',
      'GL_LINEAR_MIPMAP_LINEAR',
    ],
  },
  'TextureMagFilterMode': {
    'type': 'GLenum',
    'valid': [
      'GL_NEAREST',
      'GL_LINEAR',
    ],
  },
  'TextureUsage': {
    'type': 'GLenum',
    'valid': [
      'GL_NONE',
      'GL_FRAMEBUFFER_ATTACHMENT_ANGLE',
    ],
  },
  'VertexAttribute': {
    'type': 'GLenum',
    'valid': [
      # some enum that the decoder actually passes through to GL needs
      # to be the first listed here since it's used in unit tests.
      'GL_VERTEX_ATTRIB_ARRAY_NORMALIZED',
      'GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING',
      'GL_VERTEX_ATTRIB_ARRAY_ENABLED',
      'GL_VERTEX_ATTRIB_ARRAY_SIZE',
      'GL_VERTEX_ATTRIB_ARRAY_STRIDE',
      'GL_VERTEX_ATTRIB_ARRAY_TYPE',
      'GL_CURRENT_VERTEX_ATTRIB',
    ],
  },
  'VertexPointer': {
    'type': 'GLenum',
    'valid': [
      'GL_VERTEX_ATTRIB_ARRAY_POINTER',
    ],
  },
  'HintTarget': {
    'type': 'GLenum',
    'valid': [
      'GL_GENERATE_MIPMAP_HINT',
    ],
    'invalid': [
      'GL_PERSPECTIVE_CORRECTION_HINT',
    ],
  },
  'HintMode': {
    'type': 'GLenum',
    'valid': [
      'GL_FASTEST',
      'GL_NICEST',
      'GL_DONT_CARE',
    ],
  },
  'PixelStore': {
    'type': 'GLenum',
    'valid': [
      'GL_PACK_ALIGNMENT',
      'GL_UNPACK_ALIGNMENT',
      'GL_UNPACK_ROW_LENGTH',
      'GL_UNPACK_SKIP_ROWS',
      'GL_UNPACK_SKIP_PIXELS'
    ],
    'invalid': [
      'GL_PACK_SWAP_BYTES',
      'GL_UNPACK_SWAP_BYTES',
    ],
  },
  'PixelStoreAlignment': {
    'type': 'GLint',
    'valid': [
      '1',
      '2',
      '4',
      '8',
    ],
    'invalid': [
      '3',
      '9',
    ],
  },
  'ReadPixelFormat': {
    'type': 'GLenum',
    'valid': [
      'GL_ALPHA',
      'GL_RGB',
      'GL_RGBA',
    ],
  },
  'PixelType': {
    'type': 'GLenum',
    'valid': [
      'GL_UNSIGNED_BYTE',
      'GL_UNSIGNED_SHORT_5_6_5',
      'GL_UNSIGNED_SHORT_4_4_4_4',
      'GL_UNSIGNED_SHORT_5_5_5_1',
    ],
    'invalid': [
      'GL_SHORT',
      'GL_INT',
    ],
  },
  'ReadPixelType': {
    'type': 'GLenum',
    'valid': [
      'GL_UNSIGNED_BYTE',
      'GL_UNSIGNED_SHORT_5_6_5',
      'GL_UNSIGNED_SHORT_4_4_4_4',
      'GL_UNSIGNED_SHORT_5_5_5_1',
    ],
    'invalid': [
      'GL_SHORT',
      'GL_INT',
    ],
  },
  'RenderBufferFormat': {
    'type': 'GLenum',
    'valid': [
      'GL_RGBA4',
      'GL_RGB565',
      'GL_RGB5_A1',
      'GL_DEPTH_COMPONENT16',
      'GL_STENCIL_INDEX8',
    ],
  },
  'StencilOp': {
    'type': 'GLenum',
    'valid': [
      'GL_KEEP',
      'GL_ZERO',
      'GL_REPLACE',
      'GL_INCR',
      'GL_INCR_WRAP',
      'GL_DECR',
      'GL_DECR_WRAP',
      'GL_INVERT',
    ],
  },
  'TextureFormat': {
    'type': 'GLenum',
    'valid': [
      'GL_ALPHA',
      'GL_LUMINANCE',
      'GL_LUMINANCE_ALPHA',
      'GL_RGB',
      'GL_RGBA',
      'GL_BGRA_EXT',
    ],
    'invalid': [
      'GL_BGRA',
      'GL_BGR',
    ],
  },
  'TextureInternalFormat': {
    'type': 'GLenum',
    'valid': [
      'GL_ALPHA',
      'GL_LUMINANCE',
      'GL_LUMINANCE_ALPHA',
      'GL_RGB',
      'GL_RGBA',
    ],
    'invalid': [
      'GL_BGRA',
      'GL_BGR',
    ],
  },
  'TextureInternalFormatStorage': {
    'type': 'GLenum',
    'valid': [
      'GL_RGB565',
      'GL_RGBA4',
      'GL_RGB5_A1',
      'GL_ALPHA8_EXT',
      'GL_LUMINANCE8_EXT',
      'GL_LUMINANCE8_ALPHA8_EXT',
      'GL_RGB8_OES',
      'GL_RGBA8_OES',
    ],
  },
  'VertexAttribType': {
    'type': 'GLenum',
    'valid': [
      'GL_BYTE',
      'GL_UNSIGNED_BYTE',
      'GL_SHORT',
      'GL_UNSIGNED_SHORT',
    #  'GL_FIXED',  // This is not available on Desktop GL.
      'GL_FLOAT',
    ],
    'invalid': [
      'GL_DOUBLE',
    ],
  },
  'TextureBorder': {
    'type': 'GLint',
    'valid': [
      '0',
    ],
    'invalid': [
      '1',
    ],
  },
  'VertexAttribSize': {
    'type': 'GLint',
    'valid': [
      '1',
      '2',
      '3',
      '4',
    ],
    'invalid': [
      '0',
      '5',
    ],
  },
  'ZeroOnly': {
    'type': 'GLint',
    'valid': [
      '0',
    ],
    'invalid': [
      '1',
    ],
  },
  'FalseOnly': {
    'type': 'GLboolean',
    'valid': [
      'false',
    ],
    'invalid': [
      'true',
    ],
  },
}

class Generator(object):
    __metaclass__ = ABCMeta

    def __init__(self, verbose, function_info, kind):
        self.functions_information = {}
        self.original_functions = []
        self.functions = []
        self.verbose = verbose
        self.errors = 0
        self._function_info = {}
        self._empty_type_handler = TypeHandler()
        self._empty_function_info = FunctionInfo({}, self._empty_type_handler)
        self.pepper_interfaces = []
        self.interface_info = {}
        self.command_custom_text = None
        self.command_custom_header_text = None
        self.server_text = None
        self.kind = kind

        with open(function_info) as data_file:    
            self.functions_information = json.load(data_file)

        for func_name in self.functions_information:
          info = self.functions_information[func_name]
          type = ''
          if 'type' in info:
            type = info['type']
          self._function_info[func_name] = FunctionInfo(info, self._empty_type_handler)

    def AddFunction(self, func):
        """Adds a function."""
        self.functions.append(func)

    def GetTypeHandler(self, name):
        """Gets a type info for the given type."""
        if name in self._type_handlers:
            return self._type_handlers[name]
        return self._empty_type_handler

    def GetFunctionInfo(self, name):
        """Gets a type info for the given function name."""
        if name in self._function_info:
            return self._function_info[name]
        return self._empty_function_info

    def Log(self, msg):
        """Prints something if verbose is true."""
        if self.verbose:
            print msg

    def Error(self, msg):
        """Prints an error."""
        print "Error: %s" % msg
        self.errors += 1

    def WriteNamespaceOpen(self, file):
        """Writes the code for the namespace."""
        file.Write("namespace gpu {\n")
        file.Write("\n")

    def WriteNamespaceClose(self, file):
        """Writes the code to close the namespace."""
        file.Write("}  // namespace gpu\n")
        file.Write("\n")

    def ParseArgs(self, arg_string):
        """Parses a function arg string."""
        args = []
        num_pointer_args = 0
        parts = arg_string.split(',')
        is_gl_enum = False
        for arg_string in parts:
            if arg_string.startswith('GLenum '):
                is_gl_enum = True
            arg = CreateArg(arg_string)
            if arg:
                args.append(arg)
                if arg.IsPointer():
                    num_pointer_args += 1
        return (args, num_pointer_args, is_gl_enum)

    def ParseAPIFile(self, filename, regular_expression):
        """Parses the cmd_buffer_functions.txt file and extracts the functions"""

        f = open(filename, "r")
        functions = f.read()
        f.close()

        for line in functions.splitlines():
            match = regular_expression.match(line)
            if match:
                func_name = match.group(2)
                func_info = self.GetFunctionInfo(func_name)
                if func_info.type != 'Noop':
                    return_type = match.group(1).strip()
                    arg_string = match.group(3)
                    (args, num_pointer_args, is_gl_enum) = self.ParseArgs(arg_string)
                    # comment in to find out which functions use bare enums.
                    # if is_gl_enum:
                    #   self.Log("%s uses bare GLenum" % func_name)
                    args_for_cmds = args
                    if hasattr(func_info, 'cmd_args'):
                        (args_for_cmds, num_pointer_args, is_gl_enum) = (
                            self.ParseArgs(getattr(func_info, 'cmd_args')))
                    cmd_args = []
                    for arg in args_for_cmds:
                        arg.AddCmdArgs(cmd_args)
                    init_args = []
                    for arg in args_for_cmds:
                        arg.AddInitArgs(init_args)
                    return_arg = CreateArg(return_type + " result")
                    if return_arg:
                        init_args.append(return_arg)
                    f = Function(func_name, func_name, func_info, return_type, args,
                                 args_for_cmds, cmd_args, init_args, num_pointer_args)
                    self.original_functions.append(f)
                    gen_cmd = f.GetInfo('gen_cmd')
                    if gen_cmd == True or gen_cmd == None:
                        self.AddFunction(f)

        self.Log("Auto Generated Functions    : %d" %
             len([f for f in self.functions if f.can_auto_generate or
                  (not f.IsType('') and not f.IsType('Custom') and
                   not f.IsType('Todo'))]))

        funcs = [f for f in self.functions if not f.can_auto_generate and
                 (f.IsType('') or f.IsType('Custom') or f.IsType('Todo'))]
        self.Log("Non Auto Generated Functions: %d" % len(funcs))

        for f in funcs:
            self.Log("  %-10s %-20s gl%s" % (f.info.type, f.return_type, f.name))

    @abstractmethod
    def ParseAPIFiles(self):
        pass

    def HasCustomClientEntryPoint(self, func):
        if func.IsType('Manual'):
            return True

        # Manual init functions always allow generating the client-side entry point.
        if self.HasCustomInit(func):
            return False
        if func.IsSynchronous():
            return False
        if not func.KnowHowToPassArguments():
            return True
        return not func.KnowHowToAssignDefaultReturnValue()

    def GetClientEntryPointSignature(self, func):
        if self.HasCustomClientEntryPoint(func):
            return

        args = func.MakeTypedOriginalArgString("")
        if not args:
            args = "void"

        name = func.name
        if func.ShouldHideEntryPoint():
            name = "__hidden_gpuproxy_" + name

        return "%s %s (%s)" % (func.return_type, name, args)

    def WriteClientEntryPoints(self, filename):
        file = CWriter(filename)
        file.Write('#include "caching_client.h"\n')
        self.WriteHeaders(file)

        for func in self.functions:
            if func.name.find("eglGetProcAddress") != -1:
                continue

            header = self.GetClientEntryPointSignature(func)
            if not header:
                continue

            file.Write(header + "\n")
            file.Write("{\n")
            file.Write("    INSTRUMENT();\n")

            file.Write("    if (should_use_base_dispatch ()) {\n")

            file.Write("        ")
            if func.HasReturnValue():
                file.Write("return ")
            file.Write("dispatch_table_get_base ()->%s (NULL" % func.name)
            file.Write(func.MakeOriginalArgString("", add_separator=True))
            file.Write(");\n")
            if not func.HasReturnValue():
                file.Write("        return;\n")

            file.Write("    }\n")

            file.Write("    client_t *client = client_get_thread_local ();\n");

            file.Write("    ")
            if func.HasReturnValue():
                file.Write("return ")
            file.Write("client->dispatch.%s (client" % func.name)
            file.Write(func.MakeOriginalArgString("", separator=", ", add_separator=True))
            file.Write(");\n")

            file.Write("}\n\n")

        file.Close()

    @abstractmethod
    def HandleFunctionsGeneratingErrors(self, func, filename):
        pass

    def WriteBaseClient(self, filename):
        """Writes the base server implementation, which just places the commands on the command buffer and runs them."""
        file = CWriter(filename)
        self.WriteHeaders(file)

        for func in self.functions:
            file.Write("static %s\n" % func.return_type)
            file.Write("client_dispatch_%s (void* object" % func.name.lower())
            file.Write(func.MakeTypedOriginalArgString("", separator=",\n    ", add_separator=True))
            file.Write(")\n")

            file.Write("{\n")

            self.HandleFunctionsGeneratingErrors(func, file)

            file.Write("    INSTRUMENT();\n");
            file.Write("    command_t *command = client_get_space_for_command (COMMAND_%s);\n" % func.name.upper())

            header = "    command_%s_init (" % func.name.lower()
            indent = " " * len(header)
            file.Write(header + "command")
            args = func.MakeOriginalArgString(indent, separator = ",\n", add_separator = True)
            if args:
                file.Write(args)
            file.Write(");\n\n")

            if func.IsSynchronous():
                file.Write("    client_run_command (command);\n");
            else:
                file.Write("    client_run_command_async (command);\n");

            if func.HasReturnValue():
              file.Write("\n")
              file.Write("    return ((command_%s_t *)command)->result;\n\n" % func.name.lower())

            file.Write("}\n\n")

        file.Write("void\n")
        file.Write("client_fill_dispatch_table (dispatch_table_t *dispatch)\n")
        file.Write("{\n")
        for func in self.functions:
            file.Write('    dispatch->%s = client_dispatch_%s;\n' % (func.name, func.name.lower()))
        file.Write("}\n")
        file.Close()

    def WriteCommandHeader(self, filename):
        """Writes the command format"""
        file = CWriter(filename)

        file.Write("#ifndef COMMAND_AUTOGEN_%s_H\n" %(self.kind))
        file.Write("#define COMMAND_AUTOGEN_%s_H\n\n" %(self.kind))
        file.Write("#include \"command.h\"\n\n")
        self.WriteHeaders(file)

        for func in self.functions:
            if self.HasCustomStruct(func):
                continue
            func.WriteStruct(file)

        file.Write("\n")

        for func in self.functions:
            file.Write("private ");
            func.WriteInitSignature(file)
            file.Write(";\n\n")

            if func.NeedsDestructor() or self.HasCustomDestroyArguments(func):
                file.Write("private void\n");
                file.Write("command_%s_destroy_arguments (command_%s_t *command);\n\n" % \
                  (func.name.lower(), func.name.lower()))

        file.Write("#endif /*COMMAND_AUTOGEN_H*/\n")
        file.Close()

    @abstractmethod
    def WriteHeaders(self, file):
        pass

    def CommandCustomText(self):
        if not self.command_custom_text:
            self.command_custom_text = open(os.path.join('..', 'command_custom.c')).read()
        return self.command_custom_text

    def CommandCustomHeaderText(self):
        if not self.command_custom_header_text:
            self.command_custom_header_text = open(os.path.join('..', 'command_custom.h')).read()
        return self.command_custom_header_text

    def ServerText(self):
        if not self.server_text:
            self.server_text = open(os.path.join('..', 'server', 'server.c')).read()
        return self.server_text

    def HasCustomInit(self, func):
        init_name = "command_%s_init " % func.name.lower()
        return self.CommandCustomText().find(init_name) != -1

    def HasCustomDestroyArguments(self, func):
        init_name = "command_%s_destroy_arguments " % func.name.lower()
        return self.CommandCustomText().find(init_name) != -1

    def HasCustomStruct(self, func):
        struct_declaration = "typedef struct _command_%s " % func.name.lower()
        return self.CommandCustomHeaderText().find(struct_declaration) != -1

    def HasCustomServerHandler(self, func):
        handler_name = "server_handle_%s " % func.name.lower()
        return self.ServerText().find(handler_name) != -1

    def WriteCommandInitilizationAndSizeFunction(self, filename):
        """Writes the command implementation for the client-side"""
        file = CWriter(filename)

        file.Write("#include \"command.h\"\n")
        file.Write("#include <string.h>\n\n")
        file.Write('#include "gles2_utils.h"\n')

        for func in self.functions:
            if not self.HasCustomInit(func):
                func.WriteCommandInit(file)
            if not self.HasCustomDestroyArguments(func):
                func.WriteCommandDestroy(file)

        file.Write("void\n")
        file.Write("command_initialize_sizes (size_t *sizes)\n")
        file.Write("{\n")
        for func in self.functions:
            file.Write("sizes[COMMAND_%s] = sizeof (command_%s_t);\n" % \
                        (func.name.upper(), func.name.lower()))
        file.Write("}\n")

        file.Write("\n")
        file.Close()

    def WriteDispatchTable(self, filename):
        """Writes the dispatch struct for the server-side"""
        file = CHeaderWriter(filename)

        file.Write("#include \"config.h\"\n")
        file.Write("#include \"compiler_private.h\"\n")
        self.WriteHeaders(file)

        file.Write("typedef void (*FunctionPointerType)(void);\n")
        file.Write("typedef struct _dispatch_table {\n")
        for func in self.functions:
            file.Write("    %s (*%s) (void *object" % (func.return_type, func.name))
            file.Write(func.MakeTypedOriginalArgString("", add_separator = True), split=False)
            file.Write(");\n")
        file.Write("} dispatch_table_t;")
        file.Write("\n")
        file.Close()

    @abstractmethod
    def WriteBaseServer(self, filename):
        pass

    def WritePassthroughDispatchTableImplementation(self, filename):
        """Writes the pass-through dispatch table implementation."""
        file = CWriter(filename)

        self.WriteHeaders(file)
        file.Write("\n")

        for func in self.functions:
            file.Write("static %s (*real_%s) (" % (func.return_type, func.name))
            file.Write(func.MakeTypedOriginalArgString(""), split=False)
            file.Write(");\n")

        for func in self.functions:
            file.Write("static %s\n" % func.return_type)

            func_name = "passthrough_%s (" % func.name
            indent = " " * len(func_name)
            file.Write("%svoid* object" % func_name)
            file.Write(func.MakeTypedOriginalArgString(indent, separator = ",\n", add_separator = True), split=False)
            file.Write(")\n")
            file.Write("{\n")

            #file.Write("    INSTRUMENT ();")

            file.Write("    ")
            if func.return_type != "void":
                file.Write("return ")
            file.Write("real_%s (" % func.name)
            file.Write(func.MakeOriginalArgString(""))
            file.Write(");\n")
            file.Write("}\n\n")

        file.Write("void\n")
        file.Write("dispatch_table_fill_base (dispatch_table_t *dispatch)\n")
        file.Write("{\n")
        file.Write("    FunctionPointerType *temp = NULL;\n")

        for func in self.functions:
            file.Write('    dispatch->%s = passthrough_%s;\n' % (func.name, func.name))

        file.Write("temp = (FunctionPointerType *) &real_eglGetProcAddress;\n")
        file.Write('*temp = dlsym (libegl_handle (), "eglGetProcAddress");');

        for func in self.functions:
            if (func.name == "eglGetProcAddress"):
                continue
            file.Write('    temp = (FunctionPointerType *) &real_%s;\n' % func.name)
            file.Write('    *temp = ')
            file.Write('            find_gl_symbol (lib%s_handle (),' %(func.name.startswith('egl') and 'egl' or 'gl' ))
            file.Write('            real_eglGetProcAddress, "%s");\n' % func.name)

        file.Write("}\n")
        file.Close()

    def WriteCachingClientDispatchTableImplementation(self, filename):
        caching_client_text = open(os.path.join('..', 'client', 'caching_client.c')).read()
        file = CWriter(filename)

        for func in self.functions:
            caching_func_name = "caching_client_%s " % func.name
            if caching_client_text.find(caching_func_name) == -1:
                continue
            file.Write('    client->super.dispatch.%s = %s;\n' % (func.name, caching_func_name))
        file.Close()

    def WriteCommandEnum(self, filename):
        """Writes the command format"""
        file = CWriter(filename)
        for func in self.functions:
            func.WriteEnumName(file)
        file.Write("\n")
        file.Close()

    def WriteEnumValidation(self, filename):
        """Writes the implementation for enum validation"""
        file = CWriter(filename)
        file.Write("#include \"config.h\"\n")
        file.Write("#include \"gl2ext.h\"\n")
        file.Write("#include <GLES2/gl2.h>\n\n")

        for (key, value) in _ENUM_LISTS.iteritems():
            file.Write("private bool\n")
            file.Write("is_valid_%s (%s value)\n" % (key, value['type']))
            file.Write("{\n")
            file.Write("    return %s;\n" %
                " ||\n        ".join([("value == %s" % type) for type in value['valid']]))
            file.Write("}\n\n")
        file.Close()

class GLGenerator(Generator):
  """A class to generate GL command buffers."""

  _gles2_function_re = re.compile(r'GL_APICALL(.*?)GL_APIENTRY (.*?) \((.*?)\);')
  _egl_function_re = re.compile(r'EGLAPI(.*?)EGLAPIENTRY (.*?) \((.*?)\);')

  def __init__(self, verbose):
    Generator.__init__(self, verbose, "function_info.json", "GLES")

  def WriteNamespaceOpen(self, file):
    Generator.WriteNamespaceOpen(self, file)
    file.Write("namespace gles2 {\n")
    file.Write("\n")

  def WriteNamespaceClose(self, file):
    file.Write("}  // namespace gles2\n")
    file.Write("\n")
    Generator.WriteNamespaceClose(self, file)

  def ParseAPIFiles(self):
    self.ParseAPIFile("gles2_functions.txt", self._gles2_function_re)
    self.ParseAPIFile("egl_functions.txt", self._egl_function_re)

  def WriteBaseServer(self, filename):
    file = CWriter(filename)

    for func in self.functions:
        if self.HasCustomServerHandler(func):
            continue

        file.Write("static void\n")
        file.Write("server_handle_%s (server_t *server, command_t *abstract_command)\n" % func.name.lower())
        file.Write("{\n")
        file.Write("    INSTRUMENT ();\n")

        need_destructor_call = func.NeedsDestructor() or self.HasCustomDestroyArguments(func)
        if need_destructor_call or len(func.GetOriginalArgs()) > 0 or func.HasReturnValue():
          file.Write("    command_%s_t *command =\n" % func.name.lower())
          file.Write("            (command_%s_t *)abstract_command;\n" % func.name.lower())

        mapped_names = func.GetMappedNameAttributes()
        for mapped_name in mapped_names:
          file.Write("    if (command->%s) {\n" % mapped_name)
          file.Write("        mutex_lock (name_mapping_mutex);\n")
          file.Write("        GLuint *%s = hash_lookup (name_mapping_%s, command->%s);\n" % (mapped_name, func.GetMappedNameType(),  mapped_name))
          file.Write("        mutex_unlock (name_mapping_mutex);\n")
          file.Write("        if (!%s) {\n" % mapped_name)
          if (func.NeedsCreateMappedName(mapped_name)):
            file.Write("        GLuint *data = (GLuint *) malloc (1 * sizeof (GLuint));\n")
            file.Write("        *data = command->%s;\n" %mapped_name)
            file.Write("        hash_insert (name_mapping_%s, *data, data);\n" %func.GetMappedNameType())
            file.Write("        %s = data;\n" %mapped_name)
          else:
            file.Write("            return;\n")
          file.Write("        }\n");
          file.Write("        command->%s = *%s;\n" % (mapped_name, mapped_name))
          file.Write("    }\n")

        file.Write("    ")
        if func.HasReturnValue():
          file.Write("command->result = ")
        file.Write("server->dispatch.%s (server" % func.name)

        for arg in func.GetOriginalArgs():
          file.Write(", ")
          if arg.IsDoublePointer() and arg.type.find("const") != -1:
            file.Write("(%s) " % arg.type)
          file.Write("command->%s" % arg.name)
        file.Write(");\n")

        if need_destructor_call:
          file.Write("    command_%s_destroy_arguments (command);\n" % func.name.lower())
        file.Write("}\n\n")

    file.Write("static void\n")
    file.Write("server_fill_command_handler_table (server_t* server)\n")
    file.Write("{\n")
    for func in self.functions:
      file.Write("    server->handler_table[COMMAND_%s] = \n" % func.name.upper())
      file.Write("        server_handle_%s;\n" % func.name.lower())
    file.Write("}\n\n")

    file.Close()

  def WriteHeaders(self, file):
      file.Write("#include <EGL/egl.h>\n")
      file.Write("#include <EGL/eglext.h>\n")
      file.Write("#include <GLES2/gl2.h>\n")
      file.Write("#include <GLES2/gl2ext.h>\n\n")

  def HandleFunctionsGeneratingErrors(self, func, file):
      if func.name in FUNCTIONS_GENERATING_ERRORS:
          file.Write("    egl_state_t *state = client_get_current_state (CLIENT (object));\n");
          file.Write("    if (state)\n");
          file.Write("        state->need_get_error = true;\n\n");
 
