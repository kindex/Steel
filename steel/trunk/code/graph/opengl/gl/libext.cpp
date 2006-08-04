/*id*********************************************************
	Unit: GraphEngine - OpenGL Engine - OpenGL extensions
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

#include "../../../steel.h"
#include "../../../_cpp.h"

#if STEEL_OS == OS_WIN32
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"
#elif STEEL_OS == OS_LINUX
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "libext.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <malloc.h>


bool GL_EXTENSION_MULTITEXTURE = false;
int  GL_EXTENSION_MULTITEXTURE_TEXTURE_UNITS = 0;
bool GL_EXTENSION_VBO = false;
bool GL_EXTENSION_DOT3 = false;
bool GL_EXTENSION_TEXTURE_CUBE_MAP = false;
bool GL_EXTENSION_TEXTURE_NON2POWER = false;


// multitexture functions
#ifdef	_WIN32
// under Linux all multitexture is
// supported by OpenGL itself
PFNGLACTIVETEXTUREARBPROC				glActiveTextureARB		= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC			glClientActiveTextureARB= NULL;
PFNGLMULTITEXCOORD1FARBPROC				glMultiTexCoord1f		= NULL;
PFNGLMULTITEXCOORD1FVARBPROC			glMultiTexCoord1fv		= NULL;
PFNGLMULTITEXCOORD2FARBPROC				glMultiTexCoord2f		= NULL;
PFNGLMULTITEXCOORD2FVARBPROC			glMultiTexCoord2fv		= NULL;
PFNGLMULTITEXCOORD3FARBPROC				glMultiTexCoord3f		= NULL;
PFNGLMULTITEXCOORD3FVARBPROC			glMultiTexCoord3fv		= NULL;
PFNGLMULTITEXCOORD4FARBPROC				glMultiTexCoord4f		= NULL;
PFNGLMULTITEXCOORD4FVARBPROC			glMultiTexCoord4fv		= NULL;
#endif

// fog coord function
PFNGLFOGCOORDFEXTPROC					glFogCoordf				= NULL;

// secondary color functions
PFNGLSECONDARYCOLOR3FPROC				glSecondaryColor3f		= NULL;
PFNGLSECONDARYCOLOR3FVPROC				glSecondaryColor3fv		= NULL;
PFNGLSECONDARYCOLORPOINTERPROC			glSecondaryColorPointer	= NULL;

// register combiners functions
PFNGLCOMBINERPARAMETERFVNVPROC			glCombinerParameterfvNV	= NULL;
PFNGLCOMBINERPARAMETERIVNVPROC			glCombinerParameterivNV	= NULL;
PFNGLCOMBINERPARAMETERFNVPROC			glCombinerParameterfNV	= NULL;
PFNGLCOMBINERPARAMETERINVPROC			glCombinerParameteriNV	= NULL;
PFNGLCOMBINERINPUTNVPROC				glCombinerInputNV		= NULL;
PFNGLCOMBINEROUTPUTNVPROC				glCombinerOutputNV		= NULL;
PFNGLFINALCOMBINERINPUTNVPROC			glFinalCombinerInputNV	= NULL;

// VBO functions
PFNGLBINDBUFFERARBPROC					glBindBufferARB			= NULL;
PFNGLDELETEBUFFERSARBPROC				glDeleteBuffersARB		= NULL;
PFNGLGENBUFFERSARBPROC					glGenBuffersARB			= NULL;
PFNGLISBUFFERARBPROC					glIsBufferARB			= NULL;
PFNGLBUFFERDATAARBPROC					glBufferDataARB			= NULL;
PFNGLBUFFERSUBDATAARBPROC				glBufferSubDataARB		= NULL;
PFNGLGETBUFFERSUBDATAARBPROC			glGetBufferSubDataARB	= NULL;
PFNGLMAPBUFFERARBPROC					glMapBufferARB			= NULL;
PFNGLUNMAPBUFFERARBPROC					glUnmapBufferARB		= NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC		glGetBufferParameterivARB = NULL;
PFNGLGETBUFFERPOINTERVARBPROC			glGetBufferPointervARB	= NULL;


#ifdef	_WIN32
PFNWGLGETEXTENSIONSSTRINGARBPROC		wglGetExtensionsStringARB = NULL;

// p-buffer functions
PFNWGLCREATEPBUFFERARBPROC				wglCreatePbufferARB		= NULL;
PFNWGLGETPBUFFERDCARBPROC				wglGetPbufferDCARB		= NULL;
PFNWGLRELEASEPBUFFERDCARBPROC			wglReleasePbufferDCARB	= NULL;
PFNWGLDESTROYPBUFFERARBPROC				wglDestroyPbufferARB	= NULL;
PFNWGLQUERYPBUFFERARBPROC				wglQueryPbufferARB		= NULL;

// WGL_ARB_pixel_format
PFNWGLCHOOSEPIXELFORMATARBPROC			wglChoosePixelFormatARB = NULL;

// WGL_ARB_render_texture
PFNWGLBINDTEXIMAGEARBPROC				wglBindTexImageARB		= NULL;
PFNWGLRELEASETEXIMAGEARBPROC			wglReleaseTexImageARB	= NULL;
PFNWGLSETPBUFFERATTRIBARBPROC			wglSetPbufferAttribARB	= NULL;

// WGL_EXT_swap_control
PFNWGLSWAPINTERVALEXTPROC				wglSwapIntervalEXT		= NULL;
PFNWGLGETSWAPINTERVALEXTPROC			wglGetSwapIntervalEXT	= NULL;
#else

// GLX_SGIX_pbuffer
PFNGLXCREATEGLXPBUFFERSGIXPROC			glXCreateGLXPbufferSGIX		= NULL;
PFNGLXDESTROYGLXPBUFFERSGIXPROC			glXDestroyGLXPbufferSGIX	= NULL;
PFNGLXQUERYGLXPBUFFERSGIXPROC			glXQueryGLXPbufferSGIX		= NULL;

// GLX_SGIX_fbconfig
PFNGLXCHOOSEFBCONFIGSGIXPROC			glXChooseFBConfigSGIX			= NULL;
PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC	glXCreateContextWithConfigSGIX	= NULL;
#endif

// NV-occlusion query extension
PFNGLGENOCCLUSIONQUERIESNVPROC			glGenOcclusionQueriesNV = NULL;
PFNGLBEGINOCCLUSIONQUERYNVPROC			glBeginOcclusionQueryNV = NULL;
PFNGLENDOCCLUSIONQUERYNVPROC			glEndOcclusionQueryNV	= NULL;
PFNGLGETOCCLUSIONQUERYIVNVPROC			glGetOcclusionQueryivNV = NULL;

// ARB_vertex_program extension
PFNGLGENPROGRAMSARBPROC					glGenProgramsARB		= NULL;
PFNGLDELETEPROGRAMSARBPROC				glDeleteProgramsARB		= NULL;
PFNGLBINDPROGRAMARBPROC					glBindProgramARB		= NULL;
PFNGLISPROGRAMARBPROC					glIsProgramARB			= NULL;

PFNGLPROGRAMSTRINGARBPROC				glProgramStringARB		= NULL;
PFNGLGETPROGRAMIVARBPROC				glGetProgramivARB		= NULL;

PFNGLVERTEXATTRIB4FARBPROC				glVertexAttrib4fARB		= NULL;
PFNGLVERTEXATTRIB4FVARBPROC				glVertexAttrib4fvARB	= NULL;
PFNGLVERTEXATTRIB3FARBPROC				glVertexAttrib3fARB		= NULL;
PFNGLVERTEXATTRIB3FVARBPROC				glVertexAttrib3fvARB	= NULL;

PFNGLVERTEXATTRIBPOINTERARBPROC			glVertexAttribPointerARB		= NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC		glEnableVertexAttribArrayARB	= NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC	glDisableVertexAttribArrayARB	= NULL;

PFNGLPROGRAMLOCALPARAMETER4FARBPROC		glProgramLocalParameter4fARB	= NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC	glProgramLocalParameter4fvARB	= NULL;

PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC	glGetProgramLocalParameterfvARB = NULL;

PFNGLPROGRAMENVPARAMETER4FARBPROC		glProgramEnvParameter4fARB		= NULL;
PFNGLPROGRAMENVPARAMETER4FVARBPROC		glProgramEnvParameter4fvARB		= NULL;

PFNGLGETPROGRAMENVPARAMETERFVARBPROC	glGetProgramEnvParameterfvARB	= NULL;

// GL_EXT_texture3D
PFNGLTEXIMAGE3DEXTPROC					glTexImage3DEXT					= NULL;

// ARB_texture_compression
PFNGLCOMPRESSEDTEXIMAGE3DARBPROC		glCompressedTexImage3DARB		= NULL;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC		glCompressedTexImage2DARB		= NULL;
PFNGLCOMPRESSEDTEXIMAGE1DARBPROC		glCompressedTexImage1DARB		= NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC		glCompressedTexSubImage3DARB	= NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC		glCompressedTexSubImage2DARB	= NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC		glCompressedTexSubImage1DARB	= NULL;
PFNGLGETCOMPRESSEDTEXIMAGEARBPROC		glGetCompressedTexImageARB		= NULL;

// ARB_point_parameters
PFNGLPOINTPARAMETERFARBPROC				glPointParameterfARB			= NULL;
PFNGLPOINTPARAMETERFVARBPROC			glPointParameterfvARB			= NULL;

// ARB_occlusion_query
PFNGLGENQUERIESARBPROC					glGenQueriesARB			= NULL;
PFNGLDELETEQUERIESARBPROC				glDeleteQueriesARB		= NULL;
PFNGLISQUERYARBPROC						glIsQueryARB			= NULL;
PFNGLBEGINQUERYARBPROC					glBeginQueryARB			= NULL;
PFNGLENDQUERYARBPROC					glEndQueryARB			= NULL;
PFNGLGETQUERYIVARBPROC					glGetQueryivARB			= NULL;
PFNGLGETQUERYOBJECTIVARBPROC			glGetQueryObjectivARB	= NULL;
PFNGLGETQUERYOBJECTUIVARBPROC			glGetQueryObjectuivARB	= NULL;

// GLSL-specific extensions
// GL_ARB_shader_object
PFNGLDELETEOBJECTARBPROC				glDeleteObjectARB			= NULL;
PFNGLGETHANDLEARBPROC					glGetHandleARB				= NULL;
PFNGLDETACHOBJECTARBPROC				glDetachObjectARB			= NULL;
PFNGLCREATESHADEROBJECTARBPROC			glCreateShaderObjectARB		= NULL;
PFNGLSHADERSOURCEARBPROC				glShaderSourceARB			= NULL;
PFNGLCOMPILESHADERARBPROC				glCompileShaderARB			= NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC			glCreateProgramObjectARB	= NULL;
PFNGLATTACHOBJECTARBPROC				glAttachObjectARB			= NULL;
PFNGLLINKPROGRAMARBPROC					glLinkProgramARB			= NULL;
PFNGLUSEPROGRAMOBJECTARBPROC			glUseProgramObjectARB		= NULL;
PFNGLVALIDATEPROGRAMARBPROC				glValidateProgramARB		= NULL;
PFNGLUNIFORM1FARBPROC					glUniform1fARB				= NULL;
PFNGLUNIFORM2FARBPROC					glUniform2fARB				= NULL;
PFNGLUNIFORM3FARBPROC					glUniform3fARB				= NULL;
PFNGLUNIFORM4FARBPROC					glUniform4fARB				= NULL;
PFNGLUNIFORM1IARBPROC					glUniform1iARB				= NULL;
PFNGLUNIFORM2IARBPROC					glUniform2iARB				= NULL;
PFNGLUNIFORM3IARBPROC					glUniform3iARB				= NULL;
PFNGLUNIFORM4IARBPROC					glUniform4iARB				= NULL;
PFNGLUNIFORM1FVARBPROC					glUniform1fvARB				= NULL;
PFNGLUNIFORM2FVARBPROC					glUniform2fvARB				= NULL;
PFNGLUNIFORM3FVARBPROC					glUniform3fvARB				= NULL;
PFNGLUNIFORM4FVARBPROC					glUniform4fvARB				= NULL;
PFNGLUNIFORM1IVARBPROC					glUniform1ivARB				= NULL;
PFNGLUNIFORM2IVARBPROC					glUniform2ivARB				= NULL;
PFNGLUNIFORM3IVARBPROC					glUniform3ivARB				= NULL;
PFNGLUNIFORM4IVARBPROC					glUniform4ivARB				= NULL;
PFNGLUNIFORMMATRIX2FVARBPROC			glUniformMatrix2fvARB		= NULL;
PFNGLUNIFORMMATRIX3FVARBPROC			glUniformMatrix3fvARB		= NULL;
PFNGLUNIFORMMATRIX4FVARBPROC			glUniformMatrix4fvARB		= NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC		glGetObjectParameterfvARB	= NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC		glGetObjectParameterivARB	= NULL;
PFNGLGETINFOLOGARBPROC					glGetInfoLogARB				= NULL;
PFNGLGETATTACHEDOBJECTSARBPROC			glGetAttachedObjectsARB		= NULL;
PFNGLGETUNIFORMLOCATIONARBPROC			glGetUniformLocationARB		= NULL;
PFNGLGETACTIVEUNIFORMARBPROC			glGetActiveUniformARB		= NULL;
PFNGLGETUNIFORMFVARBPROC				glGetUniformfvARB			= NULL;
PFNGLGETUNIFORMIVARBPROC				glGetUniformivARB			= NULL;
PFNGLGETSHADERSOURCEARBPROC				glGetShaderSourceARB		= NULL;

// GL_ARB_vertex_shader
PFNGLBINDATTRIBLOCATIONARBPROC			glBindAttribLocationARB		= NULL;
PFNGLGETACTIVEATTRIBARBPROC				glGetActiveAttribARB		= NULL;
PFNGLGETATTRIBLOCATIONARBPROC			glGetAttribLocationARB		= NULL;
PFNGLGETVERTEXATTRIBFVARBPROC			glGetVertexAttribfvARB		= NULL;

// EXT_stencil_two_side
PFNGLACTIVESTENCILFACEEXTPROC			glActiveStencilFaceEXT		= NULL;

// EXT_depth_bounds_test
PFNGLDEPTHBOUNDSEXTPROC					glDepthBoundsEXT			= NULL;

////////////////////////// functions ////////////////////////////

static bool OpenGL_ExtensionsIsSupported(const char *ext, const char *extList)
{
	const char *start = extList;
	const char *ptr;

	while ( ( ptr = strstr ( start, ext ) ) != NULL )
	{
	// we've found, ensure name is exactly ext
		const char * end = ptr + strlen ( ext );

		if ( isspace ( *end ) || *end == '\0' )
			return true;

		start = end;
	}

	return false;
}

bool OpenGL_ExtensionsIsSupported( const char * ext )
{
//	OpenGL_ExtensionsInit();						// since we will need it for platform-specific extensions

	const char *extensions = (const char *) glGetString ( GL_EXTENSIONS );

	if(OpenGL_ExtensionsIsSupported(ext, extensions))
		return true;

#ifdef	_WIN32								// check Windoze extensions
	if ( wglGetExtensionsStringARB == NULL )
	return false;

	return OpenGL_ExtensionsIsSupported(ext, wglGetExtensionsStringARB(wglGetCurrentDC()));
#else										// check GLX extensions
	Display * display = glXGetCurrentDisplay ();
	int		screen	= DefaultScreen		( display );

	return OpenGL_ExtensionsIsSupported ( ext, glXQueryExtensionsString ( display, screen ) );
#endif
}

/*void assertExtensionsSupported(const char * extList)
{
	char * str = strdup ( extList );

	for ( char * ptr = strtok ( str, " ,;\t" ); ptr != NULL; ptr = strtok ( NULL, " ,;\t" ) )
		if ( !isExtensionSupported ( ptr ) )
		{
			log_msg("grapg opengl error", std::string("Extension ") + ptr + " not supported");

			free(str);
			exit(1);
		}

	free(str);
}*/

