using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Media.Imaging;
using Game;
using System.Windows.Threading;

namespace moora_silverlight
{
    public partial class Page : UserControl
    {
        World world;
        private DispatcherTimer timer;
        DateTime last_time;

        public Page()
        {
            InitializeComponent();

            try
            {
                world = new World();
                world.StartDraw(MainForm);

                timer = new DispatcherTimer();
                timer.Tick += timer_Completed;
                //timer.Interval = TimeSpan.FromMilliseconds(1);
                last_time = DateTime.Now;
                timer.Start();
            }
            catch (Exception e)
            {
                string error = e.ToString();
            }
        }

        void timer_Completed(object sender, EventArgs e)
        {
            DateTime current_time = DateTime.Now;
            world.Update((float)(current_time - last_time).TotalSeconds);
            last_time = current_time;

            world.ReDraw();
        }

        private void MainForm_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            System.Windows.Input.MouseButtonEventArgs args = (System.Windows.Input.MouseButtonEventArgs)e;
            System.Windows.Point p = args.GetPosition(this);
            world.Click((int)p.X, (int)p.Y);
        }
    }
}
