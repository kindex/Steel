/*********************************************************************
(C) KINDEX, 2004
kindex@inbox.lv
http://kindex.times.lv

3DS file loader (only trimesh&materials)

20040228 Stated 3DS TriMesh OpenGL
20040301 Materials Colors DEPTH_TEST
20040501 Fix materials
�����: 3DMAX ������������ �� ��� ������ � ��������� � .3DS.
�� �� ������������ Bump, Reflection, Self_Illum
�������: �������������� ������ � ��������� ������� � ������ ��������. 
(res/materials/<mat_name>.mat)

��� ������ ��������� updatematerial()

**********************************************************************/

#include "_3ds.h"
#include "../res_main.h"
#include "../../common/utils.h"
using namespace std;


/*color_f string2color(string s)
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
*/

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

/*int loadcolor(ifstream &f, color_f &color)
{
    unsigned short _id;
    unsigned int _len, reads = 0;
    color_24 rgb;

    f.read((char*)&_id, 2); //Read the chunk header
    f.read((char*)&_len, 4); //Read the lenght of the chunk
    reads = 6;
    switch (_id)
    {
        case 0x0010: //float red, grn, blu
            f.read((char*)&color, sizeof(color));
            reads += sizeof(color);
            break;
        case 0x0011" //char red, grn, blu; 
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
*/
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

// ************************ SCENE
int parsechain(_3DS &m, rstream &f, steel::vector<chainProcessor> tags, int size = 0)
{
    unsigned short subChainId;
    int subChainSize, reads = 0;

	while (f.good() && (reads<size || size==0))
    {
		f.read((char*)&subChainId, 2);	if(!f.good())return reads; //Read the chunk header
		f.read((char*)&subChainSize, 4);if(!f.good())return reads; //Read the lenght of the chunk
		int creads = 0;
		creads += 6;

		for(steel::vector<chainProcessor>::iterator it = tags.begin(); it != tags.end(); it++)
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


int chain_model_material(_3DS &m, rstream &f, int size) // model material info
{
	int r = 0;
	string materialName;
	r += readstring(f, materialName);

	unsigned short count;
	f.read(&count, 2); // face count
	r += 2;

	int s = m.faceMaterial.size();
	m.faceMaterial.resize(s+1);
	m.faceMaterial[s].name = materialName;

	m.faceMaterial[s].triangles = new Triangles;
	Triangles &a = *m.faceMaterial[s].triangles;

	a.data.resize(count);

	for(int i=0; i<count; i++)
	{
		unsigned short tmp;
		f.read(&tmp, 2);

		a.data[i] = m.triangleAll.data[tmp];

		r += 2;
	}
	return r;
}

int chain_triangles(_3DS &m, rstream &f, int size)
{
	int r = 0;
	unsigned short count;

	f.read((char*)&count, 2); r += 2;

	int x = sizeof(m.triangleAll.data[0]);

    m.triangleAll.data.resize(count);
	for(int i=0; i<count; i++)
	{
		unsigned short a,b,c;
		f.read(&a, 2); // short
		f.read(&b, 2); // short
		f.read(&c, 2); // short

		m.triangleAll.data[i].a[0] = a;
		m.triangleAll.data[i].a[1] = b;
		m.triangleAll.data[i].a[2] = c;

		short face_flags;
		f.read(&face_flags, 2);
		r += 8;
	}

	steel::vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4130, chain_model_material)); // TRI_MATERIAL

	return r + parsechain(m, f, t, size-r);
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
	m.vertex.data.resize(count);
	f.read(&m.vertex.data[0], count*4*3); // count*3*float (x, y, z)

	return 2+count*3*4;
}

int chain_map_coords(_3DS &m, rstream &f, int size)
{
	unsigned short count;

	f.read(&count, 2);	

	m.texCoords.data.resize(count);

	f.read(&m.texCoords.data[0], count*4*2); // float+float (u, v)

	return 2+count*3*4;
}


int chain_mesh(_3DS &m, rstream &f, int size)
{
	steel::vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4110, chain_vertexes)); // coordinates
	t.push_back(chainProcessor(0x4120, chain_triangles)); // indexes

	t.push_back(chainProcessor(0x4140, chain_map_coords)); // indexes

	return parsechain(m, f, t, size);
}

int chain_4000(_3DS &m, rstream &f, int size)
{
	string name;
	int namesize = readstring(f, name);
	
	steel::vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4100, chain_mesh));
	return parsechain(m, f, t, size - namesize) + namesize;
}


int chain_3d3d(_3DS &m, rstream &f, int size)
{
	steel::vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4000, chain_4000));

	return parsechain(m, f, t, size);
}



int chain_4d4d(_3DS &m, rstream &f, int size)
{
	steel::vector<chainProcessor> t;
	t.push_back(chainProcessor(0x3D3D, chain_3d3d));
	return parsechain(m, f, t, size);
}

bool _3DS::init(const std::string name, const std::string dir)
{
	rstream f;
	
	if(!f.open(dir + "/" + name, "3ds"))
			return false;

	steel::vector<chainProcessor> t;
	t.push_back(chainProcessor(0x4D4D, chain_4d4d));

	parsechain(*this, f, t);

	// ������� ������������� �������, ��� ��� � 3DS ����� ��� ��������
	generateNormals();
	updateAABB();


	vertex.setId(objectIdGenerator.genUid());
	normal.setId(objectIdGenerator.genUid());
	triangleAll.setId(objectIdGenerator.genUid());
	texCoords.setId(objectIdGenerator.genUid());

	for(steel::vector<FaceMaterial>::iterator it = faceMaterial.begin();
				it != faceMaterial.end(); it++)
	{
		it->triangles->setId(objectIdGenerator.genUid());

		string mat = it->name;
		
		it->material = resMaterial.add(mat);
	}

	return true;
}