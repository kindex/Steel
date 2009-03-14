using System;
using System.Net;
using System.Collections.Generic;

namespace Game
{
    public static class ImageLibrary
    {
#if SILVERLIGHT
        static Dictionary<string, System.Windows.Media.Imaging.BitmapImage> images = new Dictionary<string, System.Windows.Media.Imaging.BitmapImage>();

        static public System.Windows.Media.Imaging.BitmapImage getImage(string name)
        {
            if (images.ContainsKey(name))
            {
                return images[name];
            }
            else
            {
                System.Windows.Media.Imaging.BitmapImage myBitmapImage = new System.Windows.Media.Imaging.BitmapImage();
                myBitmapImage.UriSource = new Uri(name);
                return images[name] = myBitmapImage;
            }
        }
#else
        static Dictionary<string, System.Drawing.Bitmap> images = new Dictionary<string, System.Drawing.Bitmap>();

        static public System.Drawing.Bitmap getImage(string name)
        {
            if (images.ContainsKey(name))
            {
                return images[name];
            }
            else
            {
                System.Drawing.Bitmap myBitmapImage = new System.Drawing.Bitmap(name);
                return images[name] = myBitmapImage;
            }
        }
#endif
    }
}
