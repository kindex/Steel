/*********************************************************************NVMH1****
File:
nv_util.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#ifndef __nv_util_h__
#define __nv_util_h__

#ifdef _WIN32
#  include <windows.h>
#  pragma warning (disable:4786) // Disable the STL debug information warnings
#endif

#ifdef MACOS
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include "../opengl/glext.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include <assert.h>
#include <vector>
#include <string>
#include <map>

#include <limits.h>

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

#ifndef NV_UTIL_PROJECT //defined if building nv_util library
#if _MSC_VER >= 1300
#ifdef _DLL
#pragma message("Note: including lib: jpegMTDLL.lib\n")
#pragma comment(lib, "jpegMTDLL.lib")
#else
//#error "Your project doesn't use the Multithreaded DLL Runtime"
#endif
#endif
#endif

namespace nv_util
{
    const char *    set_search_path(const char * path);
    const char *    get_search_path();
    bool            findfile(const char * filename, int size, char * pathname);
} // nv_util

#include "nv_unzip.h"
#include "nv_tga.h"
#include "nv_jpeg.h"
#include "nv_ase.h"

#endif // __nv_util_h__  
