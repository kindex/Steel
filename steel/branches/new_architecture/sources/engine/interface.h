/*id*********************************************************
	File: engine/interface.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		������� ���������, ������� ������ ������������� ��� ������
 ************************************************************/

#ifndef ENGINE__INTERFACE_H
#define ENGINE__INTERFACE_H

#include "../steel.h"
#include "../math/vector3d.h"

typedef int InterfaceId;

class GameObject;

class BaseInterface
{
public:
	virtual bool setCurrentObject(GameObject*) = 0;
};

class Camera
{
public:
    v3 up, eye, center;

public:
	Camera(void);

    void seteye(const v3 &EYE) { eye = EYE; }
    void setcenter(const v3 &CENTER) { center = CENTER; }
	void setup(const v3 &EYE, const v3 &DIR);
	v3	direction(void) { return (center-eye).getNormalized(); }
};

struct ProcessInfo
{
	steel::time curTime;
	steel::time frameLength;
	Camera *camera;
};

#endif
