import re

_DEFAULT_RETURN_VALUES = {
    '__eglMustCastToProperFunctionPointerType': 'NULL',
    'const char*': 'NULL',
    'const GLubyte*': 'NULL',
    'void*': 'NULL',
    'GLuint': '0',
    'GLint': '0',
    'GLenum': 'GL_INVALID_ENUM',
    'GLboolean': 'GL_FALSE',
    'EGLDisplay': 'EGL_NO_DISPLAY',
    'EGLBoolean': 'EGL_FALSE',
    'EGLSurface': 'EGL_NO_SURFACE',
    'EGLenum': 'EGL_NONE',
    'EGLContext': 'EGL_NO_CONTEXT',
    'EGLImageKHR': 'EGL_NO_IMAGE_KHR',
    'EGLSyncKHR': 'EGL_NO_SYNC_KHR',
    'EGLSyncNV': 'EGL_NO_SYNC_NV'
}

class TypeHandler(object):
  """This class emits code for a particular type of function."""

  _remove_expected_call_re = re.compile(r'  EXPECT_CALL.*?;\n', re.S)

  def __init__(self):
    pass

  def InitFunction(self, func):
    """Add or adjust anything type specific for this function."""
    if func.GetInfo('needs_size'):
      func.AddCmdArg(DataSizeArgument('data_size'))

  def AddImmediateFunction(self, generator, func):
    """Adds an immediate version of a function."""
    # Generate an immediate command if there is only 1 pointer arg.
    immediate = func.GetInfo('immediate')  # can be True, False or None
    if immediate == True or immediate == None:
      if func.num_pointer_args == 1 or immediate:
        generator.AddFunction(ImmediateFunction(func))

  def WriteStruct(self, func, file):
    """Writes a structure that matches the arguments to a function."""
    comment = func.GetInfo('cmd_comment')
    if not comment == None:
      file.Write(comment)
    file.Write("typedef struct _command_%s {\n" % func.name.lower())
    file.Write("  command_t header;\n")
    args = func.GetOriginalArgs()
    for arg in args:
      type = arg.type.replace("const", "")
      file.Write("  %s %s;\n" % (type, arg.name))
    if func.return_type != "void":
      file.Write("  %s result;\n" % (func.return_type))
    file.Write("} command_%s_t;\n" % (func.name.lower()))
    file.Write("\n")

  def WriteStructFunctionPointer(self, func, file):
    file.Write(func.return_type)
    file.Write(" (*%s) (" % func.name)
    file.Write(func.MakeTypedOriginalArgString(""), split=False)
    file.Write(");")

  def WriteCommandInitArgumentCopy(self, func, arg, file):
    # FIXME: Handle constness more gracefully.
    if func.IsSynchronous():
      type = arg.type.replace("const", "")
      file.Write("    command->%s = (%s) %s;\n" % (arg.name, type, arg.name))
      return

    if arg.type.find("char*") != -1:
      file.Write("    command->%s = strdup (%s);\n" % (arg.name, arg.name))
      return

    # We only need to copy arguments if the function is asynchronous.
    if  (arg.name in func.info.argument_has_size or \
         arg.name in func.info.argument_element_size or \
         arg.name in func.info.argument_size_from_function):

      components = []
      if arg.name in func.info.argument_has_size:
          components.append(func.info.argument_has_size[arg.name])
      if arg.name in func.info.argument_element_size:
          components.append("%i" % func.info.argument_element_size[arg.name])
      if arg.name in func.info.argument_size_from_function:
          components.append("%s (%s)" % (func.info.argument_size_from_function[arg.name], arg.name))
      if arg.type.find("void*") == -1:
          components.append("sizeof (%s)" % arg.type)
      arg_size = " * ".join(components)

      file.Write("    if (%s) {\n" % (arg.name))
      file.Write("        command->%s = malloc (%s);\n" % (arg.name, arg_size))
      file.Write("        memcpy (command->%s, %s, %s);\n" % (arg.name, arg.name, arg_size))
      file.Write("    } else\n")
      file.Write("        command->%s = 0;\n" % (arg.name))
      return

    # FIXME: Handle constness more gracefully.
    type = arg.type.replace("const", "")
    file.Write("    command->%s = (%s) %s;\n" % (arg.name, type, arg.name))

  def WriteCommandInit(self, func, file):
    file.Write ("inline ")
    self.WriteInitSignature(func, file)
    file.Write("\n{\n")

    args = func.GetOriginalArgs()
    if len(args):
        subclass_command_type = "command_%s_t" % func.name.lower()
        file.Write("    %s *command = (%s *) abstract_command;\n" % (subclass_command_type, subclass_command_type))

        for arg in args:
            self.WriteCommandInitArgumentCopy(func, arg, file)

        if func.return_type != "void":
          file.Write("    command->result = 0;\n")

    file.Write("}\n\n")

  def WriteCommandDestroy(self, func, file):
    file.Write("void\n");
    file.Write("command_%s_destroy_arguments (command_%s_t *command)\n" % \
        (func.name.lower(), func.name.lower()))
    file.Write("\n{\n")

    # The only thing we do for the moment is free arguments.
    arguments_to_free = [arg for arg in func.GetOriginalArgs() if arg.IsPointer()]
    for arg in arguments_to_free:
      file.Write("    if (command->%s)\n" % arg.name)
      file.Write("        free (command->%s);\n" % arg.name)
    file.Write("}\n")

  def WriteInitSignature(self, func, file):
    """Writes the declaration of the init function for a given function."""
    file.Write("void\n") 

    call = "command_%s_init (" % func.name.lower()
    file.Write(call)
    file.Write("command_t *abstract_command")

    file.Write(func.MakeTypedOriginalArgString("", separator=",\n" + (" " * len(call)),
                                               add_separator = True))
    file.Write(")")

  def WriteEnumName(self, func, file):
    """Writes an enum name that matches the name of a function."""
    file.Write("COMMAND_" + func.name.upper() + ",\n")

  def WriteCmdSizeTest(self, func, file):
    """Writes the size test for a command."""
    file.Write("  EXPECT_EQ(sizeof(cmd), cmd.header.size * 4u);\n")

  def WriteClientGLReturnLog(self, func, file):
    """Writes the return value logging code."""
    if func.return_type != "void":
      file.Write('  GPU_CLIENT_LOG("return:" << result)\n')

  def WriteImmediateCmdComputeSize(self, func, file):
    """Writes the size computation code for the immediate version of a cmd."""
    file.Write("  static uint32_t ComputeSize(uint32_t size_in_bytes) {\n")
    file.Write("    return static_cast<uint32_t>(\n")
    file.Write("        sizeof(ValueType) +  // NOLINT\n")
    file.Write("        RoundSizeToMultipleOfEntries(size_in_bytes));\n")
    file.Write("  }\n")
    file.Write("\n")

  def WriteImmediateCmdSetHeader(self, func, file):
    """Writes the SetHeader function for the immediate version of a cmd."""
    file.Write("  void SetHeader(uint32_t size_in_bytes) {\n")
    file.Write("    header.SetCmdByTotalSize<ValueType>(size_in_bytes);\n")
    file.Write("  }\n")
    file.Write("\n")

  def WriteImmediateCmdSet(self, func, file):
    """Writes the Set function for the immediate version of a command."""
    raise NotImplementedError(func.name)


