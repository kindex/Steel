using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
#if SILVERLIGHT
using System.Windows.Controls;
using System.Windows.Media;
#endif

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
                case TerrainType.Rock:
                    return 0.0f;
                case TerrainType.Sand:
                case TerrainType.Snow:
                    return 1.0f;
                case TerrainType.Dirt:
                    return 1.5f;
                case TerrainType.Rough:
                case TerrainType.Grass:
                case TerrainType.Lava:
                    return 2.0f;
                case TerrainType.Swamp:
                    return 0.4f;
                case TerrainType.Subterrnian:
                    return 3.0f;
                default:
                    return 0.0f;
            }
        }

        public void MoveTo(float nx, float ny)
        {
            int destx = GetCellX(nx);
            int desty = GetCellX(ny);

            if (destx < 0
             || desty < 0
             || destx >= map.X
             || desty >= map.Y)
            {
                return;
            }

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
                                     && y + d.Y < map.Y)
                                    {
                                        float speed = GetSpeed(map.getTerrain(x, y));
                                        if (speed > 0)
                                        {
                                            float time = 1.0f / speed * (float)System.Math.Sqrt(d.X * d.X + d.Y * d.Y);
                                            if (w[x + d.X, y + d.Y] > w[x, y] + time)
                                            {
                                                w[x + d.X, y + d.Y] = w[x, y] + time;
                                                from[x + d.X, y + d.Y] = new Point(x, y);

                                                changed = true;
                                            }
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

#if SILVERLIGHT
        System.Windows.Shapes.Polyline graph_character;
        System.Windows.Shapes.Polyline graph_path;

        public void StartDraw(Canvas canvas)
        {
            graph_path = new System.Windows.Shapes.Polyline();
            graph_path.Stroke = new SolidColorBrush(Colors.Green);
            graph_path.StrokeThickness = 1;
            canvas.Children.Add(graph_path);

            graph_character = new System.Windows.Shapes.Polyline();
            graph_character.Stroke = new SolidColorBrush(Colors.Red);
            graph_character.StrokeThickness = 10;
            graph_character.Points.Add(new System.Windows.Point(X * World.x_size - 4, Y * World.y_size - 4));
            graph_character.Points.Add(new System.Windows.Point(X * World.x_size + 4, Y * World.y_size + 4));
            canvas.Children.Add(graph_character);
        }

        public void ReDraw()
        {
            graph_character.Points.Clear();
            graph_character.Points.Add(new System.Windows.Point(X * World.x_size - 4, Y * World.y_size - 4));
            graph_character.Points.Add(new System.Windows.Point(X * World.x_size + 4, Y * World.y_size + 4));

            graph_path.Points.Clear();

            graph_path.Points.Add(new System.Windows.Point(X * World.x_size, Y * World.y_size));
            foreach (Vector2 p in path)
            {
                graph_path.Points.Add(new System.Windows.Point(p.X * World.x_size, p.Y * World.y_size));
            }
        }
#else
        public void Draw(System.Drawing.Graphics graph)
        {
            System.Drawing.SolidBrush Brush = new System.Drawing.SolidBrush(System.Drawing.Color.Red);
            int r = 16;
            graph.FillEllipse(Brush, (int)(World.x_size * position.X) - r/2, (int)(World.y_size * position.Y) - r/2, r, r);

            System.Drawing.Pen pen = new System.Drawing.Pen(System.Drawing.Color.Green);

            Vector2 prev = position;
            foreach (Vector2 p in path)
            {
                graph.DrawLine(pen, new System.Drawing.Point((int)(prev.X * World.x_size), (int)(prev.Y * World.x_size)),
                    new System.Drawing.Point((int)(p.X * World.x_size), (int)(p.Y * World.x_size)));
                prev = p;
            }
        }
#endif
    }
}
