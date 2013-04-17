#!/usr/bin/env python
# Copyright (c) 2012 The Chromium Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""code generator for GLES2 command buffers."""
import json
import os
import os.path
import sys
import re
from optparse import OptionParser
from generator import *

_SIZE_OF_UINT32 = 4
_SIZE_OF_COMMAND_HEADER = 4
_FIRST_SPECIFIC_COMMAND_ID = 256



def main(argv):
  """This is the main function."""
  parser = OptionParser()
  parser.add_option(
      "-g", "--generate-implementation-templates", action="store_true",
      help="generates files that are generally hand edited..")
  parser.add_option(
      "--output-dir",
      help="base directory for resulting files, under chrome/src. default is "
      "empty. Use this if you want the result stored under gen.")
  parser.add_option(
      "-v", "--verbose", action="store_true",
      help="prints more output.")

  (options, args) = parser.parse_args(args=argv)

  script_dir = os.path.dirname(__file__)
  if script_dir:
      os.chdir(script_dir)

  gen = GLGenerator(options.verbose)
  gen.ParseAPIFiles()

  # Support generating files under gen/
  if options.output_dir != None:
    os.chdir(options.output_dir)

  # Shared between the client and the server.
  gen.WriteDispatchTable("dispatch_table_autogen.h")
  gen.WritePassthroughDispatchTableImplementation("dispatch_table_autogen.c")
  gen.WriteCommandHeader("command_autogen.h")
  gen.WriteCommandEnum("command_types_autogen.h")

  gen.WriteEnumValidation("enum_validation.h")

  # These are used on the client-side.
  gen.WriteCommandInitilizationAndSizeFunction("command_autogen.c")
  gen.WriteClientEntryPoints("client_entry_points.c")
  gen.WriteBaseClient("client_autogen.c")
  gen.WriteCachingClientDispatchTableImplementation("caching_client_dispatch_autogen.c")

  # These are used on the server-side.
  gen.WriteBaseServer("server_autogen.c")

  if gen.errors > 0:
    print "%d errors" % gen.errors
    return 1
  return 0


if __name__ == '__main__':
  sys.exit(main(sys.argv[1:]))