class FunctionInfo(object):
  """Holds info about a function."""

  def __init__(self, info, type_handler):
    for key in info:
      setattr(self, key, info[key])
    self.type_handler = type_handler
    if not 'type' in info:
      self.type = ''
    if not 'default_return' in info:
      self.default_return = ''
    if not 'argument_has_size' in info:
      self.argument_has_size = {}
    if not 'out_arguments' in info:
      self.out_arguments = []
    if not 'argument_element_size' in info:
      self.argument_element_size = {}
    if not 'argument_size_from_function' in info:
      self.argument_size_from_function = {}
    if not 'mapped_names' in info:
      self.mapped_names = {}

class Argument(object):
  """A class that represents a function argument."""

  cmd_type_map_ = {
    'GLenum': 'uint32_t',
    'GLuint': 'uint32_t',
    'GLboolean' : 'uint32_t',
    'GLbitfield' : 'uint32_t',
    'GLint': 'int32_t',
    'GLchar': 'int8_t',
    'GLintptr': 'int32_t',
    'GLsizei': 'int32_t',
    'GLsizeiptr': 'int32_t',
    'GLfloat': 'float',
    'GLclampf': 'float',
    'GLvoid': 'void',
    'GLeglImageOES' : 'void*'
  }
  need_validation_ = ['GLsizei*', 'GLboolean*', 'GLenum*', 'GLint*']

  def __init__(self, name, type):
    self.name = name
    self.optional = type.endswith("Optional*")
    if self.optional:
      type = type[:-9] + "*"
    self.type = type

    if type.endswith("*"):
      type = type[:-1]
      needs_asterisk = True
    else:
      needs_asterisk = False

    needs_const = False
    if type.startswith("const"):
      type = type[6:]
      needs_const = True

    if type in self.cmd_type_map_:
      self.cmd_type = self.cmd_type_map_[type]
      needs_const = False
    else:
      self.cmd_type = type

    if needs_asterisk:
      self.cmd_type += "*"

    if needs_const:
      self.cmd_type = "const " + self.cmd_type

  def IsPointer(self):
    """Returns true if argument is a pointer."""
    return self.type.find("*") != -1

  def IsDoublePointer(self):
    """Returns true if argument is a double-pointer."""
    return self.type.find("**") != -1

  def IsString(self):
    """Returns true if argument is a double-pointer."""
    return self.type == "const char*" or self.type == "char*" or \
           self.type == "const GLchar*" or self.type == "GLchar*"

  def AddCmdArgs(self, args):
    """Adds command arguments for this argument to the given list."""
    return args.append(self)

  def AddInitArgs(self, args):
    """Adds init arguments for this argument to the given list."""
    return args.append(self)

  def GetNumInvalidValues(self, func):
    """returns the number of invalid values to be tested."""
    return 0

  def GetInvalidArg(self, offset, index):
    """returns an invalid value and expected parse result by index."""
    return ("---ERROR0---", "---ERROR2---", None)

  def WriteGetAddress(self, file):
    """Writes the code to get the address this argument refers to."""
    pass

  def GetImmediateVersion(self):
    """Gets the immediate version of this argument."""
    return self


