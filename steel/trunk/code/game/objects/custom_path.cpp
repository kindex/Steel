#include "custom_path.h"

void CustomPath::process(steel::time curTime, steel::time frameLength, PhysicEngine *engine)
{
	v3 pos = getPosition();
	vel.translation = v3(-pos.y, pos.x, 0);
}
