/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Generator.
		This is homework for OOP subject at LU master program 2006.
 ************************************************************/

#include "labyrinth_generator.h"
#include <vector>
#include <common/logger.h>

class LabyrinthGenerator
{
public:
	LabyrinthGenerator(int x, int y);
	void buid(float k);
	Labyrinth getLabyrinth() const;

private:
	struct node
	{
		int dist;
		bool path;
		bool rightborder, downborder, was;

		node(void): dist(MAX), path(false), was(false), rightborder(true), downborder(true) {}
	};

	std::vector<std::vector<node> > a;
	static const int MAX = 0xFFFFFF;
	int X;
	int Y;

	bool deep(int x, int y, int level);
	void findpath(int x, int y);
};

bool LabyrinthGenerator::deep(int x, int y, int level)
{
    node& current = a[y][x];
	if (current.was)
	{
		return false;
	}
	current.dist = level;
	current.was = true;
	if (x == X-1 && y == Y-1)
	{
		return true;
	}

	bool used_directions[4] = {false, false, false, false};
	int total = 0;

	while (total < 4)
	{
		int d;
		do
		{
			d = (rand()%6)%4;
		}
		while (used_directions[d]);
		total++;

		used_directions[d] = true;

		switch(d)
		{
			case 0: 
				if (x > 0)
				{
					deep(x - 1, y, level + 1);
				}
				break;
			case 1:
				if (y > 0)
				{
					deep(x, y - 1, level + 1); 
				}
				break;
			case 2:
				if (x < X-1)
				{
					deep(x + 1, y, level + 1);
				}
				break;
			case 3:
				if (y < Y-1)
				{
					deep(x, y + 1, level + 1); 
				}
				break;
		}
	}
	return false;
}

void LabyrinthGenerator::findpath(int x, int y)
{
	node& cur_node = a[y][x];
	cur_node.path = true;
	int d = cur_node.dist - 1;
	if (y > 0 && a[y-1][x].dist == d) findpath(x, y - 1); else
	if (x > 0 && a[y][x-1].dist == d) findpath(x - 1, y); else
	if (y < Y-1 && a[y+1][x].dist == d) findpath(x, y + 1); else
	if (x < X-1 && a[y][x+1].dist == d) findpath(x + 1, y);
}

LabyrinthGenerator::LabyrinthGenerator(int x, int y) : X(x), Y(y)
{
	a.resize(Y);
	for (int i = 0; i < Y; i++)
	{
		a[i].resize(X);
	}
    if (!a.empty() && !a[0].empty())
    {
	    a[0][0].dist = 0;
    }
}

void LabyrinthGenerator::buid(float k)
{
    if (!a.empty() && !a[0].empty())
    {
	    deep(0, 0, 0);
    }
	//findpath(X-1, Y-1);
	//findpath(X-1, 0);
	//findpath(0, Y-1);

    std::string d("\n");

	for(int i = 0; i < Y; i++)
	{
        d += "|";
		for(int j = 0; j < X; j++)
		{
            d += "\t" + IntToStr(a[i][j].dist);
        }
        d += "|\n";
    }
    log_msg("labyrinth", d);

	for(int i = 0; i < Y; i++)
	{
		for(int j = 0; j < X-1; j++)
		{
			a[i][j].rightborder = frand() > k && abs(a[i][j].dist - a[i][j+1].dist) != 1;
//			a[i][j].rightborder = !(a[i][j].path && a[i][j+1].path);
		}
	}
	for(int i = 0; i < Y-1; i++)
	{
		for(int j = 0; j < X; j++)
		{
			a[i][j].downborder = frand() > k && abs(a[i][j].dist - a[i+1][j].dist) != 1;
//			a[i][j].downborder = !(a[i][j].path && a[i+1][j].path);
		}
	}
}

Labyrinth LabyrinthGenerator::getLabyrinth() const
{
	Labyrinth result;
	result.a.resize(a.size());
	for (size_t i = 0; i < a.size(); i++)
	{
		result.a[i].resize(a[i].size());
		for (size_t j = 0; j < a[i].size(); j++)
		{
			result.a[i][j].downborder = a[i][j].downborder;
			result.a[i][j].rightborder = a[i][j].rightborder;
		}
	}
	return result;
}


bool Labyrinth::isRightBorder(int x, int y) const
{
	if (x < 0 || y < 0)
	{
		return true;
	}
	size_t sx = x;
	size_t sy = y;
	return sy + 1 > a.size()
		|| sx     >= a[sy].size()
		|| a[sy][sx].rightborder;
}

bool Labyrinth::isDownBorder(int x, int y) const
{
	if (x < 0 || y < 0)
	{
		return true;
	}
	size_t sx = x;
	size_t sy = y;
	return sy + 1 > a.size()
		|| sx  >= a[sy].size()
		|| a[sy][sx].downborder;
}

int Labyrinth::getMaxX() const
{
    if (a.empty())
    {
        return 0;
    }
    else
    {
	    return a[0].size();
    }
}

int Labyrinth::getMaxY() const
{
	return a.size();
}

Labyrinth generateLabyrinth(size_t x, size_t y, float k)
{
	LabyrinthGenerator lab(x, y);
	lab.buid(k);
	return lab.getLabyrinth();
}

////////////////////////////////////////////////////////////
//int main()
//{
//	int X = 8;
//	int Y = 8;
//	Labyrinth lab(X, Y);
//	lab.buid();
//
//	for(int i = 0; i < X; i++)
//	{
//		cout << "+-";
//	}
//	cout << "+\n";
//
//	for(int i = 0; i < Y; i++)
//	{
//		cout << "|";
//		for(int j = 0; j < X; j++)
//		{
//			if (lab.a[i][j].path > 0)
//			{
//				cout << "*";
//			}
//			else
//			{
//				cout << " ";
//			}
//
//			if (lab.rightborder(j, i))
//			{
//				cout << "|";
//			}
//			else
//			{
//				cout << " ";
//			}
//		}
//		cout << endl;
//		cout << "+";
//		for(int j = 0; j < X; j++)
//		{
//			if (lab.downborder(j, i))
//			{
//				cout << "-+";
//			}
//			else
//			{
//				cout << " +";
//			}
//		}
//		cout << endl;
//	}
//
//	return 0;
//}
//
//
