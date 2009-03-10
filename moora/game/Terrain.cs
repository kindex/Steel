using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Game
{
    public enum TerrainType
    {
        None,
        Water,
        Sand,
        Any
    }

    public struct Point
    {
        public int X;
        public int Y;

        public Point(int x, int y)
        {
            X = x;
            Y = y;
        }
    }

    public class Terrain
    {
        public class TerrainImage
        {
            public string filename;
            public TerrainType[] mask = new TerrainType[9];
            public string image;

            public TerrainImage(string path, string filename)
            {
                this.filename = filename;
                this.image = Path.Combine(path, filename);

                for (int i = 0; i < 9; i++)
                {
                    char c = filename[i];
                    switch (c)
                    {
                        case 's': mask[i] = TerrainType.Sand; break;
                        case 'w': mask[i] = TerrainType.Water; break;
                        case '_': mask[i] = TerrainType.Any; break;
                    }
                }
            }
        }

        List<TerrainImage> terrain_images = new List<TerrainImage>();

        public Terrain(List<string> directories)
        {
            foreach (string dir in directories)
            {
                try
                {
                    DirectoryInfo di = new DirectoryInfo(dir);
                    FileInfo[] rgFiles = di.GetFiles("*.png");
                    foreach (FileInfo fi in rgFiles)
                    {
                        terrain_images.Add(new TerrainImage(dir, fi.Name));
                    }
                }
                catch (System.IO.DirectoryNotFoundException)
                {
                }
            }
        }

        static Point[] directions = {
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

        public string GetImage(Map map, int x, int y)
        {
            TerrainType[] find = new TerrainType[9];
            for (int i = 0; i < 9; i++)
            {
                find[i] = map.getTerrain(x + directions[i].X, y + directions[i].Y);
            }

            foreach (TerrainImage t in terrain_images)
            {
                bool fit = true;
                for (int i = 0; i < 9; i++)
                {
                    fit = fit && (t.mask[i] == TerrainType.Any || t.mask[i] == find[i]);
                }

                if (fit)
                {
                    return t.image;
                }
            }

            return null;
        }
    }
}
