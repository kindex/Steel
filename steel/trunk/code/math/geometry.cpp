/****************************************************************
  TheGame 2D & 3D geometry unit (Steel engine)
  (C) KindeX (Andrey Ivanov), kindex@inbox.lv, http://kindex.lv
  Division, 2004-2005.

  Vse razmeti v igre v sisteme CI:
  kilogramm, metr, sekunda, m/s, m/ss, dzhoul', Njuton i t.d.
  ili v % (coord 0..1)
******************************************************************/

#include "geometry.h"
#include "maths.h"

#include <vector>
using namespace std;

bool intersect(coord amin, coord amax, coord bmin, coord bmax)
{
    return
        (amin<=bmin+EPSILON) && (bmin<=amax+EPSILON)
    ||
        (amin<=bmax+EPSILON) && (bmax<=amax+EPSILON)
    ||
        (bmin<=amin+EPSILON) && (amin<=bmax+EPSILON)
    ||
        (bmin<=amax+EPSILON) && (amax<=bmax+EPSILON)
        ;
}

bool aabb::intersect(aabb const &second)
{
    return
		::intersect(min.x, max.x, second.min.x, second.max.x)
    &&
	::intersect(min.y, max.y, second.min.y, second.max.y)
	&&
	::intersect(min.z, max.z, second.min.z, second.max.z);
}

void aabb::mul(const matrix44 &matrix)
{
	vector<v3> v = getVertexes();
	
	clear();
	for(int i=0; i<8; i++)
		merge(matrix*v[i]);
}


void aabb::merge(const v3 point)
{
    if (min.x > point.x) min.x = point.x;
    if (min.y > point.y) min.y = point.y;
    if (min.z > point.z) min.z = point.z;

    if (max.x < point.x) max.x = point.x;
    if (max.y < point.y) max.y = point.y;
    if (max.z < point.z) max.z = point.z;
}

void aabb::merge(aabb const &second)
{
	if (min.x > second.min.x) min.x = second.min.x;
	if (min.y > second.min.y) min.y = second.min.y;
	if (min.z > second.min.z) min.z = second.min.z;

	if (max.x < second.max.x) max.x = second.max.x;
	if (max.y < second.max.y) max.y = second.max.y;
	if (max.z < second.max.z) max.z = second.max.z;
 }



// prinadlezhit li to4ka c otrezku (a,a+b)
// esli izvestno 4to oni lezhat na odnoj prjamoj
bool intersect(v2  const &a, v2 const &bb,v2 const &c)
{
    v2 b = a + bb;
    return
      ((a.x<=c.x+EPSILON)&&(c.x<=b.x+EPSILON)
      ||
      (a.x+EPSILON>=c.x)&&(c.x+EPSILON>=b.x))
      &&
      ((a.y<=c.y+EPSILON)&&(c.y<=b.y+EPSILON)
      ||
      (a.y+EPSILON>=c.y)&&(c.y+EPSILON>=b.y));
}

// to4ka perese4enija otrezkov (a; a+b) i (c;c+d) v to4ke point
bool intersect(v2   &a, v2   &b,v2   &c, v2   &d, v2 &point)  // to4ka perese4enija otrezkov (a; a+b) i (c;c+d) v to4ke point
{
    coord db = d.pseudoscalarProduct(b);
    if (db!=0)
    {
        coord s = (a-c).pseudoscalarProduct(b)/db;
        if (s<-EPSILON || s>1+EPSILON) return false;
        coord t = (c-a+d*s).dotProduct(b)/*/b.GetSquaredLength()*/;
        if (t<-EPSILON || t>b.getSquaredLength()+EPSILON) return false;
        point = c+d*s;
        return true;
    }
  else // parallelno
  {
    if (fabs(b.pseudoscalarProduct(c - a)) < EPSILON) // na odnoj linii
    {
        if (intersect(a,b,c)) { point = c; return true; }
        if (intersect(a,b,c+d)) { point = c+d; return true; }
        if (intersect(c,d,a)) { point = a; return true; }
        if (intersect(c,d,a+b)) { point = a+b; return true; }
    }
  }
  return false;
}


