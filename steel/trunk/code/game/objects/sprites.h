#ifndef __GAME_SPRITES
#define __GAME_SPRITES

#include "../game_obj.h"

struct Sprite
{
	v3		pos;
	coord	size;
};

class Sprites: public GameObjDummy
{
	std::vector<Sprite> sprite;
	Material	*m;
	Vertexes	vertex;
	FaceMaterials	face;
	MapCoords	mapCoords;
	Normals		normal;
	v3			eye;
public:
	Sprites() {}

	Vertexes*	getVertexes() { return &vertex; }
	FaceMaterials* getFaceMaterials() { return &face; }
	Normals*	getNormals() { return &normal; }
	MapCoords*	getMapCoords() { return &mapCoords; }

	aabb getFrame();

	bool init(ScriptLine	&s, ResCollection &res);
//	bool cleanup()	{	return true;	}
	void processGraph(v3	cameraEye);
};

#endif