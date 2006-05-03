#include "custom_path.h"

void CustomPath::process(steel::time curTime, steel::time frameLength, PhysicEngine *engine)
{
	velocity = v3((float)cos(curTime), (float)sin(curTime), 0);
}
