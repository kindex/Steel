#ifndef ERRORSTREAM_H
#define ERRORSTREAM_H

#ifdef WIN32
	#define NOMINMAX
	#include <windows.h>
#endif

#include <common/logger.h>

class ErrorStream : public NxUserOutputStream
	{
	public:
	void reportError(NxErrorCode e, const char* message, const char* file, int line)
		{
		printf("%s (%d) :", file, line);
		switch (e)
			{
			case NXE_INVALID_PARAMETER:
				printf( "invalid parameter");
				break;
			case NXE_INVALID_OPERATION:
				printf( "invalid operation");
				break;
			case NXE_OUT_OF_MEMORY:
				printf( "out of memory");
				break;
			case NXE_DB_INFO:
				printf( "info");
				break;
			case NXE_DB_WARNING:
				printf( "warning");
				break;
			default:
				printf("unknown error");
			}

		printf(" : %s\n", message);
		}

	NxAssertResponse reportAssertViolation(const char* message, const char* file, int line)
		{
		printf("access violation : %s (%s line %d)\n", message, file, line);
#ifdef WIN32
		switch (MessageBox(0, message, "AssertViolation, see console for details.", MB_ABORTRETRYIGNORE))
			{
			case IDRETRY:
				return NX_AR_CONTINUE;
			case IDIGNORE:
				return NX_AR_IGNORE;
			case IDABORT:
			default:
				return NX_AR_BREAKPOINT;
			}
#elif LINUX
		assert(0);
#elif _XBOX
		return NX_AR_BREAKPOINT;
#elif __CELLOS_LV2__
		return NX_AR_BREAKPOINT;
#endif
		}

	void print(const char* message)
		{
		printf(message);
		}
	};

const char* getNxSDKCreateError(const NxSDKCreateError& errorCode) 
{
	switch(errorCode) 
	{
		case NXCE_NO_ERROR: return "NXCE_NO_ERROR";
		case NXCE_PHYSX_NOT_FOUND: return "NXCE_PHYSX_NOT_FOUND";
		case NXCE_WRONG_VERSION: return "NXCE_WRONG_VERSION";
		case NXCE_DESCRIPTOR_INVALID: return "NXCE_DESCRIPTOR_INVALID";
		case NXCE_CONNECTION_ERROR: return "NXCE_CONNECTION_ERROR";
		case NXCE_RESET_ERROR: return "NXCE_RESET_ERROR";
		case NXCE_IN_USE_ERROR: return "NXCE_IN_USE_ERROR";
		default: return "Unknown error";
	}
};


#endif