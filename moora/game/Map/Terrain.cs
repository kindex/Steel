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
        Dirt,
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
        Random rnd;

        public static TerrainType CharToTerrain(char c)
        {
            switch (c)
            {
                case 's': return TerrainType.Sand;
                case 'w': return TerrainType.Water;
                case 'd': return TerrainType.Dirt;
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

        public Terrain(Random rnd)
        {
            this.rnd = rnd;
            foreach (string dir in Res.SearchPath.Paths)
            {
                try
                {
                    string current_dir = dir + "/terrain";
                    DirectoryInfo di = new DirectoryInfo(current_dir);
                    FileInfo[] rgFiles = di.GetFiles("*.png");
                    foreach (FileInfo fi in rgFiles)
                    {
                        terrain_images.Add(new TerrainImage(current_dir, fi.Name));
                    }
                }
                catch (System.IO.DirectoryNotFoundException)
                {
                }
                catch (System.MethodAccessException)
                {
                    terrain_images.Clear();
                    string cdir = dir + "/terrain";
                    terrain_images.Add(new TerrainImage(cdir, "s________.png"));
                    terrain_images.Add(new TerrainImage(cdir, "w_s_s_www.png"));
                    terrain_images.Add(new TerrainImage(cdir, "w_s_www_s.png"));
                    terrain_images.Add(new TerrainImage(cdir, "w_s_wwwww.png"));
                    terrain_images.Add(new TerrainImage(cdir, "w_www_s_s.png"));
                    terrain_images.Add(new TerrainImage(cdir, "w_wwwww_s.png"));
                    terrain_images.Add(new TerrainImage(cdir, "wswwwwwww.png"));
                    terrain_images.Add(new TerrainImage(cdir, "www_s_s_w.png"));
                    terrain_images.Add(new TerrainImage(cdir, "www_s_www.png"));
                    terrain_images.Add(new TerrainImage(cdir, "wwwswwwww.png"));
                    terrain_images.Add(new TerrainImage(cdir, "wwwww_s_w.png"));
                    terrain_images.Add(new TerrainImage(cdir, "wwwwwswww.png"));
                    terrain_images.Add(new TerrainImage(cdir, "wwwwwwwsw.png"));
                    terrain_images.Add(new TerrainImage(cdir, "wwwwwwwww.png"));
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

        TerrainType getTerrainReleay(TerrainType cell, TerrainType border)
        {
            switch (cell)
            {
                case TerrainType.Water:
                    if (border != cell)
                    {
                        return TerrainType.Sand;
                    }
                    break;

                case TerrainType.Grass:
                case TerrainType.Snow:
                case TerrainType.Swamp:
                case TerrainType.Subterrnian:
                case TerrainType.Lava:
                case TerrainType.Rock:
                    if (border == TerrainType.Sand || border == TerrainType.Rock || border == TerrainType.Water)
                    {
                        return TerrainType.Sand;
                    }
                    if (border != cell)
                    {
                        return TerrainType.Dirt;
                    }
                    break;
            }
            return border;
        }

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

                if (i >= 1)
                {
                    find[i] = getTerrainReleay(find[0], find[i]);
                }
            }

            List<TerrainImage> fitTerrain = new List<TerrainImage>();

            foreach (TerrainImage t in terrain_images)
            {
                bool fit = true;
                for (int i = 0; i < 9; i++)
                {
                    fit = fit && (t.mask[i] == TerrainType.Any || t.mask[i] == find[i]);
                }

                if (fit)
                {
                    fitTerrain.Add(t);
                }
            }

            if (fitTerrain.Count != 0)
            {
                return fitTerrain[ rnd.Next(0, fitTerrain.Count) ].image;
            }
            else
            {
                return null;
            }
        }
    }
}
