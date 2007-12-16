/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Generator.
		This is homework for OOP subject at LU master program 2006.
 ************************************************************/

#ifndef _LABYRINTH_GENERATOR_H_
#define _LABYRINTH_GENERATOR_H_

#include <vector>

class Labyrinth
{
public:
	bool rightborder(size_t x, size_t y);
	bool downborder(size_t x, size_t y);

	friend class LabyrinthGenerator;

private:
	struct node
	{
		bool rightborder, downborder;
	};

	std::vector<std::vector<node> > a;
};

Labyrinth generateLabyrinth(const size_t x, const size_t y);

#endif
