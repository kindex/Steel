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
#include <stack>

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


class ResStack
{
protected:
	int level;
	std::stack<std::string> stack;

public:
	ResStack(): level(0) {}

	bool	push(std::string directory);
	bool	pop(void);
	int		getLevel(void);
	std::string top(void);
};

extern ResStack resStack;

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

//	virtual bool init(const std::string name, const std::string dir) = 0;
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
	// ������������� ��������� (model, image, ...)
	std::string id;
	bool lastCached;
// ������ � ��������, � ������� �������� �������
	steel::vector<T*> data;
// Map: resource name->index
// �� ����� ���������� ������ � �������� data � resType
	std::map<const std::string,int> index; 
	steel::vector<std::string> names;


	// ���: ������� ��� ������������� ����� ������, ��������������� �� Res
	typedef T*(funcCreateResClass)(const std::string filename, const std::string baseName); 

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
	void setId(std::string _id) { id = _id; }

	// ������� ������ �� ������
	inline T* operator[](const int n) const { return data[n]; }

	// ���������, ���������� �� ������ � ��������� ������
	inline bool find(const std::string& name) {return index.find(name) != index.end(); } 

	// ����� ����� ������� �� �����
    inline int getIndex(const std::string name);
    
	// ������� ������ �� �����
	inline T* operator[] (const std::string& name);

	// ������� ������ �� ����� � ����
	inline T* get(const std::string& name) { return operator[](name); }

/*
	�������� ������ �� ����� � ����
	name - ������������� �������. ������ ��� ��� ����� ��� ����������, 
	�� ����� ���� ����������� ������������ ��������.
	������� ��������� ����� ���������� ������������ �� �������.
*/
	T* addForce(std::string name, bool pop = true);

	// �������� ������ �� ����� � ����, ���� ������� ��� ��� � ���������
	T* add(const std::string name, bool pop = true);

	// �������� ������ ��������
	bool add(const Res::ResLocatorArray &names);

	// ������������� ��� ������� � ���������.
	// _func - ������� ��� �������� ������ ��� �������� (��������������� �� Res)
	void registerClass(funcCreateResClass *_func)	{		classes.push_back(ClassCopy(_func));	}

	// ������� ����� ������
	T* createClass(ClassCopy *aclass, std::string name, std::string baseName)	{		return aclass->func(name, baseName);	}

	void pop() { if(!lastCached) resStack.pop(); }
};

std::string getFullPath(std::string filename, std::string directory);

template<class T>
bool ResCollection<T>::add(const Res::ResLocatorArray &names)
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

template<class T>
T* ResCollection<T>::add(const std::string name, bool pop)
{
	if(getIndex(name) < 0)
	{
		lastCached = false;
		return addForce(name, pop);
	}
	else
	{
		lastCached = true;
		return get(name);
	}
}

template<class T>
T* ResCollection<T>::addForce(std::string name, bool pop)
{
	std::string baseDirectory = resStack.top();
	name = getFullPath(name, baseDirectory);

	splitPath(name, baseDirectory, name);
	resStack.push(baseDirectory);

	log_msg("res " + id, "Loading " + baseDirectory + "/" + name);

    int s = classes.size();
	for(int i = 0; i < s; i++)
	{
		T *obj = createClass(&classes[i], name, baseDirectory); // + baseDirectory
		if(obj == NULL) continue;
		
		index[name] = freeindex;
		data.resize(freeindex+1);
		names.resize(freeindex+1);
	    
		std::string fullResName = baseDirectory + "/" + name;
		index[fullResName] = freeindex;
		names[freeindex] = fullResName;

		data[freeindex] = obj;
	
		log_msg("res " + id, "OK " + baseDirectory + "/" + name);
		if(pop) resStack.pop();

		freeindex++;
		return data[freeindex-1];
	}
	log_msg("res " + id, "Failed " + baseDirectory + "/" + name);
	if(pop) resStack.pop();

	return NULL;
}

// ������� ������ �� �����
template<class T>
inline T* ResCollection<T>::operator[] (const std::string& name) 
{
	int i = getIndex(name);
	if(i<0)
		return NULL;
	else
		return data[i];
}

template<class T>
inline int ResCollection<T>::getIndex(const std::string name)   
{
	std::string realname = getFullPath(name, resStack.top());
	if(index.find(realname) != index.end())
		return index[realname];	
	else 
		return -1;
}  /*If exist - return*, esle 0 */


#endif
