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
Решение: дополнительные данные о материале хранить в файлах настроек. (res/materials/<mat_name>.mat)
Это делает процедура updatematerial()

**********************************************************************/
#pragma hdrstop

#include "_3ds.h"
#include <stdio.h>




#define MAIN3DS       0x4D4D
#define EDIT3DS       0x3D3D  // this is the start of the editor config
#define KEYF3DS       0xB000  // this is the start of the keyframer config
 //>------ sub defines of EDIT3DS

#define EDIT_MATERIAL 0xAFFF
#define EDIT_CONFIG1  0x0100
#define EDIT_CONFIG2  0x3E3D
#define EDIT_VIEW_P1  0x7012
#define EDIT_VIEW_P2  0x7011
#define EDIT_VIEW_P3  0x7020
#define EDIT_VIEW1    0x7001
#define EDIT_BACKGR   0x1200
#define EDIT_AMBIENT  0x2100

#define OBJ_TRIMESH   0x4100
#define OBJ_LIGHT     0x4600
#define OBJ_CAMERA    0x4700

//>------ sub defines of OBJ_TRIMESH
#define TRI_VERTEXL     0x4110
#define TRI_VERTEXOPT   0x4111
#define TRI_FACELIST    0x4120
#define TRI_MATERIAL    0x4130
#define TRI_VERTMAPCOORD  0x4140
#define TRI_SMOOTH      0x4150
#define TRI_LOCAL       0x4160
#define TRI_VISIBLE     0x4165
//>>------  these define the different color chunk types
#define COL_RGB  0x0010
#define COL_TRU  0x0011
#define COL_UNK  0x0013


typedef struct
{
	unsigned short tag;
	long size;
}    Chunk_hdr; 

color_f string2color(string s)
{
    int first  = s.find(":");
    int second = s.find(":", first+1);

    string r = s.substr(0,first);
    string g = s.substr(first+1, second-first-1);
    string b = s.substr(second+1, s.size()-second-1);

    color_f res;
	res.r = (float)(atoi(r.c_str())/255.0f);
    res.g = (float)(atoi(g.c_str())/255.0f);
    res.b = (float)(atoi(b.c_str())/255.0f);
    return res;
}

/*void updatematerial(string name) // TODO XML
{
    Material &m = ::material[name];

    string fullname("res/materials/"+name+".mat");

    FILE *f;
    if ((f = fopen(_C(fullname),"rt")) == NULL) return;

    char s[100]; // TODO
    while (fscanf(f,"%s\n",s)>0)
    {
        string a = s;
        int x = a.find("=");
        string param = a.substr(0,x);
        string value = a.substr(x+1,a.size()-x-1);
        if (param=="reflectionimage")
        {
            m.maps[REFLMAP].filename = value;
            m.maps[REFLMAP].image = image.loadCubeMap(value);
        }
        if (param=="bumpimage")
        {
            m.maps[BUMPMAP].filename = value;
            m.maps[BUMPMAP].image = image.loadBumpMap(value);
        }
        if (param=="self-illumination")
        {
            m.self_illum = string2color(value);
        }
        if (param=="self_illumination")
        {
            m.maps[SELFILLUMIN].filename = value;
            m.maps[SELFILLUMIN].image = image.add(value);
        }
    }
    fclose(f);
}
*/


int readstring(FILE *file, string &res)
{
#define MAXREADSTRINGLEN 100
    res = "";
    char a[MAXREADSTRINGLEN+1];
    int i, reads = 0;
    char c;
    do
    {   i = 0;
        do
        {
            fread (&c, 1, 1, file); reads++;
            a[i] = c;
            i++;
        }while (c != '\0' && i<MAXREADSTRINGLEN);

        if (i==MAXREADSTRINGLEN) a[i] = '\0';
        res += a;
    } while (c != '\0');
    return reads;
}

int loadcolor(FILE *file, color_f &color)
{
    unsigned short _id;
    unsigned int _len, reads = 0;
    color_24 rgb;

    if (fread (&_id, 2, 1, file)!=1) return 0; //Read the chunk header
    if (fread (&_len, 4, 1, file)!=1) return 0; //Read the lenght of the chunk
    reads = 6;
    switch (_id)
    {
        case 0x0010 /*float red, grn, blu*/:
            fread(&color, sizeof(color),1, file);
            reads += sizeof(color);
            break;
        case 0x0011 /*char red, grn, blu; */:
            fread(&rgb, sizeof(rgb),1, file);
            color.r = (float)rgb.r / 255.0f;
            color.g = (float)rgb.g / 255.0f;
            color.b = (float)rgb.b / 255.0f;
            reads += sizeof(rgb);
            break;
        default: fseek(file, _len-6, SEEK_CUR); reads += _len - 6;  break;
    }
    return reads;
}

