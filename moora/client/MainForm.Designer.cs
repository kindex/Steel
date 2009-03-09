namespace moora_client
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.panel1 = new System.Windows.Forms.Panel();
            this.character_y = new System.Windows.Forms.Label();
            this.character_x = new System.Windows.Forms.Label();
            this.CloseButton = new System.Windows.Forms.Button();
            this.character_x_frac = new System.Windows.Forms.Label();
            this.character_y_frac = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.character_y_frac);
            this.panel1.Controls.Add(this.character_x_frac);
            this.panel1.Controls.Add(this.character_y);
            this.panel1.Controls.Add(this.character_x);
            this.panel1.Controls.Add(this.CloseButton);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel1.Location = new System.Drawing.Point(689, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(103, 573);
            this.panel1.TabIndex = 0;
            // 
            // character_y
            // 
            this.character_y.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.character_y.Location = new System.Drawing.Point(16, 27);
            this.character_y.Name = "character_y";
            this.character_y.Size = new System.Drawing.Size(56, 21);
            this.character_y.TabIndex = 2;
            this.character_y.Text = "YY";
            this.character_y.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // character_x
            // 
            this.character_x.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.character_x.Location = new System.Drawing.Point(16, 1);
            this.character_x.Name = "character_x";
            this.character_x.Size = new System.Drawing.Size(56, 24);
            this.character_x.TabIndex = 1;
            this.character_x.Text = "XX";
            this.character_x.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // CloseButton
            // 
            this.CloseButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CloseButton.Location = new System.Drawing.Point(16, 103);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(75, 23);
            this.CloseButton.TabIndex = 0;
            this.CloseButton.Text = "Close";
            this.CloseButton.UseVisualStyleBackColor = true;
            this.CloseButton.Click += new System.EventHandler(this.CloseButton_Click);
            this.CloseButton.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.CloseButton_KeyPress);
            // 
            // character_x_frac
            // 
            this.character_x_frac.AutoSize = true;
            this.character_x_frac.Location = new System.Drawing.Point(65, 9);
            this.character_x_frac.Name = "character_x_frac";
            this.character_x_frac.Size = new System.Drawing.Size(35, 13);
            this.character_x_frac.TabIndex = 3;
            this.character_x_frac.Text = "label1";
            // 
            // character_y_frac
            // 
            this.character_y_frac.AutoSize = true;
            this.character_y_frac.Location = new System.Drawing.Point(65, 35);
            this.character_y_frac.Name = "character_y_frac";
            this.character_y_frac.Size = new System.Drawing.Size(35, 13);
            this.character_y_frac.TabIndex = 4;
            this.character_y_frac.Text = "label1";
            // 
            // timer1
            // 
            this.timer1.Interval = 30;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(792, 573);
            this.Controls.Add(this.panel1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "Moora Client";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.MainForm_Paint);
            this.Closed += new System.EventHandler(this.Form1_Closed);
            this.Click += new System.EventHandler(this.splitContainer1_Panel1_Click);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.MainForm_KeyPress);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button CloseButton;
        private System.Windows.Forms.Label character_y;
        private System.Windows.Forms.Label character_x;
        private System.Windows.Forms.Label character_y_frac;
        private System.Windows.Forms.Label character_x_frac;
        private System.Windows.Forms.Timer timer1;

        #region Windows Form Designer generated code

        #endregion

    }
}

