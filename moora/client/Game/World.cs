using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;

namespace moora_client.Game
{
    class World
    {
        public Terrain terrain;
        public Map map;
        public Character character;
        public static readonly int x_size = 32;
        public static readonly int y_size = 32;

        public World()
        {
            terrain = new Terrain("../../Resources/terrain");
            map = new Map(21, 18, terrain);
            character = new Character(5, 5);
        }

        public void Draw(Graphics graph)
        {
            for (int x = 0; x < map.X; x++)
            {
                for (int y = 0; y < map.Y; y++)
                {
                    Bitmap image = map.GetImage(x, y);
                    if (image != null)
                    {
                        graph.DrawImage(image, new Point(x*x_size, y*y_size));
                    }
                }
            }
            character.Draw(graph);
        }

        public void Click(int x, int y)
        {
            character.MoveTo((float)x / x_size, (float)y / y_size);
        }
    }
}
