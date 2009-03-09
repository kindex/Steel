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
        const int x_size = 32;
        const int y_size = 32;

        public World()
        {
            terrain = new Terrain("../../Resources/terrain");
            map = new Map(21, 18, terrain);
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

        }
    }
}
