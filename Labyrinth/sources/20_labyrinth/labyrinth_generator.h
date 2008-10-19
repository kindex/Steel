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
	bool isUpBorder(int x, int y) const;
	int getMaxX() const;
	int getMaxY() const;
    void setSize(size_t x, size_t y);

	friend class LabyrinthGenerator;
    std::string serialize() const;
    void unserialize(int x, int y, const std::string& data);

private:
	struct node
	{
		bool rightborder;
        bool upborder;
	};

// first index - y, second - x

	std::vector<std::vector<node> > a;
};

Labyrinth generateLabyrinth(const size_t x, const size_t y, float k);

#endif
