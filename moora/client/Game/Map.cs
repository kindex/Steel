using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace moora_client.Game
{
    class Map
    {
        public class Cell
        {
            public TerrainType terrain;

            public Cell(TerrainType terrain)
            {
                this.terrain = terrain;
            }
        }

        Cell[,] _map;
        Terrain terrain;

        public Map(int x, int y, Terrain terrain)
        {
            this.terrain = terrain;
            Random rnd = new Random();

            _map = new Cell[x, y];
            for (int i = 0; i < x; i++)
            {
                for (int j = 0; j < y; j++)
                {
                    double r = rnd.NextDouble();
                    if (r < 0.85)
                    {
                        SetTerrain(i, j, TerrainType.Water);
                    }
                    else
                    {
                        SetTerrain(i, j, TerrainType.Sand);
                    }
                }
            }

            bool changed;

            do
            {
                changed = false;
                for (int i = 0; i < x; i++)
                {
                    for (int j = 0; j < y; j++)
                    {
                        if (getTerrain(i - 1, j) != TerrainType.Water
                         && getTerrain(i, j) == TerrainType.Water
                         && getTerrain(i + 1, j) != TerrainType.Water)
                        {
                            SetTerrain(i, j, getTerrain(i - 1, j));
                            changed = true;
                        }

                        if (getTerrain(i, j - 1) != TerrainType.Water
                         && getTerrain(i, j) == TerrainType.Water
                         && getTerrain(i, j + 1) != TerrainType.Water)
                        {
                            SetTerrain(i, j, getTerrain(i, j - 1));
                            changed = true;
                        }

                        if (getTerrain(i - 1, j - 1) != TerrainType.Water
                         && getTerrain(i, j) == TerrainType.Water
                         && getTerrain(i + 1, j + 1) != TerrainType.Water)
                        {
                            SetTerrain(i, j, getTerrain(i - 1, j - 1));
                            changed = true;
                        }

                        if (getTerrain(i - 1, j + 1) != TerrainType.Water
                         && getTerrain(i, j) == TerrainType.Water
                         && getTerrain(i + 1, j - 1) != TerrainType.Water)
                        {
                            SetTerrain(i, j, getTerrain(i - 1, j + 1));
                            changed = true;
                        }
                    }
                }
            }
            while (changed);

            do
            {
                changed = false;
                ClearImageCache();
                for (int i = 0; i < x && !changed; i++)
                {
                    for (int j = 0; j < y && !changed; j++)
                    {
                        if (GetImage(i, j) == null)
                        {
                            SetTerrain(i, j, TerrainType.Sand);
                            changed = true;
                        }
                    }
                }
            }
            while (changed);
            ClearImageCache();
        }

        public Cell getCell(int x, int y)
        {
            return _map[x, y];
        }

        void SetTerrain(int x, int y, TerrainType terrain)
        {
            _map[x, y] = new Cell(terrain);
        }

        public TerrainType getTerrain(int x, int y)
        {
            if (x == -1)
            {
                return getTerrain(0, y);
            }
            else if (y == -1)
            {
                return getTerrain(x, 0);
            }
            else if (x == X)
            {
                return getTerrain(x - 1, y);
            }
            else if (y == Y)
            {
                return getTerrain(y, y - 1);
            }
            else if (x < 0 || x >= X || y < 0 || y >= Y)
            {
                return TerrainType.None;
            }
            else
            {
                return _map[x, y].terrain;
            }
        }

        public int X { get {return _map.GetLength(0); } }
        public int Y { get { return _map.GetLength(1); } }

        Dictionary<Point, Bitmap> image_cache = new Dictionary<Point, Bitmap>();

        public void ClearImageCache()
        {
            image_cache.Clear();
        }

        public Bitmap GetImage(int x, int y)
        {
            Point key = new Point(x, y);
            if (image_cache.ContainsKey(key))
            {
                return image_cache[key];
            }

            Bitmap result = terrain.GetImage(this, x, y);
            image_cache[key] = result;

            return result;
        }

    }
}
