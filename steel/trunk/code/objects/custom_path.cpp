#include "custom_path.h"

void CustomPath::ProcessPhysic(steel::time curTime, steel::time frameLength, PhysicEngine *engine)
{
	v3 pos = getPosition().getVector();
	vel.translation = v3(-pos.y, pos.x, 0);
}
