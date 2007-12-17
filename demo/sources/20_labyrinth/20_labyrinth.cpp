/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Game main unit
 ************************************************************/


#include "20_labyrinth.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>
#include <math/plane.h>

GameLabyrinth::GameLabyrinth()
{}

bool GameLabyrinth::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
	cameraMode = C_FIRST_PERSON;
	rightWallLength = _conf.getf("rightWallLength", 1.0f);
	downWallLength = _conf.getf("downWallLength", 1.0f);
	rightWall = _conf.find("rightWall");
	downWall = _conf.find("downWall");
	if (rightWall == NULL)
	{
        abort_init("error game res", "Cannot find right wall config");
		return false;
	}
	if (downWall == NULL)
	{
        abort_init("error game res", "Cannot find down wall config");
		return false;
	}

	labyrinth = generateLabyrinth(10, 10);

	for (int i = -1; i <= labyrinth.getMaxX(); i++)
	{
		for (int j = -1; j <= labyrinth.getMaxY(); j++)
		{
			bool right = labyrinth.isRightBorder(i, j);

			if (right && i < labyrinth.getMaxX())
			{
				rightWall->setValued("origin[0]", (i + 0.5f)*downWallLength);
				rightWall->setValued("origin[1]", j*rightWallLength);
				GameObject* wall = createGameObject(rightWall);
				world->addObject(wall);
			}

			bool down = labyrinth.isDownBorder(i, j);
			if (down && j < labyrinth.getMaxY())
			{
				downWall->setValued("origin[0]", i*downWallLength);
				downWall->setValued("origin[1]", (j + 0.5f)*rightWallLength);
				GameObject* wall = createGameObject(downWall);
				world->addObject(wall);
			}
		}
	}

	return true;
}

void GameLabyrinth::process()
{
	GameFreeScene::process();
}

void GameLabyrinth::handleEventKeyDown(const std::string& key)
{
	if (key == "return")
	{
		if (cameraMode == C_FIRST_PERSON)
		{
			cameraMode = C_FREE;
		}
		else
		{
			cameraMode = C_FIRST_PERSON;
		}
	}
    GameFreeScene::handleEventKeyDown(key);
}

std::string GameLabyrinth::getWindowCaption()
{
    return  "Labyrinth FPS " + graphTimer.getfps_s()
			+ " Batches: " + IntToStr(graphEngine->total.batchCount)
			+ " Faces: " + IntToStr(graphEngine->total.triangleCount);
}

void GameLabyrinth::draw(GraphEngine& graph)
{
	GameFreeScene::draw(graph);
}

v3 GameLabyrinth::calulateCameraCollision(const v3& oldPos, const v3& newPos)
{
	v3 result = newPos;

	if (cameraMode == C_FIRST_PERSON)
	{
//		result.z = oldPos.z;

		for (int i = 0; i < 3; i++)
		{
			GameObject* crossingObject = NULL;
			v3 crossingPosition;
			Plane crossingTriangle;
			Line ray(oldPos, result - oldPos);
			bool isCollision = graphEngine->findCollision(ray, 
															crossingObject, 
															crossingPosition,
															crossingTriangle);
			if (isCollision)
			{
				if (i < 2)
				{
					v3 normal = crossingTriangle.getNormal();
					float inWallDist = normal.dotProduct(crossingPosition-result);
					result = result + normal*(inWallDist + EPSILON);
				}
				else
				{
					result = crossingPosition;
				}
			}
			else
			{
				break;
			}
		}
	}

	return result;
}

