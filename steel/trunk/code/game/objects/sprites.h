/*id*********************************************************
    Unit: game/objects/sprites
    Part of: Steel engine
    Version: 1.0
    Authors:
        * def [Alexander Papyshev]
    Description:
		“ут содержатс€ классы дл€ рендеринга системы частиц
************************************************************/
#ifndef __GAME_SPRITES
#define __GAME_SPRITES

#include "../game_obj.h"

struct sprite_t
{
	v3		pos;
	coord	size;
};

namespace SpriteAlign
{
	typedef	enum
	{
		camera,
		z,
		screen,
		custom
	} SpriteAlign;
}

// Ётот класс отвечает за вывод одного спрайта через стандартный интерфейт steel engine
class SpriteSystem: public GameObj
{
protected:
	ResCollection	*res;

	std::vector<sprite_t> sprite;
	Material	*m;
	Vertexes	vertex;
	FaceMaterials	face;
	TexCoords	texCoords;
	Normals		normal;
	v3			eye;
	SpriteAlign::SpriteAlign	align;
	v3			customAlign;
public:
	SpriteSystem() {}

	Vertexes*	getVertexes() { return &vertex; }
	FaceMaterials* getFaceMaterials() { return &face; }
	Normals*	getNormals() { return &normal; }
	TexCoords*	getTexCoords(int texNumber) { return &texCoords; }

	aabb getFrame();
//	PositionKind	getPositionKind(){	return global;}

	void initSprites();
//	bool cleanup()	{	return true;	}
	void processGraph(v3	cameraEye, v3 cameraDirection);
};

// ќдин спрайт
class Sprite: public SpriteSystem
{
public:
	bool init(ScriptLine	&s, ResCollection &res);
	void setAlign(const v3 _align);
};

#endif

