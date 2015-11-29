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
        private Point MouseDownLocation;
        private PictureBox currentPB;   // selected by left or right button

        private List<PictureBox> widgetList;


        private int getHighestNumberOfList() {
            int retVal=0;
            bool found;
            for (int i = 1; i < Int32.MaxValue ; i++)
			{
                found = false;
                foreach (PictureBox item in widgetList)
                {
                    if (i == (int)item.Tag) {
                        retVal = i;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return retVal;
			}
            return retVal;
        }

        public Form1()
        {
            InitializeComponent();
            if (System.IO.File.Exists("res/default.jpg"))
                loadBackground("res/default.jpg");

            widgetList = new List<PictureBox>();



        }

        private void removeAllWidget() {
            foreach (PictureBox item in widgetList)
            {
                item.Hide();
            }
            widgetList.Clear();
        }
        private void removeWidget(PictureBox pb) {
            foreach (PictureBox item in widgetList)
            {
                if (item == pb)
                {
                    widgetList.Remove(item);
                    pb.Hide();
                    break;
                }
            }
        }

        private void addWidget(int Ex, int Ey, int tag) {
            Image img = Image.FromFile("res/point.png" );

            PictureBox pb = new PictureBox();
            pb.Image = img;
            pb.Left = Ex-img.Width/2;
            pb.Top = Ey-img.Height/2;
            pb.Width = img.Width;
            pb.Height = img.Height;
            pb.Parent = pictureBox1;
            pb.BackColor = Color.Transparent;
            pb.MouseDown += new System.Windows.Forms.MouseEventHandler(widget_MouseDown);
            pb.MouseMove += new System.Windows.Forms.MouseEventHandler(widget_MouseMove);
            pb.Tag = tag;
            //pb.ContextMenu = contextMenuStrip1;
            pb.ContextMenuStrip = contextMenuStrip1;

            //write number?
            if (img.Width >= 20)
            {
                Label nr = new Label();
                nr.Text = tag.ToString();
                nr.Font = new Font(nr.Font.FontFamily, 6);
                nr.Parent = pb;
                nr.Left += 1;
                nr.Top += 5;
                nr.MouseDown += new System.Windows.Forms.MouseEventHandler(widget_MouseDown);
                nr.MouseMove += new System.Windows.Forms.MouseEventHandler(widget_MouseMove);
                //nr.ContextMenu = contextMenuStrip1;
                nr.ContextMenuStrip = contextMenuStrip1;
            }
            widgetList.Add(pb);

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

        }

        private void addPointsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            addWidget(100, 100, 1 );

        }
        private void widget_MouseDown(object sender, MouseEventArgs e)
        {
            var pb = sender as PictureBox;
            
            if (pb == null)
                pb = (PictureBox)((Label)sender).Parent;

            currentPB = pb;
            if ( 
                (e).Button == System.Windows.Forms.MouseButtons.Left ||
                (e).Button == System.Windows.Forms.MouseButtons.Right)
            {
                MouseDownLocation = e.Location;
            }
        }
        private void widget_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                currentPB.Left = e.X + currentPB.Left - MouseDownLocation.X;
                currentPB.Top = e.Y + currentPB.Top - MouseDownLocation.Y;
            }
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if ( (e).Button == System.Windows.Forms.MouseButtons.Left)
            {
                addWidget(e.X, e.Y, getHighestNumberOfList() + 1);
            }
         
        }

        private void contextMenuStrip1_Opening_1(object sender, CancelEventArgs e)
        {
            ContextMenuStrip strip = (ContextMenuStrip)sender;
            strip.Items[0].Text = ((int)currentPB.Tag).ToString();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            removeWidget(currentPB);
        }

        private void addNewBackgroundToolStripMenuItem_Click(object sender, EventArgs e)
        {
            removeAllWidget();
        }


    }
}
