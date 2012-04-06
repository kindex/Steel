#include "error_stream.h"
#ifdef WIN32
	#define NOMINMAX
	#include <windows.h>
#endif

#include <common/logger.h>
#include <common/utils.h>


void ErrorStream::reportError(NxErrorCode e, const char* message, const char* file, int line)
{
	std::string msg;
	switch (e)
	{
		case NXE_INVALID_PARAMETER:
			msg = "invalid parameter";
			break;
		case NXE_INVALID_OPERATION:
			msg = "invalid operation";
			break;
		case NXE_OUT_OF_MEMORY:
			msg = "out of memory";
			break;
		case NXE_DB_INFO:
			msg = "info";
			break;
		case NXE_DB_WARNING:
			msg = "warning";
			break;
		default:
			msg = "unknown error";
	}

    error("ageia", msg + ": " + message + " (file: " + file + "(" + IntToStr(line) + ")");
}

NxAssertResponse ErrorStream::reportAssertViolation(const char* message, const char* file, int line)
{
	error("ageia", std::string("access violation : ") + message + " : " + file + IntToStr(line));
#ifdef WIN32
	switch (MessageBox(0, message, "AssertViolation, see log for details.", MB_ABORTRETRYIGNORE))
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

void ErrorStream::print(const char* message)
{
	log_msg("ageia info", message);
}

const char* getNxSDKCreateError(const NxSDKCreateError& errorCode) 
{
	switch (errorCode) 
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
