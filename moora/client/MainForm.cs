using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace moora_client
{
    public partial class MainForm : Form
    {
        private Bitmap DrawArea;  // make a persistent drawing area

        // Required designer variable
        //private System.ComponentModel.Container components = null;

        private Random rnd = new Random((int)DateTime.Now.Ticks);
        private Pen myPen = new Pen(Color.Red);

        public MainForm()
        {
            InitializeComponent();
        }

        //private void InitializeComponent()
        //{
        //    this.SuspendLayout();
        //    // 
        //    // MainForm
        //    // 
        //    this.ClientSize = new System.Drawing.Size(292, 273);
        //    this.Name = "MainForm";
        //    this.Text = "Moora";
        //    this.Load += new System.EventHandler(this.MainForm_Load);
        //    this.ResumeLayout(false);

        //}


        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.CloseButton = new System.Windows.Forms.Button();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.splitContainer1_Panel1_Paint);
            this.splitContainer1.Panel1.Resize += new System.EventHandler(this.splitContainer1_Panel1_Resize);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.CloseButton);
            this.splitContainer1.Size = new System.Drawing.Size(792, 573);
            this.splitContainer1.SplitterDistance = 690;
            this.splitContainer1.TabIndex = 0;
            // 
            // CloseButton
            // 
            this.CloseButton.Location = new System.Drawing.Point(11, 12);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(75, 23);
            this.CloseButton.TabIndex = 0;
            this.CloseButton.Text = "Close";
            this.CloseButton.UseVisualStyleBackColor = true;
            this.CloseButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // MainForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(792, 573);
            this.Controls.Add(this.splitContainer1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "Moora Client";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Form1_Paint);
            this.Closed += new System.EventHandler(this.Form1_Closed);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.MainForm_KeyPress);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        // free up resources on program exit
        private void Form1_Closed(object sender, System.EventArgs e)
        {
            DrawArea.Dispose();
        }

        // paint event
        private void Form1_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            Graphics xGraph;

            xGraph = e.Graphics;
            xGraph.DrawImage(DrawArea, 0, 0, DrawArea.Width, DrawArea.Height);
            xGraph.Dispose();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            DrawArea = new Bitmap(
                this.splitContainer1.Panel1.Width,
                this.splitContainer1.Panel1.Height,
                System.Drawing.Imaging.PixelFormat.Format24bppRgb);

            Graphics xGraph;
            int k;
            int r;     // radius of circle
            int x, y;  // center coordinates of circle

            xGraph = Graphics.FromImage(DrawArea);

            // Create solid brush.
            SolidBrush Brush = new SolidBrush(Color.Red);
            for (k = 1; k < 40; k++)
            {
                // radius for circle, max 1/2 the width of the form
                r = rnd.Next(0, (this.Width / 2));
                x = rnd.Next(0, this.Width);
                y = rnd.Next(0, this.Height);

                Brush.Color = Color.FromArgb(
                  (rnd.Next(0, 255)),
                  (rnd.Next(0, 255)),
                  (rnd.Next(0, 255)));
                // convert centerX, centerY, radius to bounding rectangle
                xGraph.FillEllipse(Brush, x - r, y - r, r, r);
            }
            xGraph.Dispose();

            this.Invalidate();
        }

        private void MainForm_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 27)
            {
                Close();
            }
        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {
            Graphics xGraph;

            xGraph = e.Graphics;
            xGraph.DrawImage(DrawArea, 0, 0, DrawArea.Width, DrawArea.Height);
            xGraph.Dispose();
        }

        private void splitContainer1_Panel1_Resize(object sender, EventArgs e)
        {
            MainForm_Load(sender, e);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
