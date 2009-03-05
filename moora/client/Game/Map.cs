using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace moora_client.Game
{
    class Map
    {
        public enum TerrainType
        {
            None,
            Water,
            Grass
        }

        TerrainType[,] _map;

        public TerrainType getTerrain(int x, int y)
        {
            return _map[x, y];
        }
    }
}
