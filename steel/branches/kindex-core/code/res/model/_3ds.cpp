/*********************************************************************
(C) KINDEX, 2004
kindex@inbox.lv
http://kindex.times.lv

3DS file loader (only trimesh&materials)

20040228 Stated 3DS TriMesh OpenGL
20040301 Materials Colors DEPTH_TEST
20040501 Fix materials
Вывод: 3DMAX экспортирует не все данные о материале в .3DS.
Он не экспортирует Bump, Reflection, Self_Illum
Решение: дополнительные данные о материале хранить в файлах настроек. 
(res/materials/<mat_name>.mat)

Это делает процедура updatematerial()

**********************************************************************/
#pragma hdrstop

#include "_3ds.h"

using namespace std;

color_f string2color(string s)
{
    int first  = s.find(":");
    int second = s.find(":", first+1);

    string r = s.substr(0,first);
    string g = s.substr(first+1, second-first-1);
    string b = s.substr(second+1, s.size()-second-1);

    color_f res;
	res.r = (float)(atoi(r.c_str())/255.0);
    res.g = (float)(atoi(g.c_str())/255.0);
    res.b = (float)(atoi(b.c_str())/255.0);
    return res;
}

int readstring(rstream &f, string &res)
{
	res.clear();
	int reads = 0;
	char c;
	do
	{
		f.read (&c, 1);	reads++;
		if(c != '\0') res.push_back(c);
	}while (c != '\0' && f.good());

	return reads;
}

int loadcolor(ifstream &f, color_f &color)
{
    unsigned short _id;
    unsigned int _len, reads = 0;
    color_24 rgb;

    f.read((char*)&_id, 2); //Read the chunk header
    f.read((char*)&_len, 4); //Read the lenght of the chunk
    reads = 6;
    switch (_id)
    {
        case 0x0010 /*float red, grn, blu*/:
            f.read((char*)&color, sizeof(color));
            reads += sizeof(color);
            break;
        case 0x0011 /*char red, grn, blu; */:
            f.read((char*)&rgb, sizeof(rgb));
            color.r = (float)(rgb.r / 255.0f);
            color.g = (float)(rgb.g / 255.0f);
            color.b = (float)(rgb.b / 255.0f);
            reads += sizeof(rgb);
            break;
		default: f.seekg(_len-6, ios::cur); reads += _len - 6;  break;
    }
    return reads;
}

int loadpercentage(ifstream &f, float &p)
{
    unsigned short _id,p_s;
    unsigned int _len, reads = 0;

    f.read ((char*)&_id, 2); //Read the chunk header
    f.read ((char*)&_len, 4); //Read the lenght of the chunk

    reads = 6;
    switch (_id)
    {
        case 0x0031 /*float*/:
            f.read((char*)&p, 4);
            reads += sizeof(float);
            break;
        case 0x0030 /*short*/:
            f.read((char*)&p_s, 2);
            p = (float)p_s / 0xFFFF;
            reads += 2;
            break;
		default: f.seekg(_len-6, ios::cur); reads += _len - 6;  break;
    }
    return reads;
}


/*int _3DS::loadfacelist(istream &f)
{
	return 0;
    unsigned short _id;
    unsigned int _len, reads = 0;
    short a,b,c,faceinfo, cnt;

    f.read ((char*)&cnt, 2);  reads += 2;
    face.resize(cnt);
    index.resize(cnt*3);
    for (int i=0; i<cnt; i++)
    {
        fread (&a, 2, 1, file);
        fread (&b, 2, 1, file);
        fread (&c, 2, 1, file);
        fread (&faceinfo, 2, 1, file);
        m.index[i*3+0] = a;
        m.index[i*3+1] = b;
        m.index[i*3+2] = c;
        m.face[i].flags = faceinfo;
    }
    reads += cnt*2*4;

    while (reads<len)
    {
        if (fread (&_id, 2, 1, file)!=1) return reads; //Read the chunk header
        if (fread (&_len, 4, 1, file)!=1) return reads; //Read the lenght of the chunk
        reads += 6;
        switch (_id)
        {
        case TRI_MATERIAL:*/
      /*
MSH_MAT_GROUP mesh_material_group
cstr material_name;
short nfaces;
short facenum[nfaces];
      */
  /*          reads += readstring(file, m.MatName);

            fread (&cnt, 2, 1, file);  reads += 2;
            m.face.resize(cnt);
            for (int i=0; i<cnt; i++)
            {
                fread (&a, 2, 1, file);
                m.face[i].material = a;
            }
            reads += cnt*1*2;
            break;
        default: fseek(file, _len-6, SEEK_CUR); reads += _len - 6;  break;
        }
    }
    if (reads != len)
        throw;
    return reads;
}
*/

