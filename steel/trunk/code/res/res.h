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

#ifndef __RES_H
#define __RES_H

#include "../interface.h"
#include "../common/types.h"
#include "../steel.h"
#include "../_cpp.h"

#include <map>
#include <string>
#include <stack>

// �������� ������ ��� ����� ������������ ����������. ���� ��� ����� ���������� � /, �� ��� ����� ��������� ��� ������
std::string getFullPath(std::string filename, std::string directory);

// ���� ������� ����������. ������������ ������ ���������� ��������.
class ResStack
{
protected:
	int level;
	std::stack<std::string> stack;

public:
	bool	push(std::string directory);
	bool	pushFullPath(std::string path)
	{
		std::string baseDirectory;		
		splitPath(path, baseDirectory, path);
		return push(baseDirectory);
	}

	bool	pop(void);
	int		getLevel(void);
	std::string top(void);

	std::string getFullName(const std::string name)
	{
		return getFullPath(name, top());
	}

	template<class T>
	friend class ResCollection;

	ResStack(): level(0) {}
};
extern ResStack resStack;

/*
	����� - �������� � ��������� ������ �������.
	�������� - ��������������� ��� ������� ���� �������. �������� - ��� ������� ���� ����� ���� ��������� ������� ��� ��������.

	�� ����� ������ ����������� ������ ��� �������� �������� ��� ������� ���� (image, model).
	� ��� ����������� ����������, ������� ���������� ���������, ��������� ��� ��������.
	�� ��� ����������� ����������, ������� �������������� ����� init.
	init ��������� ������ ��� ���������� �� ���������� ��������������.
*/
class Res//: public virtual BufferedElement
{
protected:
	uid resId; // ���������� �������������
public:
//	virtual bool init(const std::string name, const std::string dir) = 0;

	uid	getId() { return resId; }
	void setId(uid id) { resId = id; }
};


/*
��������� ��������
������ ��������� �������� ��������� �����. 
������� �������������� �������, ������� ������������ ��� �������� (������������� �������).
� ������� ������� ������ ���� ���������� ��������� �������������, ������� �� �������� � ���� ���������� �����.
��� ������ ������ add, ��������� �������� ��������� ������ ����� ���������� ��������� ��� �������� ������� ����� ����.
��������, ��� �������� ������� ���� "image" ��������� ���������� BMP, JPG, PNG. ���� ���� ���� �� ��� ������ ��������� 
������ - �� �� ����������, ����� - ���.
*/
template<class T>
class ResCollection
{
public:
	ResCollection() {}

	inline T* operator[] (const std::string& name);// ������� ������ �� ������� �����
	inline T* get(const std::string& name) { return operator[](name); }  // ������� ������ �� ������� �����

	// �������� ������ �� ����� � ����, ���� ������� ��� ��� � ���������
	T* add(const std::string name, bool pop = true);

	// ������� ������
	bool remove(const std::string name);
	bool remove(T* object);

	// � ��������� add ���� ������ �������� ���� bool. ���� �� ��������� false, �� ����� �������� ������� ������� ���������� �� ����������������� � ���� ��� ������ ������� � ������� ������ pop.
	void pop() { resStack.pop(); }


protected:
	std::string id; // ��������� ������������� ��������� (image, model)
	std::map<const std::string,int> index; // ����������� ������ ��� �������� �� ������ � ������� data. ��� ���������� �������� ������ �� �����.
	std::map<T*,int> resIndex; // ����������� ��������� ������� �� ������ � ������� data. ��� ���������� �������� �������� ������� �� ������ �� ����.

	struct ResStorage
	{
		T*		object;		// ������ �� ������
		uid		id;			// ���������� ������������� �������
		int		links;		// ���������� ������ �� ���� ������. ��� ���������� ������������ ������� ������� ������������� �� 1. ��� �������� -  ����������� �� 1. ���� ���������� ������ ���������� ������ 1, �� ������ ��������� �� ���������. (���������� Linux ext2fs)
		std::string name;	// ������ ��� �������
	};
	steel::vector<ResStorage> data; // ������ �������� �������� � �������������� ���������� � ������� �������.