class BoolArgument(Argument):
  """class for GLboolean"""

  def __init__(self, name, type):
    Argument.__init__(self, name, 'GLboolean')

class UniformLocationArgument(Argument):
  """class for uniform locations."""

  def __init__(self, name):
    Argument.__init__(self, name, "GLint")

class DataSizeArgument(Argument):
  """class for data_size which Bucket commands do not need."""

  def __init__(self, name):
    Argument.__init__(self, name, "uint32_t")


class SizeArgument(Argument):
  """class for GLsizei and GLsizeiptr."""

  def __init__(self, name, type):
    Argument.__init__(self, name, type)

  def GetNumInvalidValues(self, func):
    """overridden from Argument."""
    if func.is_immediate:
      return 0
    return 1

  def GetInvalidArg(self, offset, index):
    """overridden from Argument."""
    return ("-1", "kNoError", "GL_INVALID_VALUE")

class SizeNotNegativeArgument(SizeArgument):
  """class for GLsizeiNotNegative. It's NEVER allowed to be negative"""

  def __init__(self, name, type, gl_type):
    SizeArgument.__init__(self, name, gl_type)

  def GetInvalidArg(self, offset, index):
    """overridden from SizeArgument."""
    return ("-1", "kOutOfBounds", "GL_NO_ERROR")


class EnumBaseArgument(Argument):
  """Base class for EnumArgument, IntArgument and ValidatedBoolArgument"""

  def __init__(self, name, gl_type, type, gl_error):
    Argument.__init__(self, name, gl_type)

    self.local_type = type
    self.gl_error = gl_error
    name = type[len(gl_type):]
    self.type_name = name

class EnumArgument(EnumBaseArgument):
  """A class that represents a GLenum argument"""

  def __init__(self, name, type):
    EnumBaseArgument.__init__(self, name, "GLenum", type, "GL_INVALID_ENUM")

class IntArgument(EnumBaseArgument):
  """A class for a GLint argument that can only except specific values.

  For example glTexImage2D takes a GLint for its internalformat
  argument instead of a GLenum.
  """

  def __init__(self, name, type):
    EnumBaseArgument.__init__(self, name, "GLint", type, "GL_INVALID_VALUE")


class ValidatedBoolArgument(EnumBaseArgument):
  """A class for a GLboolean argument that can only except specific values.

  For example glUniformMatrix takes a GLboolean for it's transpose but it
  must be false.
  """

  def __init__(self, name, type):
    EnumBaseArgument.__init__(self, name, "GLboolean", type, "GL_INVALID_VALUE")

class ImmediatePointerArgument(Argument):
  """A class that represents an immediate argument to a function.

  An immediate argument is one where the data follows the command.
  """

  def __init__(self, name, type):
    Argument.__init__(self, name, type)

  def AddCmdArgs(self, args):
    """Overridden from Argument."""
    pass

  def GetImmediateVersion(self):
    """Overridden from Argument."""
    return None


