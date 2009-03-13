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
        Grass,
        Snow,
        Lava,
        Rock,
        Swamp,
        Rough,
        Subterrnian,
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
        //public static char TerrainToChar(TerrainType terrain)
        //{
        //    switch (terrain)
        //    {
        //    }
        //}

        public static TerrainType CharToTerrain(char c)
        {
            switch (c)
            {
                case 's': return TerrainType.Sand;
                case 'w': return TerrainType.Water;
                case 'g': return TerrainType.Grass;
                case 'n': return TerrainType.Snow;
                case 'l': return TerrainType.Lava;
                case 'p': return TerrainType.Swamp;
                case 'r': return TerrainType.Rough;
                case 't': return TerrainType.Subterrnian;
                case '_': return TerrainType.Any;
                default: return TerrainType.None;
            }
        }

        public class TerrainImage
        {
            public string filename;
            public TerrainType[] mask = new TerrainType[9];
            public string image;

            public TerrainImage(string path, string filename)
            {
                this.filename = filename;
                this.image = path + "/" + filename;

                for (int i = 0; i < 9; i++)
                {
                    mask[i] = CharToTerrain(filename[i]);
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
                catch (System.MethodAccessException)
                {
                    terrain_images.Clear();
                    terrain_images.Add(new TerrainImage(dir, "s________.png"));
                    terrain_images.Add(new TerrainImage(dir, "w_s_s_www.png"));
                    terrain_images.Add(new TerrainImage(dir, "w_s_www_s.png"));
                    terrain_images.Add(new TerrainImage(dir, "w_s_wwwww.png"));
                    terrain_images.Add(new TerrainImage(dir, "w_www_s_s.png"));
                    terrain_images.Add(new TerrainImage(dir, "w_wwwww_s.png"));
                    terrain_images.Add(new TerrainImage(dir, "wswwwwwww.png"));
                    terrain_images.Add(new TerrainImage(dir, "www_s_s_w.png"));
                    terrain_images.Add(new TerrainImage(dir, "www_s_www.png"));
                    terrain_images.Add(new TerrainImage(dir, "wwwswwwww.png"));
                    terrain_images.Add(new TerrainImage(dir, "wwwww_s_w.png"));
                    terrain_images.Add(new TerrainImage(dir, "wwwwwswww.png"));
                    terrain_images.Add(new TerrainImage(dir, "wwwwwwwsw.png"));
                    terrain_images.Add(new TerrainImage(dir, "wwwwwwwww.png"));
                }
            }

            if (terrain_images.Count == 0)
            {
                throw new Exception("No terrain images");
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
                if (find[i] == TerrainType.None)
                {
                    find[i] = find[0];
                }
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