//
// hide platform specific in one place
//

static void * getProcAddress ( const char * name )
{
#ifdef	_WIN32
	return (void*)wglGetProcAddress ( name );
#else
	return (void *)glXGetProcAddressARB ( (const GLubyte *)name );
#endif
}

#ifdef	_WIN32
bool	initWin32Extensions()
{
	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) getProcAddress ( "wglGetExtensionsStringARB" );

	if(wglGetExtensionsStringARB == NULL)
		return false;

	const char * exts = wglGetExtensionsStringARB(wglGetCurrentDC());
	log_msg("graph opengl opengl_info", std::string("Supported WGL extensions: ") + strtr(exts, ' ', '\n' ));

	if ( exts == NULL )
		return false;

	if ( strstr ( exts, "WGL_ARB_pbuffer" ) != NULL )
	{
		wglCreatePbufferARB	= (PFNWGLCREATEPBUFFERARBPROC)	 wglGetProcAddress ( "wglCreatePbufferARB"	);
		wglGetPbufferDCARB	 = (PFNWGLGETPBUFFERDCARBPROC)		wglGetProcAddress ( "wglGetPbufferDCARB"	 );
		wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)	wglGetProcAddress ( "wglReleasePbufferDCARB" );
		wglDestroyPbufferARB	= (PFNWGLDESTROYPBUFFERARBPROC)	wglGetProcAddress ( "wglDestroyPbufferARB"	);
		wglQueryPbufferARB	 = (PFNWGLQUERYPBUFFERARBPROC)		wglGetProcAddress ( "wglQueryPbufferARB"	 );
		log_msg("graph opengl opengl_info", "WGL_ARB_pbuffer supported");
	}
	else
		log_msg("error graph opengl opengl_info", "WGL_ARB_pbuffer not supported");

	if ( strstr ( exts, "WGL_ARB_pixel_format" ) != NULL )
	{
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress ( "wglChoosePixelFormatARB" );
		log_msg("graph opengl opengl_info", "WGL_ARB_pixel_format supported");
	}
	else
		log_msg("error graph opengl opengl_info", "WGL_ARB_pixel_format not supported");

	if ( strstr ( exts, "WGL_ARB_render_texture" ) != NULL )
	{
		wglBindTexImageARB		= (PFNWGLBINDTEXIMAGEARBPROC)	 wglGetProcAddress ( "wglBindTexImageARB"	 );
		wglReleaseTexImageARB	= (PFNWGLRELEASETEXIMAGEARBPROC)	wglGetProcAddress ( "wglReleaseTexImageARB"	);
		wglSetPbufferAttribARB	= (PFNWGLSETPBUFFERATTRIBARBPROC) wglGetProcAddress ( "wglSetPbufferAttribARB" );
		log_msg("graph opengl opengl_info", "WGL_ARB_render_texture supported");
	}
	else
		log_msg("error graph opengl opengl_info", "WGL_ARB_render_texture not supported");

	if ( strstr ( exts, "WGL_EXT_swap_control" ) != NULL )
	{
		wglSwapIntervalEXT		= (PFNWGLSWAPINTERVALEXTPROC)		wglGetProcAddress ( "wglSwapIntervalEXT"	);
		wglGetSwapIntervalEXT	= (PFNWGLGETSWAPINTERVALEXTPROC)	wglGetProcAddress ( "wglGetSwapIntervalEXT" );
		log_msg("graph opengl opengl_info", "WGL_ARB_render_texture supported");
	}
	else
		log_msg("error graph opengl opengl_info", "WGL_ARB_render_texture not supported");
	

	return true;
}
#else
void	initLinuxExtensions ()
{
									// GLX_SGIX_pbuffer
	glXCreateGLXPbufferSGIX	= (PFNGLXCREATEGLXPBUFFERSGIXPROC)	getProcAddress ( "glXCreateGLXPbufferSGIX" );
	glXDestroyGLXPbufferSGIX = (PFNGLXDESTROYGLXPBUFFERSGIXPROC) getProcAddress ( "glXDestroyGLXPbufferSGIX" );
	glXQueryGLXPbufferSGIX	= (PFNGLXQUERYGLXPBUFFERSGIXPROC)	getProcAddress ( "glXQueryGLXPbufferSGIX" );

									// GLX_SGIX_fbconfig
	glXChooseFBConfigSGIX			= (PFNGLXCHOOSEFBCONFIGSGIXPROC)			getProcAddress ( "glXChooseFBConfigSGIX" );
	glXCreateContextWithConfigSGIX = (PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC) getProcAddress ( "glXCreateContextWithConfigSGIX" );

	// TODO - not tested
	Display * display = glXGetCurrentDisplay ();
	int		screen	= DefaultScreen		( display );
	const char * exts = glXQueryExtensionsString(display, screen);
	log_msg("graph opengl opengl_info", std::string("Supported glX extensions: ") + strtr(exts, ' ', '\n' ));
}
#endif