class PointerArgument(Argument):
  """A class that represents a pointer argument to a function."""

  def __init__(self, name, type):
    Argument.__init__(self, name, type)

  def IsPointer(self):
    """Returns true if argument is a pointer."""
    return True

  def AddCmdArgs(self, args):
    """Overridden from Argument."""
    args.append(Argument(self.name, self.type))

  def WriteGetCode(self, file):
    """Overridden from Argument."""
    file.Write(
        "  %s %s = GetSharedMemoryAs<%s>(\n" %
        (self.type, self.name, self.type))
    file.Write(
        "      c.%s_shm_id, c.%s_shm_offset, data_size);\n" %
        (self.name, self.name))

  def WriteGetAddress(self, file):
    """Overridden from Argument."""
    file.Write(
        "  %s %s = GetSharedMemoryAs<%s>(\n" %
        (self.type, self.name, self.type))
    file.Write(
        "      %s_shm_id, %s_shm_offset, %s_size);\n" %
        (self.name, self.name, self.name))

  def GetImmediateVersion(self):
    """Overridden from Argument."""
    return ImmediatePointerArgument(self.name, self.type)


class NonImmediatePointerArgument(PointerArgument):
  """A pointer argument that stays a pointer even in an immediate cmd."""

  def __init__(self, name, type):
    PointerArgument.__init__(self, name, type)

  def IsPointer(self):
    """Returns true if argument is a pointer."""
    return False

  def GetImmediateVersion(self):
    """Overridden from Argument."""
    return self


class ResourceIdArgument(Argument):
  """A class that represents a resource id argument to a function."""

  def __init__(self, name, type):
    match = re.match("(GLid\w+)", type)
    self.resource_type = match.group(1)[4:]
    type = type.replace(match.group(1), "GLuint")
    Argument.__init__(self, name, type)

  def WriteGetCode(self, file):
    """Overridden from Argument."""
    file.Write("  %s %s = c.%s;\n" % (self.type, self.name, self.name))

class ResourceIdBindArgument(Argument):
  """Represents a resource id argument to a bind function."""

  def __init__(self, name, type):
    match = re.match("(GLidBind\w+)", type)
    self.resource_type = match.group(1)[8:]
    type = type.replace(match.group(1), "GLuint")
    Argument.__init__(self, name, type)

  def WriteGetCode(self, file):
    """Overridden from Argument."""
    code = """  %(type)s %(name)s = c.%(name)s;
"""
    file.Write(code % {'type': self.type, 'name': self.name})

class ResourceIdZeroArgument(Argument):
  """Represents a resource id argument to a function that can be zero."""

  def __init__(self, name, type):
    match = re.match("(GLidZero\w+)", type)
    self.resource_type = match.group(1)[8:]
    type = type.replace(match.group(1), "GLuint")
    Argument.__init__(self, name, type)

  def WriteGetCode(self, file):
    """Overridden from Argument."""
    file.Write("  %s %s = c.%s;\n" % (self.type, self.name, self.name))

  def GetNumInvalidValues(self, func):
    """returns the number of invalid values to be tested."""
    return 1

  def GetInvalidArg(self, offset, index):
    """returns an invalid value by index."""
    return ("kInvalidClientId", "kNoError", "GL_INVALID_VALUE")


