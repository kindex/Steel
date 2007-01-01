/*id*********************************************************
	File: graph/opengl/gl/libext.h
	Unit: opengl ext
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		??
	Description:
		Simple library to load basic extensions
 ************************************************************/

#ifndef __LIB_EXT__
#define __LIB_EXT__

#include "../../steel.h"

#ifdef  _WIN32
    #include    <windows.h>
#else
    #define GLX_GLXEXT_LEGACY
#endif

#include    <GL/gl.h>
#include    <GL/glu.h>
#include    "glext.h"


#ifdef  _WIN32
    #include    "wglext.h"
#else
    #include    <GL/glx.h>
    #include    <GL/glxext.h>
#endif

bool OpenGL_ExtensionsIsSupported(const char *ext);
//void OpenGL_ExtensionsInitAssertExtensions(const char *extList);
void OpenGL_ExtensionsInit();
void OpenGL_ExtensionsPrintfInfo();

extern bool GL_EXTENSION_MULTITEXTURE;
extern int	GL_EXTENSION_MULTITEXTURE_TEXTURE_UNITS;
extern bool GL_EXTENSION_VBO;
extern bool GL_EXTENSION_DOT3;
extern bool GL_EXTENSION_TEXTURE_CUBE_MAP;
extern bool GL_EXTENSION_TEXTURE_NON2POWER;
extern bool GL_EXTENSION_GLSL;

extern int GL_EXTENSION_MAX_VERTEX_UNIFORM_COMPONENTS;
extern int GL_EXTENSION_MAX_VERTEX_ATTRIBS;
extern int GL_EXTENSION_MAX_TEXTURE_IMAGE_UNITS;
extern int GL_EXTENSION_MAX_VERTEX_TEXTURE_IMAGE_UNITS;
extern int GL_EXTENSION_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
extern int GL_EXTENSION_MAX_VARYING_FLOATS;
extern int GL_EXTENSION_MAX_FRAGMENT_UNIFORM_COMPONENTS;
extern int GL_EXTENSION_MAX_TEXTURE_COORDS;

#ifdef  _WIN32
extern  PFNGLACTIVETEXTUREARBPROC               glActiveTextureARB;
extern  PFNGLCLIENTACTIVETEXTUREARBPROC         glClientActiveTextureARB;
extern  PFNGLMULTITEXCOORD1FARBPROC             glMultiTexCoord1f;
extern  PFNGLMULTITEXCOORD1FVARBPROC            glMultiTexCoord1fv;
extern  PFNGLMULTITEXCOORD2FARBPROC             glMultiTexCoord2f;
extern  PFNGLMULTITEXCOORD2FVARBPROC            glMultiTexCoord2fv;
extern  PFNGLMULTITEXCOORD3FARBPROC             glMultiTexCoord3f;
extern  PFNGLMULTITEXCOORD3FVARBPROC            glMultiTexCoord3fv;
extern  PFNGLMULTITEXCOORD4FARBPROC             glMultiTexCoord4f;
extern  PFNGLMULTITEXCOORD4FVARBPROC            glMultiTexCoord4fv;
#else
    #define  glActiveTextureARB         glActiveTexture
    #define  glClientActiveTextureARB   glClientActiveTexture
#endif
                                        // fog coord function
extern  PFNGLFOGCOORDFEXTPROC                      glFogCoordf;

                                        // secondary color functions
extern  PFNGLSECONDARYCOLOR3FPROC               glSecondaryColor3f;
extern  PFNGLSECONDARYCOLOR3FVPROC              glSecondaryColor3fv;
extern  PFNGLSECONDARYCOLORPOINTERPROC          glSecondaryColorPointer;

                                    // register combiners functions
extern  PFNGLCOMBINERPARAMETERFVNVPROC          glCombinerParameterfvNV;
extern  PFNGLCOMBINERPARAMETERIVNVPROC          glCombinerParameterivNV;
extern  PFNGLCOMBINERPARAMETERFNVPROC           glCombinerParameterfNV;
extern  PFNGLCOMBINERPARAMETERINVPROC           glCombinerParameteriNV;
extern  PFNGLCOMBINERINPUTNVPROC                glCombinerInputNV;
extern  PFNGLCOMBINEROUTPUTNVPROC               glCombinerOutputNV;
extern  PFNGLFINALCOMBINERINPUTNVPROC           glFinalCombinerInputNV;

                                    // VBO functions