void OpenGL_ExtensionsInit()
{
	static bool initialized = false;					// whether initExtensions has already been called

	if(initialized)
		return;

#ifdef	_WIN32
	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		getProcAddress ( "glActiveTextureARB"		);
	glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)getProcAddress ( "glClientActiveTextureARB" );
	glMultiTexCoord1f		= (PFNGLMULTITEXCOORD1FARBPROC)		getProcAddress ( "glMultiTexCoord1fARB"	 );
	glMultiTexCoord1fv		= (PFNGLMULTITEXCOORD1FVARBPROC)	getProcAddress ( "glMultiTexCoord1fvARB"	);
	glMultiTexCoord2f		= (PFNGLMULTITEXCOORD2FARBPROC)		getProcAddress ( "glMultiTexCoord2fARB"	 );
	glMultiTexCoord2fv		= (PFNGLMULTITEXCOORD2FVARBPROC)	getProcAddress ( "glMultiTexCoord2fvARB"	);
	glMultiTexCoord3f		= (PFNGLMULTITEXCOORD3FARBPROC)		getProcAddress ( "glMultiTexCoord3fARB"	 );
	glMultiTexCoord3fv		= (PFNGLMULTITEXCOORD3FVARBPROC)	getProcAddress ( "glMultiTexCoord3fvARB"	);
	glMultiTexCoord4f		= (PFNGLMULTITEXCOORD4FARBPROC)		getProcAddress ( "glMultiTexCoord4fARB"	 );
	glMultiTexCoord4fv		= (PFNGLMULTITEXCOORD4FVARBPROC)	getProcAddress ( "glMultiTexCoord4fvARB"	);