int loadpercentage(FILE *file, float &p)
{
    unsigned short _id,p_s;
    unsigned int _len, reads = 0;

    if (fread (&_id, 2, 1, file)!=1) return 0; //Read the chunk header
    if (fread (&_len, 4, 1, file)!=1) return 0; //Read the lenght of the chunk
    reads = 6;
    switch (_id)
    {
        case 0x0031 /*float*/:
            fread(&p, sizeof(float),1, file);
            reads += sizeof(float);
            break;
        case 0x0030 /*short*/:
            fread(&p_s, sizeof(short),1, file);
            p = (float)p_s / 0xFFFF;
            reads += sizeof(short);
            break;
        default: fseek(file, _len-6, SEEK_CUR); reads += _len - 6;  break;
    }
    return reads;
}


int loadfacelist(FILE *file, unsigned int len, Model &m)
{
    unsigned short _id;
    unsigned int _len, reads = 0;
    short a,b,c,faceinfo, cnt;

    fread (&cnt, 2, 1, file);  reads += 2;
//    m.face.resize(cnt);
    m.triangle.resize(cnt);
    for (int i=0; i<cnt; i++)
    {
        fread (&a, 2, 1, file);
        fread (&b, 2, 1, file);
        fread (&c, 2, 1, file);
        fread (&faceinfo, 2, 1, file);
        m.triangle[i].a = a;
		m.triangle[i].b = b;
        m.triangle[i].c = c;
//        m.face[i].flags = faceinfo;
    }
    reads += cnt*2*4;

    while (reads<len)
    {
        if (fread (&_id, 2, 1, file)!=1) return reads; //Read the chunk header
        if (fread (&_len, 4, 1, file)!=1) return reads; //Read the lenght of the chunk
        reads += 6;
        switch (_id)
        {
        case TRI_MATERIAL:
      /*
MSH_MAT_GROUP mesh_material_group
cstr material_name;
short nfaces;
short facenum[nfaces];
      */
            reads += readstring(file, m.MatName);

            fread (&cnt, 2, 1, file);  reads += 2;
//            m.face.resize(cnt);
/*            for (int i=0; i<cnt; i++)
            {
                fread (&a, 2, 1, file);
                m.face[i].material = a;
            }*/
            reads += cnt*1*2;
            break;
        default: fseek(file, _len-6, SEEK_CUR); reads += _len - 6;  break;
        }
    }
    if (reads != len)
        throw;
    return reads;
}


// *********************** MODEL
/*int loadmodel(FILE *file,unsigned  int len, Model &m)  // example: Box01
{
    unsigned short _id, cnt;
    unsigned int _len, reads = 0;
    float x,y,z;

    string tmpname;

    reads += readstring(file, tmpname);
    if (model.exist(tmpname))
    {
        fseek(file, len-reads, SEEK_CUR);
        return len;
    }

	
    Model &m = model.addForce(tmpname);
    m.name = tmpname;
    
    while (reads<len)
    {
        if (fread (&_id,  2, 1, file)!=1) return reads; //Read the chunk header
        if (fread (&_len, 4, 1, file)!=1) return reads; //Read the lenght of the chunk
        reads += 6;
        switch (_id)
        {
            case OBJ_TRIMESH: break; // obrabativaem tutzhe, tak eto edinstvennoe, 4to est' v cepo4ke OBJECT
            case TRI_VERTEXL:
                fread (&cnt, 2, 1, file);  reads += 2;
                m.vertex.resize(cnt);
                for (int i=0; i<cnt; i++)
                {
// TODO
                    fread (&x, 4, 1, file);
                    fread (&y, 4, 1, file);
                    fread (&z, 4, 1, file);
                    m.vertex[i].x = y;
                    m.vertex[i].y = z;
                    m.vertex[i].z = x;
                }
                reads += cnt*4*3;
                break;

            case TRI_FACELIST: reads += loadfacelist(file, _len-6, m); break;

            case TRI_VERTMAPCOORD:
                fread (&cnt, 2, 1, file);  reads += 2;
                m.mapcoord.resize(cnt);
                for (int i=0; i<cnt; i++)
                {
                    fread (&x, 4, 1, file);
                    fread (&y, 4, 1, file);
                    m.mapcoord[i].x = x;
                    m.mapcoord[i].y = y;
                }
                reads += cnt*4*2;
                break;
            default: fseek(file, _len-6, SEEK_CUR); reads += _len - 6;  break;
        }
    }

    if (reads != len)
        throw;
    return reads;
}
*/
