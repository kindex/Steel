/****************************************************************
  TheGame 2D & 3D geometry unit (Steel engine)
  (C) KindeX (Andrey Ivanov), kindex@inbox.lv, http://kindex.lv
  Division, 2004-2005.

  Vse razmeti v igre v sisteme CI:
  kilogramm, metr, sekunda, m/s, m/ss, dzhoul', Njuton i t.d.
  ili v % (real 0..1)
******************************************************************/

#ifndef __GEOMETRY_H
#define __GEOMETRY_H
//#define _DEBUG

#include <vector>
#include <assert.h>

#include "maths.h"
#include "vector2d.h"
#include "vector3d.h"

// cos(a+b) = cos(a)*cos(b) - sin(a)*sin(b)
// sin(a+b) = sin(a)*cos(b) + cos(a)*sin(b)

struct aabb
{
    v3 min, max;
    void merge(v3 point);

	aabb() 
	{ 
		min.x = +INF; 
		min.y = +INF; 
		min.z = +INF; 
		max.x = -INF; 
		max.y = -INF;
		max.z = -INF;
	}
};

bool intersect(aabb const &first, aabb const &second);
aabb merge(aabb const &first, aabb const &second);

// otrezok
struct LineSegment
{
    v2 a,b; // a - koordinata odnogo konca, b - koordinata vtorogo

    LineSegment() {}
    LineSegment(v2 const &A, v2  const &B): a(A), b(B) {}
    LineSegment(LineSegment const &A) { a = A.a; b = A.b; }
};

//prjamaja
struct Line
{
    v2 a,b; // a - koordinata to4ki prjamoj, b - napravljajushaja
    Line() {}
    Line(v2  const &A, v2  const &B): a(A), b(B) {}
    Line(Line const &A) { a = A.a; b = A.b; }
};

struct DirectedLineSegment
{
    v2 s,t; // s - koordinata na4ala konca,
    // t - vektor napravlenija
};

// to4ka perese4enija otrezkov (a; a+b) i (c;c+d) v to4ke point
//bool cross(v2 &a, v2 &b,v2 &c, v2 &d, v2 &point)

// to4ka perese4enija otrezkov (a; a+b) i (c;c+d) v to4ke point
bool intersect(v2 &a, v2 &b,v2 &c, v2 &d, v2 &point);  // to4ka perese4enija otrezkov (a; a+b) i (c;c+d) v to4ke point

v2 mirror(v2 &v, v2 &a);
v3 vectmul(v3 &a, v3 &b);

struct polygon // Mnogougolnik
{ // koordinati vershin mnogougolnika
    std::vector<v2> p;
    int n; // p.size()

    polygon(): n(0) {}
    void add(const v2 &P) { p.push_back(P); n++;}
    bool isIn(const v2 A); // nahoditsja li to4ka vnutri vipuklog poligona?
    bool isIn2(const v2 A); // nahoditsja li to4ka vnutri 4etirehugolnika s samoperese4eniem
//    v2& operator [] (const int n)        { return p[n]; }
};

bool intersection_LineSegment_polygon(LineSegment &Line, polygon &P);

// rasstojanie ot to4ki do pramoj Line
coord distance(Line &line, v2 &point);
coord lineSecment_PointDistance(LineSegment line, v2 p);

// lezhit li prjamaja po odnu storonu ot poligona
// -1 - left, +1 Right, 0 - net razdelenija
int oneside(Line line, polygon p);

#endif