/*
int _3DS::loadmodel(rstream &f, int size)  // example: Box01
{
	unsigned short subChainId;
	int subChainSize, reads = 0;

	while (reads<size)
	{
		f.read((char*)&subChainId, 2);	if(!f.good())return reads; //Read the chunk header
		f.read((char*)&subChainSize, 4);if(!f.good())return reads; //Read the lenght of the chunk
		streampos sp = f.tellg();
		reads += 6;

		switch (subChainId)
        {
            case TRI_VERTEXL:
				unsigned short cnt;
                f.read ((char*)&cnt, 2);  reads += 2;
                vertex.resize(cnt);
                for (int i=0; i<cnt; i++)
                {
					float x,y,z;
                    f.read((char*)&x, 4);
                    f.read((char*)&y, 4);
                    f.read((char*)&z, 4);
                    vertex[i].x = y;
                    vertex[i].y = z;
                    vertex[i].z = x;
                }
                reads += cnt*4*3;
                break;

        }
		f.seekg(sp, ios::beg);
		f.seekg(subChainSize - 6, ios::cur); 
		reads += subChainSize - 6; 
    }

    return reads;
}
*/

// ************************ SCENE
int parsechain(_3DS &m, rstream &f, vector<chainProcessor> tags, int size = 0)
{
    unsigned short subChainId;
    int subChainSize, reads = 0;

	while (f.good() && (reads<size || size==0))
    {
		f.read((char*)&subChainId, 2);	if(!f.good())return reads; //Read the chunk header
		f.read((char*)&subChainSize, 4);if(!f.good())return reads; //Read the lenght of the chunk
		int creads = 0;
		creads += 6;

		for(vector<chainProcessor>::iterator it = tags.begin(); it != tags.end(); it++)
			if(it->tag == subChainId)
			{
				creads += it->f(m, f, subChainSize-6);
					break;
			}
		int add = subChainSize - creads;
		f.skip(add); 
		creads += add;

		reads += creads;
    }
    // (reads != len) throw; ERROR
    return reads;
}


int chain_triangles(_3DS &m, rstream &f, int size)
{
	int r = 0;
	unsigned short count;

	f.read((char*)&count, 2); r += 2;

	int x = sizeof(m.triangle[0]);

    m.triangle.resize(count);
	for(int i=0; i<count; i++)
	{
		f.read(&m.triangle[i], 3*2); // short*3 
		short face_flags;
		f.read(&face_flags, 2);
		r += 8;
	}

	return r;
}

/*
Vertices number (unsigned short)
Vertices list: x1,y1,z1,x2,y2,z2 etc. (for each vertex: 3*float)
*/
int chain_vertexes(_3DS &m, rstream &f, int size)
{
	unsigned short count;

	f.read(&count, 2);	
	if(count*12 +2!= size)
			throw;
	m.vertex.resize(count);
	f.read(&m.vertex[0], count*4*3); // count*3*float (x, y, z)

	return 2+count*3*4;
}

int chain_mesh(_3DS &m, rstream &f, int size)
{
	vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4110, chain_vertexes));
	t.push_back(chainProcessor(0x4120, chain_triangles));
	return parsechain(m, f, t, size);
}

int chain_4000(_3DS &m, rstream &f, int size)
{
	string name;
	int namesize = readstring(f, name);
	
	vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4100, chain_mesh));
	return parsechain(m, f, t, size - namesize) + namesize;
}


int chain_3d3d(_3DS &m, rstream &f, int size)
{
	vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4000, chain_4000));

	return parsechain(m, f, t, size);
}



int chain_4d4d(_3DS &m, rstream &f, int size)
{
	vector<chainProcessor> t;
	t.push_back(chainProcessor(0x3D3D, chain_3d3d));
	return parsechain(m, f, t, size);
}

bool _3DS::init(string &name)
{
	rstream f("../res/" + name);
	
	vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4D4D, chain_4d4d));

	parsechain(*this, f, t);

	return true;
}

