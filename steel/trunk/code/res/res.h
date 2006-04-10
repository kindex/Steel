/*id*******************************************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        ������ ��� Division
    Description:
        ������ ��� ��������, �������� ������� �������� � �������� ��� ����. 
	Parts:
		res.cpp
		� ������������� �������� ������ ��� �������� � �������� ���� ����� ��������.
**************************************************************************************/

/*-----------------------------------------------------------------------

    Full Description:
        ������ ��� ��������, �������� � �������� ��� �������� ���������, 
		������ ���: image, model, audio.
		������� ����� ResCollection �������� ������� �� �������� ��������, 
		������������ ��, ��������� ������������ ��������.
		������� ������ Res ������������ ��� ��������  � �������� �������� �� �����.
		Res::res_kind ���������� ���� ��������.
		������ �������������:

Res* createBMP(string filename) {return new BMP(filename); }

	ResCollection res;
	res.registerClass(createBMP,	Res::image);

	res.add(Res::image, "box");

	res["Image"]->bitmap ... 

-----------------------------------------------------------------------*/


#ifndef __RES_H
#define __RES_H

#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "../common/types.h"
#include "../steel.h"
#include "../_cpp.h"

/*
Resourse input file stream
��� �������� �������� �� �������� ������� ������ ������������ ���� ����� ��� ������ �� ������
�� ���� ����������� �������� �� �����, ������ ��� ���������� �� �����.
*/
class rstream: public std::ifstream
{
public:
	rstream(std::string s, ios_base::openmode _Mode = std::ios::binary) 
	{ 
		open(s.c_str(), _Mode | std::ios::in); 
	}
	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};

// Forward declaration
class ResCollection;

/*
����� - �������� � ��������� ������ �������.
�������� - ��������������� ��� ������� �������. �������� - ��� ������� ���� ����� ���� ��������� ������� ��� ��������.

�� ����� ������ ����������� ������ ��� �������� �������� ��� ������� ���� (image, model).
� ��� ����������� unload, ������� ���������� ���������, ��������� ��� ��������.
�� ��� ����������� ����������, ������� �������������� ����� init.
init ��������� ������ ��� ���������� �� ���������� ��������������.

��������, �������� cubeMap ���� ��������� 6 �������, � ������ ����� ������� �� ��� ����. 
��� �������� ������ �� ���� ������� ������������ ����� image, ������� ����� ������� �� ������
������� (bmp, jpg, png).
*/
class Res: public steelAbstract
{
public:
// ���������� ����� ��������
#define RES_KIND_COUNT 5
// ���� �������� (���� ��������)
	typedef enum 
	{
			none,
			image,
			model,
			config,
			script
	}	res_kind;
// ��������� ��� ������������� ������� (���, ������)
	struct ResLocator
	{
		res_kind kind;
		std::string name;

		ResLocator(res_kind akind, std::string aname): kind(akind), name(aname) {}
	};

	typedef
		std::vector<ResLocator>
		ResLocatorArray;

//	virtual bool init(const std::string name, ResCollection &res) = 0;
	virtual bool unload() = 0;
};

// ���: ������� ��� ������������� ����� ������, ��������������� �� Res
typedef Res*(funcCreateResClass)(const std::string filename, ResCollection *res); 
// ����� ������. ����� ��� �������������� ������ � ����� �������.
struct ClassCopy
{
	ClassCopy(funcCreateResClass* _func){ func = _func;}

	funcCreateResClass* func;
};


/*
��������� ��������
������ ��������� �������� ��������� �����. 
������� �������������� �������, ������� ������������ ��� �������� (������������� �������).
� ������� ������� ������ ���� ���������� ��������� �������������, ������� �� �������� � ���� ���������� �����.
��� ������ ������ add, ��������� �������� ��������� ������ ����� ���������� ��������� ��� �������� ������� ����� ����.
��������, ��� �������� ������� ���� "image" ��������� ���������� BMP, JPG, PNG. ���� ���� ���� �� ��� ������ ��������� 
������ - �����������, ����� - ���.
*/
class ResCollection
{
// ������ � ��������, � ������� �������� �������
	std::vector<Res*> data;
// ���� ���� ��������
	std::vector<Res::res_kind> resType;
// Map: resource name->index
// �� ����� ���������� ������ � �������� data � resType
	std::map<const std::string,int> index; 
	std::vector<std::string> names;

	typedef 
		std::vector<ClassCopy> 
		ResClassArray;
// ������� ������� ��� �������� �������� ������� ����
	ResClassArray classes[RES_KIND_COUNT];
// ���������� ����������� �������� (� ������ 0)
	int freeindex;
public:
	ResCollection(): freeindex(0) {}
// ������� ������ �� ������
	Res* operator [] (const int n)        { return data[n]; }
// ���������, ���������� �� ������ � ��������� ������
	bool find(const std::string& name) {return index.find(name) != index.end(); } 
// ����� ����� ������� �� �����
    int getIndex(const std::string name)   {if(index.find(name) != index.end())	return index[name];	else return -1;  }  /*If exist - return*, esle 0 */
    
// ������� ������ �� �����
	Res* operator [] (const std::string& name) 
	{
		int i = getIndex(name);
		if(i<0)
			return NULL;
		else
			return data[i];
	}

// ������� ������ �� ����� � ����
	Res* get(const Res::res_kind kind, const std::string& name) 
	{ 
		int i = getIndex(name);
		if(i<0)
			return NULL;
		else
		{
			if(kind == resType[i])
				return data[i]; 
			else
				return NULL;
		}
	}

// ������� ������ (������) �� ����� 
// TODO: �� ������� - �����������
	Res* getModel(const std::string& name)
	{
		Res* m = add(Res::model, name);
		if(m != NULL)
			return m;
		else
		{
			alog.msg("error res model", "Model not found: "+name);
			return NULL;
		}
	}

// �������� ������ �� ����� � ����
	Res* addForce(const Res::res_kind kind, const std::string& name);
// �������� ������ �� ����� � ����, ���� ������� ��� ��� � ���������
	Res* add(const Res::res_kind kind, const std::string& name);
// �������� ������ ��������
	bool add(Res::ResLocatorArray &names);

// ������������� ��� ������� � ���������.
// _func - ������� ��� �������� ������ ��� �������� (��������������� �� Res)
	void registerClass(funcCreateResClass *_func, const Res::res_kind kind);
// ������� ����� ������
	Res* createClass(ClassCopy *aclass, std::string name);
};

#endif
