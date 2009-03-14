using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
#if SILVERLIGHT
using System.Windows.Controls;
using System.Windows.Media;
#endif

namespace Game
{
    public class World
    {
        public Terrain terrain;
        public Map map;
        public Character character;
        public static readonly int x_size = 32;
        public static readonly int y_size = 32;

        public World()
        {
            Random rnd = new Random();
            
#if SILVERLIGHT
            dirs.Add("file://K:/Projects/steel/moora/Resources/terrain");
//            dirs.Add("http://division.kindex.lv/moora/terrain");
#else
            Res.SearchPath.addPath("../../../Resources");
            Res.SearchPath.addPath("Resources");
#endif
            terrain = new Terrain(rnd);
//            map = new RandomMap(21, 18, terrain, rnd);

            List<string> file = Res.TextFile.GetTextFile("map.txt");

            map = new FileMap(terrain, file);

            int char_x;
            int char_y;

            int try_number = 0;
            do
            {
                char_x = rnd.Next(0, map.X);
                char_y = rnd.Next(0, map.Y);
                if (try_number++ > 1000)
                {
                    break;
                }
            }
            while (map.getTerrain(char_x, char_y) != TerrainType.Sand);

            character = new Character(char_x, char_y, map);
        }

        public void Click(int x, int y)
        {
            character.MoveTo((float)x / x_size, (float)y / y_size);
        }

        public void Update(float deltaTime)
        {
            character.Update(deltaTime);
        }

#if SILVERLIGHT
        public void StartDraw(Canvas canvas)
        {
            canvas.Children.Clear();

            for (int x = 0; x < map.X; x++)
            {
                for (int y = 0; y < map.Y; y++)
                {
                    string image = map.GetImage(x, y);
                    Image img = new Image();
                    img.Source = ImageLibrary.getImage(image);

                    TranslateTransform t = new TranslateTransform();
                    t.X = x * x_size;
                    t.Y = y * y_size;

                    img.RenderTransform = t;

                    canvas.Children.Add(img);
                }
            }
            character.StartDraw(canvas);
        }

        public void ReDraw()
        {
            character.ReDraw();
        }

#else
        public void Draw(System.Drawing.Graphics graph)
        {
            for (int x = 0; x < map.X; x++)
            {
                for (int y = 0; y < map.Y; y++)
                {
                    string image = map.GetImage(x, y);
                    if (image != null)
                    {
                        System.Drawing.Bitmap bitmap = ImageLibrary.getImage(image);
                        if (bitmap != null)
                        {
                            graph.DrawImage(bitmap, new System.Drawing.Point(x * x_size, y * y_size));
                        }
                    }
                }
            }
            character.Draw(graph);
        }
#endif
    }
}
