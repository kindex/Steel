using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Game
{
    public class Map
    {
        public class Cell
        {
            public TerrainType terrain;

            public Cell(TerrainType terrain)
            {
                this.terrain = terrain;
            }
        }

        protected Cell[,] _map;
        protected Terrain terrain;

        protected Map(Terrain terrain)
        {
            this.terrain = terrain;
        }

        public Cell getCell(int x, int y)
        {
            return _map[x, y];
        }

        protected void SetTerrain(int x, int y, TerrainType terrain)
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

        Dictionary<Point, string> image_cache = new Dictionary<Point, string>();

        public void ClearImageCache()
        {
            image_cache.Clear();
        }

        public string GetImage(int x, int y)
        {
            Point key = new Point(x, y);
            if (image_cache.ContainsKey(key))
            {
                return image_cache[key];
            }

            string result = terrain.GetImage(this, x, y);
            image_cache[key] = result;

            return result;
        }
    }
}
