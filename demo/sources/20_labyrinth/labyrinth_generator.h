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
	bool isRightBorder(int x, int y) const;
	bool isDownBorder(int x, int y) const;
	int getMaxX() const;
	int getMaxY() const;

	friend class LabyrinthGenerator;

private:
	struct node
	{
		bool rightborder;
        bool downborder;
	};

	std::vector<std::vector<node> > a;
};

Labyrinth generateLabyrinth(const size_t x, const size_t y, float k);

#endif
