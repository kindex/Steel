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
        public Map map;
        public Character character;
        const int x_size = 32;
        const int y_size = 32;

        public class TerrainImage
        {
            public string filename;
            public Map.TerrainType[] mask = new Map.TerrainType[9];
            public Bitmap image;

            public TerrainImage(string path, string filename)
            {
                this.filename = filename;
                this.image = new System.Drawing.Bitmap(Path.Combine(path, filename));

                for (int i = 0; i < 9; i++)
                {
                    char c = filename[i];
                    switch (c)
                    {
                        case 's': mask[i] = Map.TerrainType.Sand; break;
                        case 'w': mask[i] = Map.TerrainType.Water; break;
                        case '_': mask[i] = Map.TerrainType.Any; break;
                    }
                }
            }
        }
        List<TerrainImage> terrain_images = new List<TerrainImage>();

        public World()
        {
            map = new Map(21, 18);

            string path = "../../Resources/terrain";
            DirectoryInfo di = new DirectoryInfo(path);
            FileInfo[] rgFiles = di.GetFiles("*.png");
            foreach (FileInfo fi in rgFiles)
            {
                terrain_images.Add(new TerrainImage(path, fi.Name));
            }
        }

        Bitmap FitImage(int x, int y)
        {
            Point[] d = {
                             new Point(0, 0), 
                             new Point(-1, -1), 
                             new Point(0, -1), 
                             new Point(1, -1), 
                             new Point(1, 0), 
                             new Point(1, 1), 
                             new Point(0, 1), 
                             new Point(-1, 1), 
                             new Point(-1, 0)
                         };

            Map.TerrainType[] find = new Map.TerrainType[9];
            for (int i = 0; i < 9; i++)
            {
                find[i] = map.getTerrain(x + d[i].X, y + d[i].Y);
            }

            foreach (TerrainImage t in terrain_images)
            {
                bool fit = true;
                for (int i = 0; i < 9; i++)
                {
                    fit = fit && (t.mask[i] == Map.TerrainType.Any || t.mask[i] == find[i]);
                }

                if (fit)
                {
                    return t.image;
                }
            }

            return null;
        }

        public void Draw(Graphics graph)
        {
            for (int x = 0; x < map.X; x++)
            {
                for (int y = 0; y < map.Y; y++)
                {
                    Bitmap image = FitImage(x, y);
                    if (image != null)
                    {
                        graph.DrawImage(image, new Point(x*x_size, y*y_size));
                    }
                }
            }

        }
    }
}