class Function(object):
  """A class that represents a function."""

  def __init__(self, original_name, name, info, return_type, original_args,
               args_for_cmds, cmd_args, init_args, num_pointer_args):
    self.name = name
    self.original_name = original_name
    self.info = info
    self.type_handler = info.type_handler
    self.return_type = return_type
    self.original_args = original_args
    self.num_pointer_args = num_pointer_args
    self.can_auto_generate = num_pointer_args == 0 and return_type == "void"
    self.cmd_args = cmd_args
    self.init_args = init_args
    self.InitFunction()
    self.args_for_cmds = args_for_cmds
    self.is_immediate = False

  def IsType(self, type_name):
    """Returns true if function is a certain type."""
    return self.info.type == type_name

  def InitFunction(self):
    """Calls the init function for the type handler."""
    self.type_handler.InitFunction(self)

  def GetInfo(self, name):
    """Returns a value from the function info for this function."""
    if hasattr(self.info, name):
      return getattr(self.info, name)
    return None


  def AddInfo(self, name, value):
    """Adds an info."""
    setattr(self.info, name, value)

  def IsCoreGLFunction(self):
    return (not self.GetInfo('extension') and
            not self.GetInfo('pepper_interface'))

  def IsSynchronous(self):
    if self.IsType('Asynchronous'):
        return False
    else:
        return self.IsType('Synchronous') \
            or self.HasReturnValue() \
            or len(self.info.out_arguments) > 0

  def HasReturnValue(self):
    return self.return_type != 'void'

  def IsOutArgument(self, arg):
    """Returns true if given argument is an out argument for this function"""
    return arg.name in self.info.out_arguments

  def GetGLFunctionName(self):
    """Gets the function to call to execute GL for this command."""
    if self.GetInfo('decoder_func'):
      return self.GetInfo('decoder_func')
    return "gl%s" % self.original_name

  def GetGLTestFunctionName(self):
    gl_func_name = self.GetInfo('gl_test_func')
    if gl_func_name == None:
      gl_func_name = self.GetGLFunctionName()
    if gl_func_name.startswith("gl"):
      gl_func_name = gl_func_name[2:]
    else:
      gl_func_name = self.original_name
    return gl_func_name

  def AddCmdArg(self, arg):
    """Adds a cmd argument to this function."""
    self.cmd_args.append(arg)

  def GetCmdArgs(self):
    """Gets the command args for this function."""
    return self.cmd_args

  def ClearCmdArgs(self):
    """Clears the command args for this function."""
    self.cmd_args = []

  def GetInitArgs(self):
    """Gets the init args for this function."""
    return self.init_args

  def GetOriginalArgs(self):
    """Gets the original arguments to this function."""
    return self.original_args

  def GetLastOriginalArg(self):
    """Gets the last original argument to this function."""
    return self.original_args[len(self.original_args) - 1]

  def __GetArgList(self, arg_string, add_separator, separator):
    """Adds a comma if arg_string is not empty and add_comma is true."""
    prefix = ""
    if add_separator and len(arg_string):
      prefix = separator
    return "%s%s" % (prefix, arg_string)

  def MakeDefaultReturnStatement(self):
    """Make a return statement with the default value."""
    if self.return_type == "void":
        return "return"
    elif self.info.default_return:
        return "return %s" % self.info.default_return
    else:
        return "return %s" % _DEFAULT_RETURN_VALUES[self.return_type]

  def MakeTypedOriginalArgString(self, prefix, add_separator = False, separator = ", "):
    """Gets a list of arguments as they arg in GL."""
    args = self.GetOriginalArgs()
    arg_string = separator.join(
        ["%s%s %s" % (prefix, arg.type, arg.name) for arg in args])
    return self.__GetArgList(arg_string, add_separator, separator)

  def MakeOriginalArgString(self, prefix, add_separator = False, separator = ", "):
    """Gets the list of arguments as they are in GL."""
    args = self.GetOriginalArgs()
    arg_string = separator.join(
        ["%s%s" % (prefix, arg.name) for arg in args])
    return self.__GetArgList(arg_string, add_separator, separator)

  def WriteStruct(self, file):
    self.type_handler.WriteStruct(self, file)

  def WriteStructFunctionPointer(self, file):
    self.type_handler.WriteStructFunctionPointer(self, file)

  def WriteCommandInit(self, file):
    self.type_handler.WriteCommandInit(self, file)

  def WriteCommandDestroy(self, file):
    self.type_handler.WriteCommandDestroy(self, file)

  def WriteInitSignature(self, file):
    self.type_handler.WriteInitSignature(self, file)

  def WriteEnumName(self, file):
    self.type_handler.WriteEnumName(self, file)

  def KnowHowToPassArguments(self):
    # Passing argument is quite easy for synchronous functions, because
    # we can just pass the pointers without doing a copy.
    if self.IsSynchronous():
        return True
    if self.IsType('Passthrough'):
        return True

    for arg in self.GetOriginalArgs():
        if self.IsOutArgument(arg):
            continue
        if arg.name in self.info.out_arguments:
            continue
        if arg.name in self.info.argument_has_size:
            continue
        if arg.name in self.info.argument_element_size:
            continue
        if arg.name in self.info.argument_size_from_function:
            continue
        if arg.IsString():
            continue
        if arg.IsPointer():
            return False
    return True

  def KnowHowToAssignDefaultReturnValue(self):
    return not self.HasReturnValue() or \
           self.return_type in _DEFAULT_RETURN_VALUES or \
           self.info.default_return

  def NeedsDestructor(self):
    return not self.IsSynchronous() and not self.IsType('Passthrough')

  def IsExtensionFunction(self):
    return self.name.endswith("OES") or \
           self.name.endswith("QCOM") or \
           self.name.endswith("APPLE") or \
           self.name.endswith("ANGLE") or \
           self.name.endswith("NV") or \
           self.name.endswith("IMG") or \
           self.name.endswith("EXT") or \
           self.name.endswith("ARB") or \
           self.name.endswith("AMD") or \
           self.name.endswith("MESA") or \
           self.name.endswith("SEC") or \
           self.name.endswith("KHR") or \
           self.name.endswith("HI")

  def ShouldHideEntryPoint(self):
    return self.IsExtensionFunction() and not self.name.startswith("egl")

  def GetMappedNameType(self):
    if ('type' in self.info.mapped_names):
        return self.info.mapped_names['type']
    return ''

  def GetMappedNameAttributes(self):
    if ('attrib_list' in self.info.mapped_names):
        return self.info.mapped_names['attrib_list']
    return []

  def NeedsCreateMappedName(self, mapped_name):
    if ('create_attrib_if_needed' in self.info.mapped_names):
        return mapped_name in self.info.mapped_names['create_attrib_if_needed']
    return False