#endif

	glFogCoordf				= (PFNGLFOGCOORDFEXTPROC)			getProcAddress ( "glFogCoordfEXT"		 );

	glSecondaryColor3f		= (PFNGLSECONDARYCOLOR3FPROC)		getProcAddress ( "glSecondaryColor3fEXT"		);
	glSecondaryColor3fv		= (PFNGLSECONDARYCOLOR3FVPROC)		getProcAddress ( "glSecondaryColor3fvEXT"	 );
	glSecondaryColorPointer	= (PFNGLSECONDARYCOLORPOINTERPROC)	getProcAddress ( "glSecondaryColorPointerEXT" );

	glCombinerParameterfvNV	= (PFNGLCOMBINERPARAMETERFVNVPROC)	getProcAddress ( "glCombinerParameterfvNV"	);
	glCombinerParameterivNV	= (PFNGLCOMBINERPARAMETERIVNVPROC)	getProcAddress ( "glCombinerParameterivNV"	);
	glCombinerParameterfNV	= (PFNGLCOMBINERPARAMETERFNVPROC)	getProcAddress ( "glCombinerParameterfNV"	);
	glCombinerParameteriNV	= (PFNGLCOMBINERPARAMETERINVPROC)	getProcAddress ( "glCombinerParameteriNV"	);
	glCombinerInputNV		= (PFNGLCOMBINERINPUTNVPROC)		getProcAddress ( "glCombinerInputNV"		);
	glCombinerOutputNV		= (PFNGLCOMBINEROUTPUTNVPROC)		getProcAddress ( "glCombinerOutputNV"		);
	glFinalCombinerInputNV	= (PFNGLFINALCOMBINERINPUTNVPROC)	getProcAddress ( "glFinalCombinerInputNV"	);

	glBindBufferARB			= (PFNGLBINDBUFFERARBPROC)			getProcAddress ( "glBindBufferARB"			);
	glDeleteBuffersARB		= (PFNGLDELETEBUFFERSARBPROC)		getProcAddress ( "glDeleteBuffersARB"		);
	glGenBuffersARB			= (PFNGLGENBUFFERSARBPROC)			getProcAddress ( "glGenBuffersARB"			);
	glIsBufferARB			 = (PFNGLISBUFFERARBPROC)			getProcAddress ( "glIsBufferARB"			 );
	glBufferDataARB			= (PFNGLBUFFERDATAARBPROC)			getProcAddress ( "glBufferDataARB"			);
	glBufferSubDataARB		= (PFNGLBUFFERSUBDATAARBPROC)		getProcAddress ( "glBufferSubDataARB"		);
	glGetBufferSubDataARB	 = (PFNGLGETBUFFERSUBDATAARBPROC)	getProcAddress ( "glGetBufferSubDataARB"	 );
	glMapBufferARB			= (PFNGLMAPBUFFERARBPROC)			getProcAddress ( "glMapBufferARB"			);
	glUnmapBufferARB			= (PFNGLUNMAPBUFFERARBPROC)		getProcAddress ( "glUnmapBufferARB"			);
	glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC) getProcAddress ( "glGetBufferParameterivARB" );
	glGetBufferPointervARB	= (PFNGLGETBUFFERPOINTERVARBPROC)	getProcAddress ( "glGetBufferPointervARB"	);

									// NV-occlusion query extension
	glGenOcclusionQueriesNV = (PFNGLGENOCCLUSIONQUERIESNVPROC)	getProcAddress ( "glGenOcclusionQueriesNV" );
	glBeginOcclusionQueryNV = (PFNGLBEGINOCCLUSIONQUERYNVPROC)	getProcAddress ( "glBeginOcclusionQueryNV" );
	glEndOcclusionQueryNV	= (PFNGLENDOCCLUSIONQUERYNVPROC)	getProcAddress ( "glEndOcclusionQueryNV"	);
	glGetOcclusionQueryivNV = (PFNGLGETOCCLUSIONQUERYIVNVPROC)	getProcAddress ( "glGetOcclusionQueryivNV" );

									// ARB_vertex_program extension
	glGenProgramsARB				= (PFNGLGENPROGRAMSARBPROC)				getProcAddress ( "glGenProgramsARB" );
	glDeleteProgramsARB			 = (PFNGLDELETEPROGRAMSARBPROC)				getProcAddress ( "glDeleteProgramsARB" );
	glBindProgramARB				= (PFNGLBINDPROGRAMARBPROC)				getProcAddress ( "glBindProgramARB" );
	glIsProgramARB					= (PFNGLISPROGRAMARBPROC)				getProcAddress ( "glIsProgramARB" );
	glProgramStringARB				= (PFNGLPROGRAMSTRINGARBPROC)			getProcAddress ( "glProgramStringARB" );
	glGetProgramivARB				= (PFNGLGETPROGRAMIVARBPROC)			getProcAddress ( "glGetProgramivARB" );
	glVertexAttrib4fARB			 = (PFNGLVERTEXATTRIB4FARBPROC)				getProcAddress ( "glVertexAttrib4fARB" );
	glVertexAttrib4fvARB			= (PFNGLVERTEXATTRIB4FVARBPROC)			getProcAddress ( "glVertexAttrib4fvARB" );
	glVertexAttrib3fARB			 = (PFNGLVERTEXATTRIB3FARBPROC)				getProcAddress ( "glVertexAttrib3fARB" );
	glVertexAttrib3fvARB			= (PFNGLVERTEXATTRIB3FVARBPROC)			getProcAddress ( "glVertexAttrib3fvARB" );
	glVertexAttribPointerARB		= (PFNGLVERTEXATTRIBPOINTERARBPROC)		getProcAddress ( "glVertexAttribPointerARB" );
	glEnableVertexAttribArrayARB	= (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)	getProcAddress ( "glEnableVertexAttribArrayARB" );
	glDisableVertexAttribArrayARB	= (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)getProcAddress ( "glDisableVertexAttribArrayARB" );
	glProgramLocalParameter4fARB	= (PFNGLPROGRAMLOCALPARAMETER4FARBPROC)	getProcAddress ( "glProgramLocalParameter4fARB" );
	glProgramLocalParameter4fvARB	= (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)getProcAddress ( "glProgramLocalParameter4fvARB" );
	glGetProgramLocalParameterfvARB = (PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC)getProcAddress ( "glGetProgramLocalParameterfvARB" );
	glProgramEnvParameter4fARB		= (PFNGLPROGRAMENVPARAMETER4FARBPROC)	getProcAddress ( "glProgramEnvParameter4fARB" );
	glProgramEnvParameter4fvARB	 = (PFNGLPROGRAMENVPARAMETER4FVARBPROC)		getProcAddress ( "glProgramEnvParameter4fvARB" );
	glGetProgramEnvParameterfvARB	= (PFNGLGETPROGRAMENVPARAMETERFVARBPROC)getProcAddress ( "glGetProgramEnvParameterfvARB" );

									// GL_EXT_texture3D
	glTexImage3DEXT				 =(PFNGLTEXIMAGE3DEXTPROC)					getProcAddress ( "glTexImage3DEXT" );

									// ARB_texture_compression
	glCompressedTexImage3DARB		= (PFNGLCOMPRESSEDTEXIMAGE3DARBPROC)	getProcAddress ( "glCompressedTexImage3DARB"	);
	glCompressedTexImage2DARB		= (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)	getProcAddress ( "glCompressedTexImage2DARB"	);
	glCompressedTexImage1DARB		= (PFNGLCOMPRESSEDTEXIMAGE1DARBPROC)	getProcAddress ( "glCompressedTexImage1DARB"	);
	glCompressedTexSubImage3DARB	= (PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC)	getProcAddress ( "glCompressedTexSubImage3DARB" );
	glCompressedTexSubImage2DARB	= (PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC)	getProcAddress ( "glCompressedTexSubImage2DARB" );
	glCompressedTexSubImage1DARB	= (PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC)	getProcAddress ( "glCompressedTexSubImage1DARB" );
	glGetCompressedTexImageARB		= (PFNGLGETCOMPRESSEDTEXIMAGEARBPROC)	getProcAddress ( "glGetCompressedTexImageARB"	);

									// ARB_point_parameters
	glPointParameterfARB			 = (PFNGLPOINTPARAMETERFARBPROC)		getProcAddress ( "glPointParameterfARB" );
	glPointParameterfvARB			= (PFNGLPOINTPARAMETERFVARBPROC)		getProcAddress ( "glPointParameterfvARB" );

									// ARB_occlusion_query
	glGenQueriesARB		= (PFNGLGENQUERIESARBPROC)		getProcAddress ( "glGenQueriesARB"		);
	glDeleteQueriesARB	 = (PFNGLDELETEQUERIESARBPROC)	getProcAddress ( "glDeleteQueriesARB"	 );
	glIsQueryARB			= (PFNGLISQUERYARBPROC)		getProcAddress ( "glIsQueryARB"			);
	glBeginQueryARB		= (PFNGLBEGINQUERYARBPROC)		getProcAddress ( "glBeginQueryARB"		);
	glEndQueryARB			= (PFNGLENDQUERYARBPROC)	getProcAddress ( "glEndQueryARB"			);
	glGetQueryivARB		= (PFNGLGETQUERYIVARBPROC)		getProcAddress ( "glGetQueryivARB"		);
	glGetQueryObjectivARB	= (PFNGLGETQUERYOBJECTIVARBPROC)getProcAddress ( "glGetQueryObjectivARB"	);
	glGetQueryObjectuivARB = (PFNGLGETQUERYOBJECTUIVARBPROC)getProcAddress ( "glGetQueryObjectuivARB" );

									// GLSL-specific extensions
									// GL_ARB_shader_object
	glDeleteObjectARB		= (PFNGLDELETEOBJECTARBPROC)		getProcAddress ( "glDeleteObjectARB" );
	glGetHandleARB			= (PFNGLGETHANDLEARBPROC)			getProcAddress ( "glGetHandleARB" );
	glDetachObjectARB		= (PFNGLDETACHOBJECTARBPROC)		getProcAddress ( "glDetachObjectARB" );
	glCreateShaderObjectARB	= (PFNGLCREATESHADEROBJECTARBPROC)	getProcAddress ( "glCreateShaderObjectARB" );
	glShaderSourceARB		= (PFNGLSHADERSOURCEARBPROC)		getProcAddress ( "glShaderSourceARB" );
	glCompileShaderARB		= (PFNGLCOMPILESHADERARBPROC)		getProcAddress ( "glCompileShaderARB" );
	glCreateProgramObjectARB= (PFNGLCREATEPROGRAMOBJECTARBPROC)	getProcAddress ( "glCreateProgramObjectARB" );
	glAttachObjectARB		= (PFNGLATTACHOBJECTARBPROC)		getProcAddress ( "glAttachObjectARB" );
	glLinkProgramARB		= (PFNGLLINKPROGRAMARBPROC)			getProcAddress ( "glLinkProgramARB" );
	glUseProgramObjectARB	= (PFNGLUSEPROGRAMOBJECTARBPROC)	getProcAddress ( "glUseProgramObjectARB" );
	glValidateProgramARB	= (PFNGLVALIDATEPROGRAMARBPROC)		getProcAddress ( "glValidateProgramARB" );
	glUniform1fARB			= (PFNGLUNIFORM1FARBPROC)			getProcAddress ( "glUniform1fARB" );
	glUniform2fARB			= (PFNGLUNIFORM2FARBPROC)			getProcAddress ( "glUniform2fARB" );
	glUniform3fARB			= (PFNGLUNIFORM3FARBPROC)			getProcAddress ( "glUniform3fARB" );
	glUniform4fARB			= (PFNGLUNIFORM4FARBPROC)			getProcAddress ( "glUniform4fARB" );
	glUniform1iARB			= (PFNGLUNIFORM1IARBPROC)			getProcAddress ( "glUniform1iARB" );
	glUniform2iARB			= (PFNGLUNIFORM2IARBPROC)			getProcAddress ( "glUniform2iARB" );
	glUniform3iARB			= (PFNGLUNIFORM3IARBPROC)			getProcAddress ( "glUniform3iARB" );
	glUniform4iARB			= (PFNGLUNIFORM4IARBPROC)			getProcAddress ( "glUniform4iARB" );
	glUniform1fvARB			= (PFNGLUNIFORM1FVARBPROC)			getProcAddress ( "glUniform1fvARB" );
	glUniform2fvARB			= (PFNGLUNIFORM2FVARBPROC)			getProcAddress ( "glUniform2fvARB" );
	glUniform3fvARB			= (PFNGLUNIFORM3FVARBPROC)			getProcAddress ( "glUniform3fvARB" );
	glUniform4fvARB			= (PFNGLUNIFORM4FVARBPROC)			getProcAddress ( "glUniform4fvARB" );
	glUniform1ivARB			= (PFNGLUNIFORM1IVARBPROC)			getProcAddress ( "glUniform1ivARB" );
	glUniform2ivARB			= (PFNGLUNIFORM2IVARBPROC)			getProcAddress ( "glUniform2ivARB" );
	glUniform3ivARB			= (PFNGLUNIFORM3IVARBPROC)			getProcAddress ( "glUniform3ivARB" );
	glUniform4ivARB			= (PFNGLUNIFORM4IVARBPROC)			getProcAddress ( "glUniform4ivARB" );
	glUniformMatrix2fvARB	= (PFNGLUNIFORMMATRIX2FVARBPROC)	getProcAddress ( "glUniformMatrix2fvARB" );
	glUniformMatrix3fvARB	= (PFNGLUNIFORMMATRIX3FVARBPROC)	getProcAddress ( "glUniformMatrix3fvARB" );
	glUniformMatrix4fvARB	= (PFNGLUNIFORMMATRIX4FVARBPROC)	getProcAddress ( "glUniformMatrix4fvARB" );
	glGetObjectParameterfvARB = (PFNGLGETOBJECTPARAMETERFVARBPROC) getProcAddress ( "glGetObjectParameterfvARB" );
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) getProcAddress ( "glGetObjectParameterivARB" );
	glGetInfoLogARB			= (PFNGLGETINFOLOGARBPROC)			getProcAddress ( "glGetInfoLogARB" );
	glGetAttachedObjectsARB	= (PFNGLGETATTACHEDOBJECTSARBPROC)	getProcAddress ( "glGetAttachedObjectsARB" );
	glGetUniformLocationARB	= (PFNGLGETUNIFORMLOCATIONARBPROC)	getProcAddress ( "glGetUniformLocationARB" );
	glGetActiveUniformARB	= (PFNGLGETACTIVEUNIFORMARBPROC)	getProcAddress ( "glGetActiveUniformARB" );
	glGetUniformfvARB		= (PFNGLGETUNIFORMFVARBPROC)		getProcAddress ( "glGetUniformfvARB" );
	glGetUniformivARB		= (PFNGLGETUNIFORMIVARBPROC)		getProcAddress ( "glGetUniformivARB" );
	glGetShaderSourceARB	= (PFNGLGETSHADERSOURCEARBPROC)		getProcAddress ( "glGetShaderSourceARB" );

										// GL_ARB_vertex_shader
	glBindAttribLocationARB	= (PFNGLBINDATTRIBLOCATIONARBPROC)	getProcAddress ( "glBindAttribLocationARB" );
	glGetActiveAttribARB	= (PFNGLGETACTIVEATTRIBARBPROC)		getProcAddress ( "glGetActiveAttribARB"	);
	glGetAttribLocationARB	= (PFNGLGETATTRIBLOCATIONARBPROC)	getProcAddress ( "glGetAttribLocationARB"	);
	glGetVertexAttribfvARB	= (PFNGLGETVERTEXATTRIBFVARBPROC)	getProcAddress ( "glGetVertexAttribfvARB"	);

										// EXT_stencil_two_side
	glActiveStencilFaceEXT	= (PFNGLACTIVESTENCILFACEEXTPROC)	getProcAddress ( "glActiveStencilFaceEXT" );

					// EXT_depth_bounds_test
	glDepthBoundsEXT		= (PFNGLDEPTHBOUNDSEXTPROC)			getProcAddress ( "glDepthBoundsEXT" );

