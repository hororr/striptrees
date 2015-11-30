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
        private int sX, sY, sW, sH, sN;
        private int imageWidth; // loaded picture's width /res/point.png
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

            sX = 500;
            sY = 500;
            sH = 200;
            sW = 200;
            sN = 100;

        }

        private void removeAllWidget() {
            foreach (PictureBox item in widgetList)
            {
                item.Hide();
                item.MouseDown -= widget_MouseDown;
                item.MouseMove -= widget_MouseMove;
                item.ContextMenuStrip = null;

                if (enoughBigToFitLabel())
                {
                    Label l = (Label)item.Controls.OfType<Label>();
                    l.MouseDown -= widget_MouseDown;
                    l.MouseMove -= widget_MouseMove;
                    l.ContextMenuStrip = null;
                }
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
                    pb.MouseDown -= widget_MouseDown;
                    pb.MouseMove -= widget_MouseMove;
                    pb.ContextMenuStrip = null;

                    if (enoughBigToFitLabel()) {
                        Label l = (Label)pb.Controls.OfType<Label>();
                        l.MouseDown -= widget_MouseDown;
                        l.MouseMove -= widget_MouseMove;
                        l.ContextMenuStrip = null;
                    }
                    break;
                }
            }
        }

        private bool enoughBigToFitLabel()
        {
            if (imageWidth >= 20)
                return true;
            return false;
        }
        private void addWidget(int Ex, int Ey, int tag) {
            Image img = Image.FromFile("res/point.png" );
            imageWidth = img.Width;

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
            if (enoughBigToFitLabel() )
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

        private void generateTree(int sx, int sy, int width, int height, int nr)
        {
            int[] x = new int[nr];
            int[] y = new int[nr];
            
            double xs = 0.95 * width / 2;
            double ys = 0.45 * xs;
            float xc = width / 2;
            float yc = height - height / 4;

            removeAllWidget();
            for (int i = 0; i < nr; i++)
            {
                float t = ((float)i) / (float)nr;
                x[i] = (int)(xc + xs * (1 - t) * Math.Sin(6 * t * 2.0 * Math.PI)) + sx;
                y[i] = (int)(yc + ys * (1 - t) * Math.Cos(6 * t * 2.0 * Math.PI) - (height / 2) * t) + sy;
                addWidget(x[i], y[i], i);
            }

            

        }

        private void generateTreeToolStripMenuItem_Click(object sender, EventArgs e)
        {
 
            //Form_Generate.ShowDialog();
            Form_Generate f = new Form_Generate();
            
            /*
            f.StartX = pictureBox1.Width / 2;
            f.StartY = pictureBox1.Height - 20;
            f.Width =  (int) ( (float)pictureBox1.Width * (float)0.5 );
            f.Height = (int) ( (float)pictureBox1.Height *(float) 0.7 );
            f.Nr = 100;
             * */

            f.StartX = sX;
            f.StartY = sY;
            f.MyWidth = sW;
            f.MyHeight = sH;
            f.Nr = sN;


            f.SetData();
            
            f.ShowDialog();
             
            generateTree(
                f.StartX,
                f.StartY,
                f.MyWidth,
                f.MyHeight,
                f.Nr
                );


            sX = f.StartX ;
            sY = f.StartY ;
            sW = f.MyWidth ;
            sH = f.MyHeight ;
            sN = f.Nr ;

        }


    }
}
