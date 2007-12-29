/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Steel + NX lib
 ************************************************************/
#ifndef _STEEL_NX_H_
#define _STEEL_NX_H_

static inline NxVec3 tonx(const v3& v)
{
    return NxVec3(v.x, v.y, v.z);
}

static inline v3 tov3(const NxVec3& v)
{
    return v3(v.x, v.y, v.z);
}

static inline matrix34 to_matrix34(const NxMat34& m)
{
    matrix34 result;
    for (int i = 0; i < 3; i++)
    {
        result.setRow(i, tov3(m.M.getRow(i)));
    }
    result.setTranslation(tov3(m.t));
    return result;
}

static inline NxMat34 tonx(const matrix34& m)
{
    NxMat34 result;
    for (int i = 0; i < 3; i++)
    {
        result.M.setRow(i, tonx(m.getRow(i)));
    }
    result.t = tonx(m.getTranslation());
    return result;
}

#endif
