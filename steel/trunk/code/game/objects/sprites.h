#ifndef __GAME_SPRITES
#define __GAME_SPRITES

#include "../game_obj.h"

struct sprite_t
{
	v3		pos;
	coord	size;
};

class SpriteSystem: public GameObjDummy
{
protected:
	ResCollection	*res;

	std::vector<sprite_t> sprite;
	Material	*m;
	Vertexes	vertex;
	FaceMaterials	face;
	MapCoords	mapCoords;
	Normals		normal;
	v3			eye;
	bool	zedAlign;
public:
	SpriteSystem() {}

	Vertexes*	getVertexes() { return &vertex; }
	FaceMaterials* getFaceMaterials() { return &face; }
	Normals*	getNormals() { return &normal; }
	MapCoords*	getMapCoords() { return &mapCoords; }

	aabb getFrame();
//	PositionKind	getPositionKind(){	return global;}

	void initSprites();
//	bool cleanup()	{	return true;	}
	void processGraph(v3	cameraEye, v3 cameraDirection);
};

class Sprite: public SpriteSystem
{
public:
	bool init(ScriptLine	&s, ResCollection &res);
};

#endif

