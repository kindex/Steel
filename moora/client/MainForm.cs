using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using moora_client.Game;
using System.Drawing.Drawing2D;

namespace moora_client
{
    public partial class MainForm : Form
    {
        private Random rnd = new Random((int)DateTime.Now.Ticks);
        private Pen myPen = new Pen(Color.Red);
        World world;

        public MainForm()
        {
            world = new World();

            InitializeComponent();
        }

        // free up resources on program exit
        private void Form1_Closed(object sender, System.EventArgs e)
        {
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.Invalidate();
        }

        private void MainForm_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 27)
            {
                Close();
            }
        }

        private void splitContainer1_Panel1_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.MouseEventArgs args = (System.Windows.Forms.MouseEventArgs)e;
            world.Click(args.X, args.Y);
            this.Invalidate(true);
        }

        private void RedrawButton_Click(object sender, EventArgs e)
        {
            this.Invalidate(true);
        }

        private void MainForm_Paint(object sender, PaintEventArgs e)
        {
            world.Draw(e.Graphics);
            float x = world.character.X;
            float y = world.character.Y;
            character_x.Text = ((int)x).ToString();
            character_y.Text = ((int)y).ToString();
            character_x_frac.Text = "." + ((int)(100.0f * (x - (int)x))).ToString();
            character_y_frac.Text = "." + ((int)(100.0f * (y - (int)y))).ToString();
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void CloseButton_KeyPress(object sender, KeyPressEventArgs e)
        {
            MainForm_KeyPress(sender, e);
        }

        private void character_y_frac_Click(object sender, EventArgs e)
        {

        }
    }
}
