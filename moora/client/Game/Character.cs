using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace moora_client.Game
{
    class Character
    {
        Vector2 position;
        Vector2 destination;
        List<Vector2> path = new List<Vector2>();
        Map map;

        public Character(int x, int y, Map map)
        {
            destination = position = new Vector2(x + 0.5f, y + 0.5f);
            this.map = map;
        }

        public void Draw(Graphics graph)
        {
            SolidBrush Brush = new SolidBrush(Color.Red);
            int r = 16;
            graph.FillEllipse(Brush, (int)(World.x_size * position.X) - r/2, (int)(World.y_size * position.Y) - r/2, r, r);

            Pen pen = new Pen(Color.Green);
            Point a = new Point(CellX*World.x_size, CellY*World.x_size);
            Point b = new Point((CellX + 1) * World.x_size, CellY * World.x_size);
            Point c = new Point((CellX + 1) * World.x_size, (CellY+1)* World.x_size);
            Point d = new Point((CellX + 0) * World.x_size, (CellY+1) * World.x_size);
            graph.DrawLine(pen, a, b);
            graph.DrawLine(pen, b, c);
            graph.DrawLine(pen, c, d);
            graph.DrawLine(pen, d, a);

            Vector2 prev = position;
            foreach (Vector2 p in path)
            {
                graph.DrawLine(pen, new Point((int)(prev.X * World.x_size), (int)(prev.Y * World.x_size)),
                    new Point((int)(p.X * World.x_size), (int)(p.Y * World.x_size)));
                prev = p;
            }
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
            int dx = Math.Abs(CellX - destx);
            int dy = Math.Abs(CellY - desty);

//            if (dx == 0 && dy <= 1 || dx <= 1 && dy == 0)
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
                                        float speed = GetSpeed(map.getTerrain(x, y)) * (float)Math.Sqrt(d.X * d.X + d.Y * d.Y);
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