// BUG 
template<typename T> T sqr(T x) 
{
	return x*x;
}


template<typename T>int sgn(T A) 
{ 
	return (A>EPSILON)-(A<-EPSILON);
}


// otra#ennij vector v ot a
v2 mirror(v2 &v, v2 &a)
{ // V - na6 vektor, A - ot 4ego otra#aem
  coord d;
//  vt:=(V*a/a^2)*;
  d=(v.x*a.x+v.y*a.y)/(sqr(a.x)+sqr(a.y));
  return v2(v.x-2*a.x*d , v.y-2*a.y*d);
};

//  | A B |
//  | C D |
coord det2(coord a, coord  b, coord c, coord d)
{
  return (a*d - b*c); // Determinant
}

v3 vectmul(v3 &a, v3 &b)
{
// | A.Y  A.Z |   | A.Z  A.X |    |  A.X  A.Y  |
// | B.Y  B.Z | , | B.Z  B.X | ,  |  B.X  B.Y  |
   return v3(
      det2(a.y, a.z, b.y, b.z),
      det2(a.z, a.x, b.z, b.x),
      det2(a.x, a.y, b.x, b.y)
   );
}

//coord abs(coord a) { return (a>=0)?a:-a; }


bool intersection_LineSegment_polygon(LineSegment &Line, polygon &P)
{
    if (P.isIn(Line.a) || P.isIn(Line.b)) return true;
    v2 point;
    //bool intersect(v2 &a, v2 &b,v2 &c, v2 &d, v2 &point)
    // to4ka perese4enija otrezkov (a; a+b) i (c;c+d) v to4ke point
/*    for (unsigned int i=0; i<P.p.size(); i++)
        if (intersect(
            Line.a, Line.b-Line.a,
            P.p[i], P.p[(i+1)%P.p.size()]-P.p[i], point)) return true;
*/
    return false;
}

bool polygon::isIn(v2 A)// ona vnutri vseh uglov
{
    int all = 0;
    for(int i=0; i<n; i += 1)
    {
        if (p[i] == A ||
            fabs((p[(i+1)%n]-p[i]).pseudoscalarProduct(A-p[i]))<EPSILON &&
            intersect(p[i],p[(i+1)%n]-p[i], A)) return true;
        v2 first = p[i] - p[(i+1)%n];
        v2 second = A - p[i];
        int z = sgn(first.pseudoscalarProduct(second));
        if (all==0)
            all = z;
        else
            if (all != z) return false;
    }
    return true;
}

bool polygon::isIn2(const v2 A) // nahoditsja li to4ka vnutri 4etirehugolnika s samoperese4eniem
{
    bool left[4], Aleft[4], all = true;
    for(int i=0; i<4; i++)
    {
        v2 base = p[i] - p[(i-1+n)%n];
        left[i] = base.pseudoscalarProduct(p[(i+1)%n]-p[(i)%n])>0;
        Aleft[i] = (p[(i+1)%n]-p[(i)%n]).pseudoscalarProduct(A-p[i])>0;
        all &= left[i] == left[0];
        if (p[i] == A ||
            fabs((p[(i+1)%n]-p[i]).pseudoscalarProduct(A-p[i]))<EPSILON &&
            intersect(p[i],p[(i+1)%n]-p[i], A)) return true;
    }
    if (all) // vipuklij mnogoug
        return isIn(A);
    else
    {
        for(int i=0; i<4; i++)
        {
            if (left[i] == left[(i+1)%n])
            {
                if (left[i] == Aleft[(i+n-1)%n] && left[i] == Aleft[i] &&
                left[i] == Aleft[(i+1)%n])
                    return true;

            }
        }
    }
    return false;
}

