import json
import os
import os.path
import sys
import re

_DO_NOT_EDIT_WARNING = """// This file is auto-generated. DO NOT EDIT!"""

class CWriter(object):
  """Writes to a file formatting it for Google's style guidelines."""

  def __init__(self, filename):
    self.filename = filename
    self.file_num = 0
    self.content = []

  def SetFileNum(self, num):
    """Used to help write number files and tests."""
    self.file_num = num

  def Write(self, string, split=True):
    """Writes a string to a file spliting if it's > 80 characters."""
    lines = string.splitlines()
    num_lines = len(lines)
    for ii in range(0, num_lines):
      self.__WriteLine(lines[ii], ii < (num_lines - 1) or string[-1] == '\n', split)

  def __FindSplit(self, string):
    """Finds a place to split a string."""
    splitter = string.find('=')
    if splitter >= 1 and not string[splitter + 1] == '=' and splitter < 80:
      return splitter
    # parts = string.split('(')
    parts = re.split("(?<=[^\"])\((?!\")", string)
    fptr = re.compile('\*\w*\)')
    if len(parts) > 1:
      splitter = len(parts[0])
      for ii in range(1, len(parts)):
        # Don't split on the dot in "if (.condition)".
        if (not parts[ii - 1][-3:] == "if " and
            # Don't split "(.)" or "(.*fptr)".
            (len(parts[ii]) > 0 and
                not parts[ii][0] == ")" and not fptr.match(parts[ii]))
            and splitter < 80):
          return splitter
        splitter += len(parts[ii]) + 1
    done = False
    end = len(string)
    last_splitter = -1
    while not done:
      splitter = string[0:end].rfind(',')
      if splitter < 0 or (splitter > 0 and string[splitter - 1] == '"'):
        return last_splitter
      elif splitter >= 80:
        end = splitter
      else:
        return splitter

  def __WriteLine(self, line, ends_with_eol, split):
    """Given a signle line, writes it to a file, splitting if it's > 80 chars"""
    if len(line) >= 80:
      if split:
          i = self.__FindSplit(line)
      else:
          i = 0
      if i > 0:
        line1 = line[0:i + 1]
        if line1[-1] == ' ':
          line1 = line1[:-1]
        lineend = ''
        if line1[0] == '#':
          lineend = ' \\'
        nolint = ''
        self.__AddLine(line1 + nolint + lineend + '\n')
        match = re.match("( +)", line1)
        indent = ""
        if match:
          indent = match.group(1)
        splitter = line[i]
        if not splitter == ',':
          indent = "    " + indent
        self.__WriteLine(indent + line[i + 1:].lstrip(), True, split)
        return
    nolint = ''
    self.__AddLine(line + nolint)
    if ends_with_eol:
      self.__AddLine('\n')

  def __AddLine(self, line):
    self.content.append(line)

  def Close(self):
    """Close the file."""
    content = "".join(self.content)
    write_file = True
    if os.path.exists(self.filename):
      old_file = open(self.filename, "rb");
      old_content = old_file.read()
      old_file.close();
      if content == old_content:
        write_file = False
    if write_file:
      file = open(self.filename, "wb")
      file.write(content)
      file.close()


class CHeaderWriter(CWriter):
  """Writes a C Header file."""

  _non_alnum_re = re.compile(r'[^a-zA-Z0-9]')

  def __init__(self, filename, file_comment = None, guard_depth = 3):
    CWriter.__init__(self, filename)

    base = os.path.dirname(os.path.abspath(filename))
    for i in range(guard_depth):
      base = os.path.dirname(base)

    hpath = os.path.abspath(filename)[len(base) + 1:]
    self.guard = self._non_alnum_re.sub('_', hpath).upper() + '_'

    self.Write("%s\n" % _DO_NOT_EDIT_WARNING)
    if not file_comment == None:
      self.Write(file_comment)
    self.Write("#ifndef %s\n" % self.guard)
    self.Write("#define %s\n\n" % self.guard)

  def Close(self):
    self.Write("#endif  // %s\n\n" % self.guard)
    CWriter.Close(self)