extern  PFNGLBINDBUFFERARBPROC                  glBindBufferARB;
extern  PFNGLDELETEBUFFERSARBPROC               glDeleteBuffersARB;
extern  PFNGLGENBUFFERSARBPROC                  glGenBuffersARB;
extern  PFNGLISBUFFERARBPROC                    glIsBufferARB;
extern  PFNGLBUFFERDATAARBPROC                  glBufferDataARB;
extern  PFNGLBUFFERSUBDATAARBPROC               glBufferSubDataARB;
extern  PFNGLGETBUFFERSUBDATAARBPROC            glGetBufferSubDataARB;
extern  PFNGLMAPBUFFERARBPROC                   glMapBufferARB;
extern  PFNGLUNMAPBUFFERARBPROC                 glUnmapBufferARB;
extern  PFNGLGETBUFFERPARAMETERIVARBPROC        glGetBufferParameterivARB;
extern  PFNGLGETBUFFERPOINTERVARBPROC           glGetBufferPointervARB;

#ifdef  _WIN32
extern  PFNWGLGETEXTENSIONSSTRINGARBPROC        wglGetExtensionsStringARB;

                                    // p-buffer functions
extern  PFNWGLCREATEPBUFFERARBPROC              wglCreatePbufferARB;
extern  PFNWGLGETPBUFFERDCARBPROC               wglGetPbufferDCARB;
extern  PFNWGLRELEASEPBUFFERDCARBPROC           wglReleasePbufferDCARB;
extern  PFNWGLDESTROYPBUFFERARBPROC             wglDestroyPbufferARB;
extern  PFNWGLQUERYPBUFFERARBPROC               wglQueryPbufferARB;

                                    // WGL_ARB_pixel_format
extern  PFNWGLCHOOSEPIXELFORMATARBPROC          wglChoosePixelFormatARB;

                                    // WGL_ARB_render_texture
extern  PFNWGLBINDTEXIMAGEARBPROC               wglBindTexImageARB;
extern  PFNWGLRELEASETEXIMAGEARBPROC            wglReleaseTexImageARB;
extern  PFNWGLSETPBUFFERATTRIBARBPROC           wglSetPbufferAttribARB;

                                    // WGL_EXT_swap_control
extern  PFNWGLSWAPINTERVALEXTPROC               wglSwapIntervalEXT;
extern  PFNWGLGETSWAPINTERVALEXTPROC            wglGetSwapIntervalEXT;
#else
                                    // GLX_SGIX_pbuffer
extern  PFNGLXCREATEGLXPBUFFERSGIXPROC          glXCreateGLXPbufferSGIX;
extern  PFNGLXDESTROYGLXPBUFFERSGIXPROC         glXDestroyGLXPbufferSGIX;
extern  PFNGLXQUERYGLXPBUFFERSGIXPROC           glXQueryGLXPbufferSGIX;

                                    // GLX_SGIX_fbconfig
extern  PFNGLXCHOOSEFBCONFIGSGIXPROC            glXChooseFBConfigSGIX;
extern  PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC   glXCreateContextWithConfigSGIX;
#endif

                                    // NV-occlusion query extension
extern  PFNGLGENOCCLUSIONQUERIESNVPROC          glGenOcclusionQueriesNV;
extern  PFNGLBEGINOCCLUSIONQUERYNVPROC          glBeginOcclusionQueryNV;
extern  PFNGLENDOCCLUSIONQUERYNVPROC            glEndOcclusionQueryNV;
extern  PFNGLGETOCCLUSIONQUERYIVNVPROC          glGetOcclusionQueryivNV;

                                    // ARB_vertex_program extension