coord distance(Line &line, v2 &point)
{
    v2 diag = point - line.a;
    v2 lineDirection = line.b.getNormalized();
    coord proj = lineDirection.dotProduct(diag);
	coord s = diag.getSquaredLength() - sqr(proj);
    if (s<0 && s>-EPSILON)
      s = 0;
    //if (s<0)      assert(s >= 0);
    coord h = sqrt(s);
    return h;
}

coord lineSecment_PointDistance(LineSegment line, v2 p)
{
// libo proekcija, libo konec otrezka
    v2 linedirection = (line.b-line.a).getNormalized();
    Line l;
    l.a = line.a;
    l.b = linedirection;
    coord proj = (line.a-p).dotProduct(l.b);
    if (intersect(line.a, line.b, l.a + l.b*proj))
    // popadaem v otrezok - minimalnie rasstojanie - projekcia
        return distance(l, p);
    else
    {
        coord d1 = (line.a-p).getLength();
        coord d2 = (line.b-p).getLength();
        if (d1<d2) return d1; else return d2;
    }
}

int oneside(Line line, polygon p)
{
    int res = 0; 
    for(int i=0; i<p.n; i++)
    {
        int m = sgn(line.b.pseudoscalarProduct(p.p[i]-line.a));
        if (res == 0)
            res = m;
        else
            if (m !=0 && res != m) return 0;
    }
    return res;
}


struct Plane
{
    v3 point, a,b; // to4ka i dve napravljajushie
};

// Plane vs line
bool intersect(Plane plane, v3 e/*point*/, v3 c/*direction*/, coord &p /*result*/)
{
    v3 a = plane.a;
    v3 b = plane.b;
    e -= plane.point;
    v3 bxa = b.vectorProduct(a);
    v3 cxa = c.vectorProduct(a);
    v3 exa = e.vectorProduct(a);
    v3 cxaxbxa = cxa.vectorProduct(bxa);
//    float p !;
    coord len = cxaxbxa.getSquaredLength();
    if (len < EPSILON) return false;
    p = -(exa.vectorProduct(bxa).dotProduct(cxaxbxa)/len);
    return true;
}

// triangle vs line
bool intersect(/*triang*/v3 A, v3 B, v3 C, v3 e/*point*/, v3 c/*direction*/, coord &p)
{
    Plane plane;
    plane.point = A;
    plane.a = B-A;
    plane.b = C-A;
    if (!intersect(plane, e, c, p)) return false;
    v3 point = e + c*p;
    coord S1 = fabs(plane.a.vectorProduct(plane.b).getLength());
    coord S2 =
        fabs((A-point).vectorProduct(B-point).getLength())+
        fabs((B-point).vectorProduct(C-point).getLength())+
        fabs((C-point).vectorProduct(A-point).getLength());
    return fabs(S2-S1)<EPSILON;
}


// line vs all objects
/*bool intersect(Model &m, v3 source, v3 direction, coord &minp, int &face)
{
    bool found = false;
    int n = m.face.size();
    for(int i=0; i<n; i++)
    {
        v3 A = m.vertex[m.index[i*3]];
        v3 B = m.vertex[m.index[i*3+1]];
        v3 C = m.vertex[m.index[i*3+2]];

        v3 v;
        coord p;
        if(intersect(A,B,C, source, direction, p) && p>EPSILON)
            if (!found || p<minp)
            {
                found = true;
                minp = p;
                face = i;
            }
    }
    return found;
}
*/

//bool intersect(v3 source, v3 direction, coord &minp, int &m /*model*/, int &face)
/*{
    bool found = false;
    for(int i=0; i<model.freeindex; i++)
    {
        coord p;
        int cface;
        if (intersect(model[i], source, direction, p, cface))
        {
            if (!found || p<minp)
            {
                found = true;
                minp = p;
                m = i;
                face = cface;
            }
        }
    }
    return found;
}
*/