	// ���: ������� ��� ������������� ����� ������, ��������������� �� Res
	typedef T*(funcCreateResClass)(const std::string filename, const std::string baseName); 

// ������� ������� ��� �������� �������� ������� ����
	steel::vector<funcCreateResClass*> classes;

	// *********** Functions ***************
	bool removeRaw(int index); // ������� ������ �� ������. ���� ���������� ������ = 0, �� ������ ��������� ��������� �� ���������
	bool purge(int delIndex); // ��������� ������� ������ �� ���������
	void setId(std::string _id) { id = _id; } // ������������� ������������� ��������

	// ��������� ������� ��� �������� ������, ������� ����� ��������� ������
	void registerResLoader(funcCreateResClass *_func)	{		classes.push_back(_func);	}

	// ����� ����� ������� �� ������� �����
    inline int getIndex(const std::string name);
	// ���������, ���������� �� ������ � ��������� ������ ������
	inline bool find(const std::string& name) {return index.find(name) != index.end(); } 

	T* addForce(std::string name, bool pop = true);

	friend bool registerResources();
};



template<class T>
T* ResCollection<T>::add(const std::string name, bool pop)
{
	std::string name2 = resStack.getFullName(name);
	int index = getIndex(name2);

	if(index < 0)
	{
		return addForce(name2, pop);
	}
	else
	{
		data[index].links++;

		if(!pop)
			resStack.pushFullPath(name2);

		return data[index].object;
	}
}

template<class T>
bool ResCollection<T>::remove(T* object)
{
#if (STEEL_COMPILER != GCC) || !defined(STEEL_COMPILER_DEVCPP)
    std::map<T*,int>::const_iterator it = resIndex.find(object);
     
	if(it == resIndex.end())
		return false;
	else 
		return removeRaw(it->second);
#else
// TODO
	if(resIndex.find(object) == resIndex.end())
		return false;
	else 
		return removeRaw(resIndex.find(object)->second);
#endif
}


template<class T>
bool ResCollection<T>::remove(const std::string name)
{
	int index = getIndex(name);
	if(index < 0)
		return false;
	else
		return removeRaw(index);
}

template<class T>
bool ResCollection<T>::removeRaw(int index)
{
	if(data[index].links > 0)
	{
		data[index].links--;
		if(data[index].links == 0)
		purge(index);
		return true;
	}
	else
		return false;// ��� �����
}

template<class T>
bool ResCollection<T>::purge(int delIndex)
{
	std::string name;
	name = data[delIndex].name;
	
	resIndex.erase(data[delIndex].object);
	index.erase(name);

	log_msg("res del " + id, "Deleting resourse " + data[delIndex].name);
	delete data[delIndex].object;

	if(delIndex + 1 < (int)data.size())
	{
		index[data.back().name] = delIndex;
		resIndex[data.back().object] = delIndex;
		data[delIndex] = data.back();
	}
	
	data.resize(data.size() - 1);
		
	return true;
}


template<class T>
T* ResCollection<T>::addForce(std::string name, bool pop)
{
	std::string baseDirectory;
	
	splitPath(name, baseDirectory, name);

	resStack.push(baseDirectory);

	log_msg("res " + id, "Loading " + baseDirectory + "/" + name);

    int s = classes.size();
	for(int i = 0; i < s; i++)
	{
//		T *obj = createClass(&classes[i], name, baseDirectory); // + baseDirectory

		T *obj = (*classes[i])(name, baseDirectory); // + baseDirectory

		if(obj == NULL) continue;
		
		std::string fullResName = baseDirectory + "/" + name;

		ResStorage newResStorage;
		newResStorage.id = obj->getId();
		newResStorage.name = fullResName;
		newResStorage.links = 1;
		newResStorage.object = obj;

		data.push_back(newResStorage);

		index[fullResName] = data.size() - 1;
		resIndex[obj] = data.size() - 1;
	
		log_msg("res " + id, "OK " + fullResName);
		if(pop) resStack.pop();

		return obj;
	}
	log_msg("res error " + id, "Failed " + baseDirectory + "/" + name);
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
		return data[i].object;
}

template<class T>
inline int ResCollection<T>::getIndex(const std::string name)   
{
	std::map<const std::string,int>::const_iterator it = index.find(name);
	if(it != index.end())
		return it->second;	
	else 
		return -1;
}  /*If exist - return*, esle 0 */


#endif
