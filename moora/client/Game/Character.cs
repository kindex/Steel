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

        public Character(int x, int y)
        {
            destination = position = new Vector2(x + 0.5f, y + 0.5f);
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
        }

        public void MoveTo(float x, float y)
        {
            destination = new Vector2(x, y);
        }

        public float X { get { return position.X; } }
        public float Y { get { return position.Y; } }
        public int CellX { get { return (int)position.X; } }
        public int CellY { get { return (int)position.Y; } }

        public void Update(float deltaTime)
        {
            const float speed = 1.0f;
            float distance = speed*deltaTime;
            if ((destination - position).Length < distance)
            {
                position = destination;
            }
            else
            {
                Vector2 direction = (destination - position).GetNormalized();
                position += direction*distance;
            }
        }
    }
}
