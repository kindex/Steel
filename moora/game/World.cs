using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Game
{
    public class World
    {
        public Terrain terrain;
        public Map map;
        public Character character;
        public static readonly int x_size = 32;
        public static readonly int y_size = 32;

        public World()
        {
            Random rnd = new Random();
            List<string> dirs = new List<string>();
            dirs.Add("../../Resources/terrain");
            dirs.Add("Resources/terrain");

            terrain = new Terrain(dirs);
            map = new Map(21, 18, terrain, rnd);

            int char_x;
            int char_y;

            do
            {
                char_x = rnd.Next(0, map.X);
                char_y = rnd.Next(0, map.Y);
            }
            while (map.getTerrain(char_x, char_y) != TerrainType.Sand);

            character = new Character(char_x, char_y, map);
        }

        public void Click(int x, int y)
        {
            character.MoveTo((float)x / x_size, (float)y / y_size);
        }

        public void Update(float deltaTime)
        {
            character.Update(deltaTime);
        }
    }
}
