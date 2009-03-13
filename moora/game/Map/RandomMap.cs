using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Game
{
    public class RandomMap : Map
    {
        public RandomMap(int x, int y, Terrain terrain, Random rnd)
            : base(terrain)
        {
            GenerateRandomMap(x, y, rnd);
        }

        void GenerateRandomMap(int x, int y, Random rnd)
        {
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
    }
}
