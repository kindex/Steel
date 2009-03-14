using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Game.Res
{
    static class SearchPath
    {
        static List<string> paths = new List<string>();
        static public List<string> Paths { get { return paths; } }
        public static void addPath(string path)
        {
            paths.Add(path);
        }
    }
}
