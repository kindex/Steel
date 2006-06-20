/*id*******************************************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
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

	res.registerClass(createBMP,	Res::image);

	res.add(Res::image, "box");

	res["Image"]->bitmap ... 

-----------------------------------------------------------------------*/


#ifndef __RES_H
#define __RES_H

#include <map>
#include <string>
#include <fstream>

#include "../interface.h"

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
	rstream() {}
	rstream(std::string s, std::string ext = "", ios_base::openmode _Mode = std::ios::binary) 
	{ 
		open(s, ext, _Mode);
	}

	bool open(std::string s, std::string ext = "", ios_base::openmode _Mode = std::ios::binary);

	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};


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
class Res//: public virtual BufferedElement
{
public:
// ���������� ����� ��������
#define RES_KIND_COUNT 6
// ���� �������� (���� ��������)
	typedef enum 
	{
			none,
			image, // class Image: 2D, 3D, Cubemap (1x6 maps in one)
			model, // class Model
			config, // class Config
			script,
			material

	}	res_kind;
// ��������� ��� ������������� ������� (���, ������)
	struct ResLocator
	{
		res_kind kind;
		std::string name;

		ResLocator(res_kind akind, std::string aname): kind(akind), name(aname) {}
	};

	typedef
		steel::vector<ResLocator>
		ResLocatorArray;

//	virtual bool init(const std::string name, ResCollection &res) = 0;
	virtual bool unload() = 0;

	uid id;
	uid	getId() { return id; }
	void setId(uid _id) { id = _id; }
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
template<class T>
class ResCollection
{
protected:
// ������ � ��������, � ������� �������� �������
	steel::vector<T*> data;
// Map: resource name->index
// �� ����� ���������� ������ � �������� data � resType
	std::map<const std::string,int> index; 
	steel::vector<std::string> names;


	// ���: ������� ��� ������������� ����� ������, ��������������� �� Res
	typedef T*(funcCreateResClass)(const std::string filename); 

	// ����� ������. ����� ��� �������������� ������ � ����� �������.
	struct ClassCopy
	{
		funcCreateResClass* func;

		ClassCopy(): func(NULL) {}
		ClassCopy(funcCreateResClass* _func){ func = _func;}

	};

	
// ������� ������� ��� �������� �������� ������� ����
	steel::vector<ClassCopy> classes;
// ���������� ����������� �������� (� ������ 0)
	int freeindex;
public:
	ResCollection(): freeindex(0) {}
// ������� ������ �� ������

	T* operator[](const int n) const { return data[n]; }

// ���������, ���������� �� ������ � ��������� ������
	bool find(const std::string& name) {return index.find(name) != index.end(); } 
// ����� ����� ������� �� �����
    int getIndex(const std::string name)   {if(index.find(name) != index.end())	return index[name];	else return -1;  }  /*If exist - return*, esle 0 */
    
// ������� ������ �� �����
	T* operator [] (const std::string& name) 
	{
		int i = getIndex(name);
		if(i<0)
			return NULL;
		else
			return data[i];
	}

// ������� ������ �� ����� � ����
	T* get(const std::string& name) 
	{ 
		int i = getIndex(name);
		if(i<0)
			return NULL;
		else
			return data[i]; 
	}

/*
	�������� ������ �� ����� � ����
	name - ������������� �������. ������ ��� ��� ����� ��� ����������, 
	�� ����� ���� ����������� ������������ ��������.
	������� ��������� ����� ���������� ������������ �� �������.
*/
	T* addForce(const std::string name)
	{
        int s = classes.size();
		for(int i = 0; i < s; i++)
		{
			T *obj = createClass(&classes[i], name);
			if(obj == NULL) continue;
			
			index[name] = freeindex;
			data.resize(freeindex+1);
			names.resize(freeindex+1);
		    
			index[name] = freeindex; 
			names[freeindex] = name; 

			data[freeindex] = obj;
		
			log_msg("res", "loaded " + name);

			freeindex++;
			return data[freeindex-1];
		}
		log_msg("res", "failed " + name);
		return NULL;
	}
// �������� ������ �� ����� � ����, ���� ������� ��� ��� � ���������
	T* add(const std::string name)
	{
		if(index.find(name) == index.end())
		{
			return addForce(name);
		}
		else
			return get(name);
	}



// �������� ������ ��������
	bool add(const Res::ResLocatorArray &names)
	{
		for(Res::ResLocatorArray::iterator it = names.begin();	it != names.end();	it++)
    		{
    			if(!find(it->name))
    			{
    				if(!addForce(it->kind, it->name)) return false;
    			}
    		}
		return true;
	}


// ������������� ��� ������� � ���������.
// _func - ������� ��� �������� ������ ��� �������� (��������������� �� Res)
	void registerClass(funcCreateResClass *_func)
	{
		classes.push_back(ClassCopy(_func));
	}
// ������� ����� ������
	T* createClass(ClassCopy *aclass, std::string name)
	{
		return aclass->func(name);
	}
};



#endif
