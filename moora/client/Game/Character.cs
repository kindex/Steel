using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace moora_client.Game
{
    class Character
    {
        class Position
        {
            public float X;
            public float Y;

            public Position(float X, float Y)
            {
                this.X = X;
                this.Y = Y;
            }
        }

        Position position;

        public Character(int x, int y)
        {
            position = new Position(x + 0.5f, y + 0.5f);
        }

        public void Draw(Graphics graph)
        {
            SolidBrush Brush = new SolidBrush(Color.Red);
            graph.FillEllipse(Brush, (int)(World.x_size * position.X), (int)(World.y_size * position.Y), 15, 15);
        }

        public void MoveTo(float x, float y)
        {
            position.X = x;
            position.Y = y;
        }

        public float X { get { return position.X; } }
        public float Y { get { return position.Y; } }
    }
}
