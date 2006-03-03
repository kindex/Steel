#ifndef __RES_H
#define __RES_H

/*
�������� ��������. ������� ������ ��� �����:
* ��������
* ������, ��������������� �������� (�������� ��������)
* ���� ���������, ����� �������
������ ���� �� ������ �� ����� ���� ������, ���:
Model, Image, Audio, Engine Settings, Animation, Weapon Settings
�� ��� ����������� ������ � ���������������� ���������, ��������:
Image: BMP, JPEG, PNG

������ ������ ���������������� ���� � �����, �� �������� �� �����������, ��������
���������� �����, ��� ��� ������ �������� �� ������ ����������� � ������� �� ���� ����.
�� ����, ������ ����� � ������ �� ��������� - � ���� ����� ����������� ���������, ����� 
�����-��������??

ResCollection ������ ���������� ��� ���� ��������, �������� ������ ��� ��������, ��������,
���� ��� ���������� ��� �������� ������.
*/

#include <map>
#include <vector>
#include <string>
#include <fstream>

// Resourse stream
class rstream: public std::ifstream
{
public:
	rstream(std::string s) 
	{ 
		open(s.c_str(), std::ios::binary | std::ios::in); 
	}
/*	bool good();
	bool bad();*/
	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};

class Res
{
public:
	virtual bool init(std::string& name) = 0;
//	virtual bool load(rstream &f, int size) = 0;
	virtual bool unload() = 0;
//	virtual bool reload() = 0; // reload image on driver change
};

struct ClassCopy
{
	Res* data;
	int size;
};

class ResCollection
{
//	typedef map<const string,int> t_index;
//	typedef t_index::value_type value_type;
//	typedef vector<string> t_names;

	std::vector<Res*> data;
	std::map<const std::string,int> index; // �� ����� ���������� ������ � ������� data
	std::vector<std::string> names;

	std::map<const std::string, ClassCopy> classes;

	int freeindex;
public:
	ResCollection(): freeindex(0) {}

	Res* operator [] (const int n)        { return data[n]; }
    Res* operator [] (const std::string& name) { return data[getindex(name)]; }

    int getindex(const std::string& name)
    {
        return index[name];
    }  /*If exist - return*, esle 0 */

    int lastinsertedid(){ return freeindex-1; }
    void setname(int n, std::string name) { index[name] = n; names[n] = name; }

	Res* addForce(const std::string& name);
	Res* add(const std::string& name);

/*
��������� 2 ������� ���������� ����� �� ����� � ������� ��������� ������������ ������.
������������ ������� �� �����, �� ����� � ������ ����� malloc+memcpy ������� ������
� ������� ��� ����������� ��� ���.
*/
	void registerClass(Res* Class, int size, std::string fileextension);
	Res* getClass(std::string fileextension);
};


#endif