extern  PFNGLGENPROGRAMSARBPROC                 glGenProgramsARB;
extern  PFNGLDELETEPROGRAMSARBPROC              glDeleteProgramsARB;
extern  PFNGLBINDPROGRAMARBPROC                 glBindProgramARB;
extern  PFNGLISPROGRAMARBPROC                   glIsProgramARB;

extern  PFNGLPROGRAMSTRINGARBPROC               glProgramStringARB;
extern  PFNGLGETPROGRAMIVARBPROC                glGetProgramivARB;

extern  PFNGLVERTEXATTRIB4FARBPROC              glVertexAttrib4fARB;
extern  PFNGLVERTEXATTRIB4FVARBPROC             glVertexAttrib4fvARB;
extern  PFNGLVERTEXATTRIB3FARBPROC              glVertexAttrib3fARB;
extern  PFNGLVERTEXATTRIB3FVARBPROC             glVertexAttrib3fvARB;

extern  PFNGLVERTEXATTRIBPOINTERARBPROC         glVertexAttribPointerARB;
extern  PFNGLENABLEVERTEXATTRIBARRAYARBPROC     glEnableVertexAttribArrayARB;
extern  PFNGLDISABLEVERTEXATTRIBARRAYARBPROC    glDisableVertexAttribArrayARB;

extern  PFNGLPROGRAMLOCALPARAMETER4FARBPROC     glProgramLocalParameter4fARB;
extern  PFNGLPROGRAMLOCALPARAMETER4FVARBPROC    glProgramLocalParameter4fvARB;
extern  PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC  glGetProgramLocalParameterfvARB;

extern  PFNGLPROGRAMENVPARAMETER4FARBPROC       glProgramEnvParameter4fARB;
extern  PFNGLPROGRAMENVPARAMETER4FVARBPROC      glProgramEnvParameter4fvARB;
extern  PFNGLGETPROGRAMENVPARAMETERFVARBPROC    glGetProgramEnvParameterfvARB;

                                    // EXT_texture3D
extern  PFNGLTEXIMAGE3DEXTPROC                  glTexImage3DEXT;

                                    // ARB_texture_compression
extern  PFNGLCOMPRESSEDTEXIMAGE3DARBPROC        glCompressedTexImage3DARB;
extern  PFNGLCOMPRESSEDTEXIMAGE2DARBPROC        glCompressedTexImage2DARB;
extern  PFNGLCOMPRESSEDTEXIMAGE1DARBPROC        glCompressedTexImage1DARB;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC     glCompressedTexSubImage3DARB;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC     glCompressedTexSubImage2DARB;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC     glCompressedTexSubImage1DARB;
extern  PFNGLGETCOMPRESSEDTEXIMAGEARBPROC       glGetCompressedTexImageARB;

                                    // ARB_point_parameters
extern  PFNGLPOINTPARAMETERFARBPROC             glPointParameterfARB;
extern  PFNGLPOINTPARAMETERFVARBPROC            glPointParameterfvARB;

                                    // ARB_occlusion_query
extern  PFNGLGENQUERIESARBPROC                  glGenQueriesARB;
extern  PFNGLDELETEQUERIESARBPROC               glDeleteQueriesARB;
extern  PFNGLISQUERYARBPROC                     glIsQueryARB;
extern  PFNGLBEGINQUERYARBPROC                  glBeginQueryARB;
extern  PFNGLENDQUERYARBPROC                    glEndQueryARB;
extern  PFNGLGETQUERYIVARBPROC                  glGetQueryivARB;
extern  PFNGLGETQUERYOBJECTIVARBPROC            glGetQueryObjectivARB;
extern  PFNGLGETQUERYOBJECTUIVARBPROC           glGetQueryObjectuivARB;

                                    // GLSL-specific extensions
                                    // GL_ARB_shader_object
