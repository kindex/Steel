#include "interface.h"

#include "common/logger.h"
#include "common/utils.h"

IdGenerator objectIdGenerator;
ModificationTime globalFrameNumber;

uid IdGenerator::genUid() 
{ 
	return freeId++;
}
