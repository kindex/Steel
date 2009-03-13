using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Game
{
    class FileMap : Map
    {
        public FileMap(Terrain terrain, List<string> file)
            : base(terrain)
        {
            ParseFile(file);
        }

        void ParseFile(List<string> file)
        {
            int y = file.Count;
            int x = 0;
            foreach (string s in file)
            {
                x = System.Math.Max(x, s.Length);
            }

            _map = new Cell[x, y];
            for (int j = 0; j < y; j++)
            {
                for (int i = 0; i < x; i++)
                {
                    char c;
                    if (i < file[j].Length)
                    {
                        c = file[j][i];
                    }
                    else
                    {
                        c = ' ';
                    }

                    TerrainType t = Terrain.CharToTerrain(c);
                    SetTerrain(i, j, t);
                }
            }
        }
    }
}
