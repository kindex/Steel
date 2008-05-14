/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Visiblility check
 ************************************************************/

#include "visibility_check.h"
#include <common/containers.h>
#include <cmath>
#include <algorithm>
#include <queue>

Cell getCell(const v3 position,
             const v2 cell_size)
{
    return std::make_pair(int(floor(position.x/cell_size.x)),
                          int(floor(position.y/cell_size.y)));
}

Cell getNormalizedCell(const v2 position)
{
    return std::make_pair(int(position.x), int(position.y));
}

struct Point
{
    Point(int x, int y,
          int x2, int y2,
          int lab_x, int lab_y,
          int wall_id, double angle, bool outer) : 
        x(x),
        y(y),
        x2(x2),
        y2(y2),
        lab_x(lab_x),
        lab_y(lab_y),
        wall_id(wall_id),
        angle(angle),
        outer(outer)
    {}
    int x;
    int y;
    int x2;
    int y2;
    int lab_x;
    int lab_y;
    int wall_id;
    double angle;
    bool outer;

    static double get_angle(double x, double y)
    {
        double len = sqrt(x*x + y*y);
        double cosa = x/len;
        double sina = y/len;
        return (sina)/(1 + cosa - EPSILON2);
    }

    bool operator < (const Point& second) const
    {
        return angle < second.angle;
    }
};

struct Wall
{
    Wall(int wall_id, double dist, int lab_x, int lab_y) :
        wall_id(wall_id),
        dist(dist),
        lab_x(lab_x),
        lab_y(lab_y)
    {}

    int wall_id;
    double dist;
    int lab_x;
    int lab_y;

    bool operator < (const Wall& second) const
    {
        return dist > second.dist;
    }
};

double getDist(v2 eye, Point point)
{
    return (eye-v2(float(point.x), float(point.y))).getSquaredLength();
}

void addWall(int x1, int y1,
             int x2, int y2,
             int lab_x, int lab_y,
             int wall_idx,
             const v2 eye,
             IN OUT std::vector<Point>& points)
{
    double a1 = Point::get_angle(x1 - eye.x, y1 - eye.y);
    double a2 = Point::get_angle(x2 - eye.x, y2 - eye.y);

    bool outer = ((y1 > eye.y) != (y2 > eye.y)) && (x1 < eye.x) && (x1 == x2);

    points.push_back(Point(x1, y1, x2, y2, lab_x, lab_y, wall_idx, a1, outer));
    points.push_back(Point(x2, y2, x1, y1, lab_x, lab_y, wall_idx, a2, outer));
}

void fill_line(Cell cell, IN OUT CellVisibilitySet& result)
{
    //v2 a = (eye - point).getNormalized()*EPSILON;
    //result.insert(getNormalizedCell(point + a));
    
    //result.insert(getNormalizedCell(v2(point.x - EPSILON, point.x - EPSILON)));
    //result.insert(getNormalizedCell(v2(point.x - EPSILON, point.x + EPSILON)));
    //result.insert(getNormalizedCell(v2(point.x + EPSILON, point.x - EPSILON)));
    //result.insert(getNormalizedCell(v2(point.x + EPSILON, point.x + EPSILON)));

    result.insert(cell);

    // TODO: cycle
}

CellVisibilitySet calculateCellVisibilitySet(const Labyrinth& labyrinth,
                                             const v3&        camera,
                                             const v2         cell_size)
{
    CellVisibilitySet result;

    v2 eye = v2(camera.x/cell_size.x, camera.y/cell_size.y);
  
    std::vector<Point> points;
    int wall_idx = 0;

    for (int i = -1; i < labyrinth.getMaxX(); i++)
    {
        for (int j = -1; j < labyrinth.getMaxY(); j++)
        {
            if (labyrinth.isRightBorder(i, j))
            {
                addWall(i+1, j, i+1, j+1, i, j, wall_idx, eye, points);
                wall_idx++;
            }

			if (labyrinth.isDownBorder(i, j))
			{
                addWall(i, j+1, i+1, j+1, i, j, wall_idx, eye, points);
                wall_idx++;
            }
        }
    }

    std::sort(points.begin(), points.end());
    std::set<int> current_walls;
    std::set<int> exited_walls;
    std::priority_queue<Wall> queue;

    for EACH(std::vector<Point>, points, it)
    {
        Point& point = *it;

        if (point.outer && current_walls.find(point.wall_id) == current_walls.end())
        {
            current_walls.insert(point.wall_id);
            queue.push(
                Wall(point.wall_id,
                     getDist(eye, point),
                     point.lab_x,
                     point.lab_y));
        }
    }

    Cell prev_cell = std::make_pair(int(eye.x), int(eye.y));
    if (!queue.empty())
    {
        int wall_id = *current_walls.find(queue.top().wall_id);
        for EACH(std::vector<Point>, points, it)
        {
            if (it->wall_id == wall_id)
            {
                prev_cell = std::make_pair(it->x, int(eye.y));
                break;
            }
        }
    }

    bool top = false;
    for EACH(std::vector<Point>, points, it)
    {
        Point& point = *it;
        std::set<int>::iterator s = current_walls.find(point.wall_id);
        bool end = false;
        if (s == current_walls.end()) 
        { // new wall
            current_walls.insert(point.wall_id);
            queue.push(
                Wall(
                    point.wall_id,
                    getDist(eye, point),
                    point.lab_x,
                    point.lab_y));
            top = queue.top().wall_id == point.wall_id;
        }
        else
        { // old wall
            top = queue.top().wall_id == point.wall_id;
            exited_walls.insert(point.wall_id);
            current_walls.erase(s);
            while (!queue.empty() && present(queue.top().wall_id, exited_walls))
            {
                erase(queue.top().wall_id, exited_walls);
                queue.pop();
                end = true;
            }
        }

        Cell new_cell = std::make_pair(point.lab_x, point.lab_y);
        if (end && !queue.empty() && ((it+1) == points.end() || (it+1)->x != point.x || (it+1)->y != point.y))
        {
            new_cell = std::make_pair(queue.top().lab_x, queue.top().lab_y);
            //v2 dir = new_point - eye;
            //float a;
            //float b;
            //if (isCross(queue.top().line, Line(v3(eye.x, eye.y, 0.0f), v3(dir.x, dir.y, 0.0f)), a, b))
            //{
            //    v3 p = queue.top().line.point(a);
            //    new_point.set(p.x, p.y);
            //}
        }

        if (top && prev_cell != new_cell)
        {
            fill_line(prev_cell, result);
            fill_line(new_cell, result);
            prev_cell = new_cell;
            top = false;
        }
    }

    result.insert(getCell(camera, cell_size));

    CellVisibilitySet result2;
    for EACH(CellVisibilitySet, result, it)
    {
        result2.insert(std::make_pair(it->first, it->second));
        result2.insert(std::make_pair(it->first - 1, it->second - 1));
        result2.insert(std::make_pair(it->first - 1, it->second + 1));
        result2.insert(std::make_pair(it->first + 1, it->second - 1));
        result2.insert(std::make_pair(it->first + 1, it->second + 1));
    }

    return result2;
}