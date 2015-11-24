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
            if (System.IO.File.Exists("res/default.jpg"))
                loadBackground("res/default.jpg");
        }

        
        private void loadBackground(string st) {
                Image image = Image.FromFile(st);
                pictureBox1.Image = image;
                pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
                pictureBox1.Height = image.Height;
                pictureBox1.Width = image.Width;

                this.Height = image.Height + 70;
                this.Width = image.Width ;

        }
        private void backgroundToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            if (openFileDialog_background.ShowDialog() == DialogResult.OK)
            {
                loadBackground(openFileDialog_background.FileName);
            }

        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            //Control control = (Control)sender;
            //pictureBox1.Height = ClientRectangle.Height - 100;
            //pictureBox1.Width = ClientRectangle.Width - 100;
        }
    }
}
