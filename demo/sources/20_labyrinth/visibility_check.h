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

#pragma once

#include "labyrinth_generator.h"
#include <engine/engine_types.h>
#include <math/vector3d.h>
#include <math/vector2d.h>
#include <set>

typedef std::set<std::pair<int, int> > CellVisibilitySet;
typedef std::pair<int, int> Cell;

Cell getCell(const v3 position,
             const v2 cell_size);

CellVisibilitySet calculateCellVisibilitySet(const Labyrinth& labyrinth,
                                             const v3&        camera,
                                             const v2         cell_size);
