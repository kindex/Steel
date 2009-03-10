using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Game.Math;

namespace Game
{
    public class Character
    {
        public Vector2 Position { get { return position; } }
        public List<Vector2> Path { get { return path; } }

        Vector2 position;
        Vector2 destination;
        List<Vector2> path = new List<Vector2>();
        Map map;

        public Character(int x, int y, Map map)
        {
            destination = position = new Vector2(x + 0.5f, y + 0.5f);
            this.map = map;
        }

        public float X { get { return position.X; } }
        public float Y { get { return position.Y; } }

        public static int GetCellX(float x)
        {
            return (int)x;
        }

        public static int GetCellY(float y)
        {
            return (int)y;
        }

        public int CellX { get { return GetCellX(position.X); } }
        public int CellY { get { return GetCellY(position.Y); } }

        public void Update(float deltaTime)
        {
            float speed = GetSpeed(map.getTerrain(CellX, CellY));
            float distance = speed*deltaTime;
            Vector2 original_position = position;
            if ((destination - position).Length < distance)
            {
                position = destination;
                if (path.Count != 0 && path[0] == destination)
                {
                    path.RemoveAt(0);
                }

                if (path.Count != 0)
                {
                    destination = path[0];
                }
            }
            else
            {
                Vector2 direction = (destination - position).GetNormalized();
                position += direction*distance;

                speed = GetSpeed(map.getTerrain(CellX, CellY));
                if (speed <= 0)
                {
                    position = original_position;
                }
            }
        }

        public float GetSpeed(TerrainType terrain)
        {
            switch (terrain)
            {
                case TerrainType.None:
                case TerrainType.Water:
                    return 0.0f;
                case TerrainType.Sand:
                    return 10.0f;
                default:
                    return 0.0f;
            }
        }

        public void MoveTo(float nx, float ny)
        {
            int destx = GetCellX(nx);
            int desty = GetCellX(ny);
            int dx = System.Math.Abs(CellX - destx);
            int dy = System.Math.Abs(CellY - desty);

            if (dx == 0 && dy == 0)
            {
                destination = new Vector2(nx, ny);
            }
            else
            {
                float[,] w = new float[map.X, map.Y];
                Point[,] from = new Point[map.X, map.Y];

                for (int x = 0; x < map.X; x++)
                {
                    for (int y = 0; y < map.Y; y++)
                    {
                        w[x, y] = float.MaxValue;
                    }
                }
                w[CellX, CellY] = 0.0f;

                Point[] directions = {
                    new Point(-1, 0),
                    new Point(+1, 0),
                    new Point(0, +1),
                    new Point(0, -1),
                    new Point(-1, -1),
                    new Point(+1, +1),
                    new Point(-1, +1),
                    new Point(+1, -1),
                };

                bool changed;

                do
                {
                    changed = false;
                    for (int x = 0; x < map.X; x++)
                    {
                        for (int y = 0; y < map.Y; y++)
                        {
                            if (w[x, y] < float.MaxValue)
                            {
                                foreach (Point d in directions)
                                {
                                    if (x + d.X >=0
                                     && x + d.X < map.X
                                     && y + d.Y >=0
                                     && y + d.Y < map.Y
                                     && map.getTerrain(x + d.X, y + d.Y) == TerrainType.Sand)
                                    {
                                        float speed = GetSpeed(map.getTerrain(x, y)) * (float)System.Math.Sqrt(d.X * d.X + d.Y * d.Y);
                                        if (w[x + d.X, y + d.Y] > w[x, y] + speed)
                                        {
                                            w[x + d.X, y + d.Y] = w[x, y] + speed;
                                            from[x + d.X, y + d.Y] = new Point(x, y);

                                            changed = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                while (changed);

                if (w[destx, desty] < float.MaxValue)
                {
                    int cx = destx;
                    int cy = desty;
                    path = new List<Vector2>();

                    while (cx != CellX || cy != CellY)
                    {
                        Point next = from[cx, cy];

                        cx = next.X;
                        cy = next.Y;

                        if (cx != CellX || cy != CellY)
                        {
                            path.Add(new Vector2(cx + 0.5f, cy + 0.5f));
                        }
                    }

                    path.Reverse();
                    path.Add(new Vector2(nx, ny));
                    destination = path[0];
                }
                else
                {
                    path.Clear();
                    destination = position;
                }
            }
        }
    }
}
