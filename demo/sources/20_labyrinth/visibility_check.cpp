/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Visiblility check
 ************************************************************/

#include "visibility_check.h"
#include <cmath>

Cell getCell(const v3 position,
             const v2 cell_size)
{
    return std::make_pair(int(floor(position.x/cell_size.x)),
                          int(floor(position.y/cell_size.y)));
}

CellVisibilitySet calculateCellVisibilitySet(const Labyrinth& labyrinth,
                                             const v3&        camera,
                                             const v2         cell_size)
{
    CellVisibilitySet result;

    result.insert(getCell(camera, cell_size));

    return result;
}