class ImmediateFunction(Function):
  """A class that represnets an immediate function command."""

  def __init__(self, func):
    new_args = []
    for arg in func.GetOriginalArgs():
      new_arg = arg.GetImmediateVersion()
      if new_arg:
        new_args.append(new_arg)

    cmd_args = []
    new_args_for_cmds = []
    for arg in func.args_for_cmds:
      new_arg = arg.GetImmediateVersion()
      if new_arg:
        new_args_for_cmds.append(new_arg)
        new_arg.AddCmdArgs(cmd_args)

    new_init_args = []
    for arg in new_args_for_cmds:
      arg.AddInitArgs(new_init_args)

    Function.__init__(
        self,
        func.original_name,
        "%sImmediate" % func.name,
        func.info,
        func.return_type,
        new_args,
        new_args_for_cmds,
        cmd_args,
        new_init_args,
        0)
    self.is_immediate = True

def CreateArg(arg_string):
  """Creates an Argument."""
  arg_parts = arg_string.split()
  if len(arg_parts) == 1 and arg_parts[0] == 'void':
    return None
  # Is this a pointer argument?
  elif arg_string.find('*') >= 0:
    if arg_parts[0] == 'NonImmediate':
      return NonImmediatePointerArgument(
          arg_parts[-1],
          " ".join(arg_parts[1:-1]))
    else:
      return PointerArgument(
          arg_parts[-1],
          " ".join(arg_parts[0:-1]))
  # Is this a resource argument? Must come after pointer check.
  elif arg_parts[0].startswith('GLidBind'):
    return ResourceIdBindArgument(arg_parts[-1], " ".join(arg_parts[0:-1]))
  elif arg_parts[0].startswith('GLidZero'):
    return ResourceIdZeroArgument(arg_parts[-1], " ".join(arg_parts[0:-1]))
  elif arg_parts[0].startswith('GLid'):
    return ResourceIdArgument(arg_parts[-1], " ".join(arg_parts[0:-1]))
  elif arg_parts[0].startswith('GLenum') and len(arg_parts[0]) > 6:
    return EnumArgument(arg_parts[-1], " ".join(arg_parts[0:-1]))
  elif arg_parts[0].startswith('GLboolean') and len(arg_parts[0]) > 9:
    return ValidatedBoolArgument(arg_parts[-1], " ".join(arg_parts[0:-1]))
  elif arg_parts[0].startswith('GLboolean'):
    return BoolArgument(arg_parts[-1], " ".join(arg_parts[0:-1]))
  elif arg_parts[0].startswith('GLintUniformLocation'):
    return UniformLocationArgument(arg_parts[-1])
  elif (arg_parts[0].startswith('GLint') and len(arg_parts[0]) > 5 and
        not arg_parts[0].startswith('GLintptr')):
    return IntArgument(arg_parts[-1], " ".join(arg_parts[0:-1]))
  elif (arg_parts[0].startswith('GLsizeiNotNegative') or
        arg_parts[0].startswith('GLintptrNotNegative')):
    return SizeNotNegativeArgument(arg_parts[-1],
                                   " ".join(arg_parts[0:-1]),
                                   arg_parts[0][0:-11])
  elif arg_parts[0].startswith('GLsize'):
    return SizeArgument(arg_parts[-1], " ".join(arg_parts[0:-1]))
  else:
    return Argument(arg_parts[-1], " ".join(arg_parts[0:-1]))
