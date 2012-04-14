using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Game.Res
{
    static class TextFile
    {
        public static List<string> GetTextFile(string file)
        {
            foreach (string path in SearchPath.Paths)
            {
                try
                {
                    List<string> result = new List<string>();
                    using (StreamReader SR = new StreamReader(path + "/" + file))
                    {
                        for (; ; )
                        {
                            string s = SR.ReadLine();
                            if (s == null)
                            {
                                break;
                            }
                            result.Add(s);
                        }
                    }
                    return result;
                }
                catch (System.IO.DirectoryNotFoundException)
                {
                }
            }
            return null;
        }
    }
}