/*void raytrace(v3 source, v3 direction, coord e)
{
    total += e;
    direction.Normalize();
    int prevmat = -1;
    for(int t=0; t<100; t++)
    {
        coord p;
        int modelid, f;
        bool b = intersect(source, direction, p, modelid, f);
        v3 point = source + direction*p;
        if (!b)
            break;

        Model &m = model[modelid];

        v3 A = m.vertex[m.index[f*3+0]]; // face first vertex
        v3 B = m.vertex[m.index[f*3+1]]; // face first vertex
        v3 C = m.vertex[m.index[f*3+2]]; // face first vertex

        v3 S = B-A; // pervij vektor ploskosti
        v3 T = C-A; // vtoroj vektor ploskosti


        v3 normal = S.vectorProduct(T).GetNormalized(); // face normal, 4tobi uznat', lu4 vihodit iz tela ili vhodit

        bool in = normal.dotProduct(direction)<0; // vhodim vnutr'

/*      reflection
        coord cosalfa = fabs(normal.dotProduct(direction));
        coord sinalfa = sqrt(1-sqr(cosalfa)); // sin^2 + cos^2 =1
        coord k1 = 1.3, k2 = 1; // TODO koef prelomlenija
        coord sinbetta = sinalfa*(k2/k1);
        if (sinbetta<0)
        {
            sinbetta = -sinbetta;
            normal = -normal;
        }
        if (sinbetta>1 || sinbetta<-1) break; // polnoe otrazhenie
        coord cosbetta = sqrt(1-sqr(sinbetta)); // sin^2 + cos^2 =1

        // znaem dlinu orazhennogo vektora, znaem ugol -> znaem projeckii
        // iz projekcij sobiraem vektor
        v3 Sp = sinbetta*S;
        v3 Tp = sinbetta*T;
        v3 normalp = cosbetta*normal;
        direction = Sp + Tp + normalp;*/

/*        v3 D = source + direction*(p+EPSILON);//     (A+B+C)/3; // face center
        glBegin(GL_LINES);
        glVertex3f(D.x,D.y,D.z);
        glVertex3f(D.x+normal.x, D.y+normal.y, D.z+normal.z);
        glEnd();
*/


/*            glColor3f(e,e,e);
            glBegin(GL_LINES);
            glVertex3f(source.x, source.y, source.z);
            glVertex3f(point.x, point.y, point.z);
            glEnd();
*/
        // change direction - prelomlenie

/*        coord dist = (source-point).GetLength();

        int curmat;
        if (in)
            curmat = prevmat;
        else
            curmat = modelid;

        coord mju; // pronicaemost'*kg/m
        if (curmat>=0)
        {
            curmat = model[curmat].mat;
            mju = material[curmat].maps[DIFFUSEMAP].color.r;
        }
        else
            mju = 0; // vaakum

        coord laste = e;
        e = e*exp(-mju*dist);
        absorbed += laste-e;

        if (m.MatName == "screen") // poplai na ekran
        {
            image["screen"].putpixeladd(point.x+0.5, point.y+0.5, -e, -e, -e);
            break;
        }

        source = point + 2*EPSILON*direction;
        prevmat = modelid;
    }
}
*/

std::vector<v3> aabb::getVertexes()
{
	std::vector<v3> r;
	v3 &a = min; 
	v3 &b = max;
	
	r.push_back(v3(a.x, a.y, a.z));
	r.push_back(v3(a.x, a.y, b.z));
	r.push_back(v3(a.x, b.y, a.z));
	r.push_back(v3(a.x, b.y, b.z));
	r.push_back(v3(b.x, a.y, a.z));
	r.push_back(v3(b.x, a.y, b.z));
	r.push_back(v3(b.x, b.y, a.z));
	r.push_back(v3(b.x, b.y, b.z));

	return r;
}
