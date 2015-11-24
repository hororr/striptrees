using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace StripTreeEmu
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void backgroundToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            if (openFileDialog_background.ShowDialog() == DialogResult.OK)
            {
                Image image = Image.FromFile(openFileDialog_background.FileName);
                pictureBox1.Image = image;
                pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
                pictureBox1.Height = ClientRectangle.Height-100;
                pictureBox1.Width = ClientRectangle.Width-100;
                this.Controls.Add(pictureBox1);
            }

        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            //Control control = (Control)sender;
            pictureBox1.Height = ClientRectangle.Height - 100;
            pictureBox1.Width = ClientRectangle.Width - 100;
        }
    }
}
