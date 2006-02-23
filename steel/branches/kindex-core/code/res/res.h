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
using namespace std;

class Res
{
public:
	virtual bool init(string& name);
	virtual bool load(ifstream &f) = 0;
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

	vector<Res*> data;
	map<const string,int> index; // �� ����� ���������� ������ � ������� data
	vector<string> names;

	map<const string, ClassCopy> classes;

	int freeindex;
public:
	ResCollection(): freeindex(0) {}

	Res* operator [] (const int n)        { return data[n]; }
    Res* operator [] (const string& name) { return data[getindex(name)]; }

    int getindex(const string& name)
    {
        return index[name];
    }  /*If exist - return*, esle 0 */

    int lastinsertedid(){ return freeindex-1; }
    void setname(int n, string name) { index[name] = n; names[n] = name; }

	Res* addForce(const string& name);
	Res* add(const string& name);

/*
��������� 2 ������� ���������� ����� �� ����� � ������� ��������� ������������ ������.
������������ ������� �� �����, �� ����� � ������ ����� malloc+memcpy ������� ������
� ������� ��� ����������� ��� ���.
*/
	void registerClass(Res* Class, int size, string fileextension);
	Res* getClass(string fileextension);
};


#endif