extern  PFNGLDELETEOBJECTARBPROC                glDeleteObjectARB;
extern  PFNGLGETHANDLEARBPROC                   glGetHandleARB;
extern  PFNGLDETACHOBJECTARBPROC                glDetachObjectARB;
extern  PFNGLCREATESHADEROBJECTARBPROC          glCreateShaderObjectARB;
extern  PFNGLSHADERSOURCEARBPROC                glShaderSourceARB;
extern  PFNGLCOMPILESHADERARBPROC               glCompileShaderARB;
extern  PFNGLCREATEPROGRAMOBJECTARBPROC         glCreateProgramObjectARB;
extern  PFNGLATTACHOBJECTARBPROC                glAttachObjectARB;
extern  PFNGLLINKPROGRAMARBPROC                 glLinkProgramARB;
extern  PFNGLUSEPROGRAMOBJECTARBPROC            glUseProgramObjectARB;
extern  PFNGLVALIDATEPROGRAMARBPROC             glValidateProgramARB;
extern  PFNGLUNIFORM1FARBPROC                   glUniform1fARB;
extern  PFNGLUNIFORM2FARBPROC                   glUniform2fARB;
extern  PFNGLUNIFORM3FARBPROC                   glUniform3fARB;
extern  PFNGLUNIFORM4FARBPROC                   glUniform4fARB;
extern  PFNGLUNIFORM1IARBPROC                   glUniform1iARB;
extern  PFNGLUNIFORM2IARBPROC                   glUniform2iARB;
extern  PFNGLUNIFORM3IARBPROC                   glUniform3iARB;
extern  PFNGLUNIFORM4IARBPROC                   glUniform4iARB;
extern  PFNGLUNIFORM1FVARBPROC                  glUniform1fvARB;
extern  PFNGLUNIFORM2FVARBPROC                  glUniform2fvARB;
extern  PFNGLUNIFORM3FVARBPROC                  glUniform3fvARB;
extern  PFNGLUNIFORM4FVARBPROC                  glUniform4fvARB;
extern  PFNGLUNIFORM1IVARBPROC                  glUniform1ivARB;
extern  PFNGLUNIFORM2IVARBPROC                  glUniform2ivARB;
extern  PFNGLUNIFORM3IVARBPROC                  glUniform3ivARB;
extern  PFNGLUNIFORM4IVARBPROC                  glUniform4ivARB;
extern  PFNGLUNIFORMMATRIX2FVARBPROC            glUniformMatrix2fvARB;
extern  PFNGLUNIFORMMATRIX3FVARBPROC            glUniformMatrix3fvARB;
extern  PFNGLUNIFORMMATRIX4FVARBPROC            glUniformMatrix4fvARB;
extern  PFNGLGETOBJECTPARAMETERFVARBPROC        glGetObjectParameterfvARB;
extern  PFNGLGETOBJECTPARAMETERIVARBPROC        glGetObjectParameterivARB;
extern  PFNGLGETINFOLOGARBPROC                  glGetInfoLogARB;
extern  PFNGLGETATTACHEDOBJECTSARBPROC          glGetAttachedObjectsARB;
extern  PFNGLGETUNIFORMLOCATIONARBPROC          glGetUniformLocationARB;
extern  PFNGLGETACTIVEUNIFORMARBPROC            glGetActiveUniformARB;
extern  PFNGLGETUNIFORMFVARBPROC                glGetUniformfvARB;
extern  PFNGLGETUNIFORMIVARBPROC                glGetUniformivARB;
extern  PFNGLGETSHADERSOURCEARBPROC             glGetShaderSourceARB;

                                        // GL_ARB_vertex_shader
extern  PFNGLBINDATTRIBLOCATIONARBPROC          glBindAttribLocationARB;
extern  PFNGLGETACTIVEATTRIBARBPROC             glGetActiveAttribARB;
extern  PFNGLGETATTRIBLOCATIONARBPROC           glGetAttribLocationARB;
extern  PFNGLGETVERTEXATTRIBFVARBPROC           glGetVertexAttribfvARB;

                                        // EXT_stencil_two_side
extern  PFNGLACTIVESTENCILFACEEXTPROC           glActiveStencilFaceEXT;

					// EXT_depth_bounds_test
extern	PFNGLDEPTHBOUNDSEXTPROC			glDepthBoundsEXT;

#endif
