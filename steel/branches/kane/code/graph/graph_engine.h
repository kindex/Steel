#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../engine.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/geometry.h"


/********************************* GRAPH **********************************************/



typedef vector<v3> vertexes;

// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned short a[3];
};

typedef vector<Triangle> triangles;

/*
Èíòåðôåéñ äëÿ âñåõ îáúåêòîâ â ïðîåêòå, êîòîðûå äîëæíû ðèñîâàòüñÿ ÷åðåç 
ñòðàíäàðòíûé ðåíäåðåð. Îáúåêòû ñîñòàâíûå. Ðîäèòåëü âîçâðàùàåò ñïèñîê
ïðèêðåïëžííûõ ê íåìó îáúåêòîâ. Â êàðêàñ (aabb) âõîäèò òàêæå è êàðêàñû ïîòîìêîâ.
Ýòî íóæíî äëÿ áûñòðîãî îòñå÷åíèÿ íåâèäèìûõ îáúåêòîâ.
Ãðàôè÷åñêèé èíòåðôåéñ, â îòëè÷àå îò ôèçè÷åñêîãî, òîëüêî ñ÷èòûâàåò èíôîðìàöèþ.
Ãðàôèêà ðèñóåò, ôèçèêà ìåíÿåò îáúåêòû.
Ïî èäåå, îáúåêò ìîæåò è íå áðàòü êîîðäèíàòû è ôîðìó èç ïàìÿòè, à ãåíåðèðîâàòü èõ íà õîäó,
íàïðèìåð, êàê ìåòà-øàðèêè, ñèñòåìà ÷àñòèö
*/

class GraphInterface;
typedef vector<GraphInterface*> GraphInterfaceList;

class GraphInterface: public Interface
{
public:
	virtual ~GraphInterface() {}
/*	ñïèñîê ñîñòàâíûõ ÷àñòåé îáúåêòà (ïîòîìêîâ). Íàïðèìåð, äëÿ ìèðà - ýòî ñòåíû è ìîíñòðû, à äëÿ ìîíñòðà ýòî ìîæåò áûòü ÷àñòàìè òåëà.*/
	virtual GraphInterfaceList getChildrens() = 0;

/*Ëþáàÿ òî÷êà âíóòðè îáúåêòà*/
	virtual v3 getPos() = 0; // return center of object, origin
/*Êàðêàñ - ïðÿìîóãîëüíèê, â êîòîðîì ñîäåðæèòñÿ îáúåêò. Ìîæåò áûòü áîëüøå, íî íå ìåíüøå ïðîñòðàíñòâà, çàíèìàåìûì îáõåêòîì. Äîëæåí âû÷èñëÿòüñÿ áûñòðî*/
	virtual aabb getFrame() = 0; // AABB of object
/*Ôîðìà îáúåêòÿ äëÿ ðèñîâàíèÿ (áåç ôîðìû ïîòîìêîâ)
 full shape for physics, graph (polygon, 3D mesh)
 quality = 0 - aabb
 quality = 1 - ñî âñåìè äåòàëÿìè
 Â çàâèñèìîñòè îò ðàññòîÿíèÿ è ïðîèçâîäèòåëüíîñòè ìàøèíû.
 */
	//virtual GShape getGShape(float quality=1) = 0; 
	/*
Vertex
Face
Material
Sprites ?
NURBS ?
Lights
Decals ?

Ïðîãàììíî ñãåíåðèðîâàííûå òåêñòóðû? no
Video textures (avi, camera)

*/
	// Èíòåðôåéñ çàòî÷åí ïîä 3DS è OpenGL
	// Ñëåäóþùèå ôóíêöèè âîçâðÿùÿþò ññûëêè íà ìàññèâû äàííûõ (NULL if none), è äîëæíû 
	// îòâå÷àòü çà õðàííèå ýòèõ äàííûõ äî ñëåäóþùåãî âûçîâà ýòîé ôóíêöèè
	// èëè âûçîâà cleanup
	virtual bool cleanup() = 0; // ïðè íåîáõîäèìîñòè îò÷èùàåò 
	virtual vertexes*	getVertexes() = 0; // ñïèñîê âåðøèí. 
	// ìàññèâ èíäåêñîâ âåðøèí, êîòîðûå îáðàçóþò òðåóãîëüíèêè (ãðàíè)
	virtual triangles*	getTriangles() = 0; 
	// ìàòåðèàë êàæäîé ãðàíè
//	virtual faces*	getFaces() = 0; // NULL if none

};


class GraphEngine: public Engine
{
protected:
	// data, to store collected information
	vertexes	vertex;
	triangles	triangle;
public:
	virtual ~GraphEngine() {}
	// Collect information about object: how to render it
	virtual bool inject(GraphInterface *object);
	// Draw colelcted information. May be called few times without recollection information
	bool process() = 0; // Override OpenGL, D3D, ...
	// Clear collected information
	bool clear()
	{
		vertex.clear();
		triangle.clear();
		return true;
	}
};




#endif