#ifdef	_WIN32
	initWin32Extensions ();
#else
	initLinuxExtensions ();
#endif

	GL_EXTENSION_MULTITEXTURE = OpenGL_ExtensionsIsSupported("GL_ARB_multitexture");
	if(GL_EXTENSION_MULTITEXTURE)
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,	&GL_EXTENSION_MULTITEXTURE_TEXTURE_UNITS);

	GL_EXTENSION_VBO = OpenGL_ExtensionsIsSupported("GL_ARB_vertex_buffer_object");
	GL_EXTENSION_TEXTURE_CUBE_MAP = OpenGL_ExtensionsIsSupported("GL_EXT_texture_cube_map");
	GL_EXTENSION_DOT3 = OpenGL_ExtensionsIsSupported("GL_EXT_texture_env_dot3") && GL_EXTENSION_TEXTURE_CUBE_MAP;
	GL_EXTENSION_TEXTURE_NON2POWER = OpenGL_ExtensionsIsSupported("GL_ARB_texture_non_power_of_two");

	initialized = true;
}

void OpenGL_ExtensionsPrintfInfo()				// print info about card, driver, version & etc
{
	if(OpenGL_ExtensionsIsSupported("GL_ARB_multitexture"))
	{
		int maxTextureUnits;

		glGetIntegerv ( GL_MAX_TEXTURE_UNITS_ARB,	&maxTextureUnits );
		log_msg("opengl_info", "ARB_multitexture supported. Max texture units " + IntToStr(maxTextureUnits));
	}
	else
		log_msg("opengl_info", "No ARB_multitexture");

	if (OpenGL_ExtensionsIsSupported("GL_NV_register_combiners"))
	{
		int maxCombiners;

		glGetIntegerv(GL_MAX_GENERAL_COMBINERS_NV, &maxCombiners);
		log_msg("opengl_info", "NV_register_combiners supported. Max general combiners " + IntToStr(maxCombiners));
	}
	else
		log_msg("opengl_info", "No NV_register_combiners");

	if(OpenGL_ExtensionsIsSupported("GL_ARB_vertex_buffer_object"))
		log_msg("opengl_info", "VBO supported");
	else
		log_msg("opengl_info", "No VBO");

	if(OpenGL_ExtensionsIsSupported("GL_NV_occlusion_query"))
		log_msg("opengl_info", "NV_occlusion_query extension supported");

	if(OpenGL_ExtensionsIsSupported("GL_SGIS_generate_mipmap"))
		log_msg("opengl_info", "GL_SGIS_generate_mipmap extension supported");

	if(OpenGL_ExtensionsIsSupported("GL_ARB_vertex_program"))
		log_msg("opengl_info", "GL_ARB_vertex_program extension supported");

	const char *exts = (char*)glGetString(GL_EXTENSIONS);
	log_msg("graph opengl opengl_info", std::string("Supported GL extensions: ") + strtr(exts, ' ', '\n' ));
}
