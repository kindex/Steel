#ifndef ERRORSTREAM_H
#define ERRORSTREAM_H

#include "nxuseroutputstream.h"

class ErrorStream : public NxUserOutputStream
{
public:
	void reportError(NxErrorCode e, const char* message, const char* file, int line);
	NxAssertResponse reportAssertViolation(const char* message, const char* file, int line);
	void print(const char* message);
};

const char* getNxSDKCreateError(const NxSDKCreateError& errorCode);


#endif
