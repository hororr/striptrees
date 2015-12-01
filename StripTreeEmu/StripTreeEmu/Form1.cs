using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace StripTreeEmu
{


    public partial class Form1 : Form
    {
        private int ServerPort = 8081;
        private Thread thdUDPServer;
        private bool isRunning=false;

        private Point MouseDownLocation;
        private PictureBox currentPB;   // selected by left or right button

        private List<PictureBox> widgetList;

        Byte[] colors24;

        //Tuple<int, int> tupleWidget;
        public class savedWidget
        {
            public savedWidget() { }
            public savedWidget(int _x, int _y, int _nr)
            {
                x = _x;
                y = _y;
                nr = _nr;
            }
            public int x { get; set; }
            public int y { get; set; }
            public int nr { get; set; }
        }
        private void saveToXML( string fileName) {
            XElement xml = new XElement("Widgets",
                                            from PictureBox in widgetList
                                            select new XElement("Widget",
                                                new XElement("nr", (int)PictureBox.Tag),
                                                new XElement("x", PictureBox.Left),
                                                 new XElement("y", PictureBox.Top))

                                       );
            xml.Save(fileName);
           
        }

        private void loadFromXML(string fileName) {
            XElement xml = XElement.Load(fileName);
            savedWidget[]  widgets = (from savedWidget in xml.Elements("Widget")
                                 select new savedWidget
                                         {
                                             x = (int)savedWidget.Element("x"),
                                             y = (int)savedWidget.Element("y"),
                                             nr = (int)savedWidget.Element("nr")
                                         })
                                        .ToArray();

            foreach (var item in widgets)
            {
                addWidget(item.x, item.y, item.nr);
            }
        }

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

        public void setWidgetsVisibility(bool visible) {
            

            foreach (PictureBox item in widgetList)
                {
                    if (visible)
                        item.Show();
                    else
                        item.Hide();
                }
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
            if (isRunning)
                return;
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

        private void saveToolStripMenuItem1_Click(object sender, EventArgs e)
        {

        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {

               
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialog_W.ShowDialog();
            if (result == DialogResult.OK) // Test result.
            {
                saveToXML(saveFileDialog_W.FileName);
            }
        }

        private void loadFromToolStripMenuItem_Click(object sender, EventArgs e)
        {
            removeAllWidget();
            DialogResult result = openFileDialog_W.ShowDialog();
            if (result == DialogResult.OK) // Test result.
            {
                loadFromXML(openFileDialog_W.FileName);
            }
        }

        private void layoutToolStripMenuItem1_Click(object sender, EventArgs e)
        {

        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveToXML("widgets.xml");
        }

        private void loadToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            removeAllWidget();
            loadFromXML("widgets.xml");
        }

        private void StartServer()
        {
            isRunning = true;
            layoutToolStripMenuItem.Enabled = false;
            startToolStripMenuItem1.Enabled = false;

            stopToolStripMenuItem1.Enabled = true;

            setWidgetsVisibility(false);
            colors24 = new Byte[widgetList.Count*3];
            for (int i = 0; i < widgetList.Count*3; i++)
            {
                colors24[i] = 0;
            }
        }

        private void StopServer()
        {
            isRunning = false;
            layoutToolStripMenuItem.Enabled = true;
            startToolStripMenuItem1.Enabled = true;

            stopToolStripMenuItem1.Enabled = false;

            setWidgetsVisibility(true);

            
        }

        private void startToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            StartServer();
        }


        // network part
        public void serverThread()
        {
            UdpClient udpClient = new UdpClient(ServerPort);
            try
            {
                while (true)
                {
                    IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);
                    Byte[] receiveBytes = udpClient.Receive(ref RemoteIpEndPoint);
                    
                    //string returnData = Encoding.ASCII.GetString(receiveBytes);
                    //lbConnections.Items.Add(RemoteIpEndPoint.Address.ToString() + ":" + returnData.ToString());
                }
            }
            catch (ThreadInterruptedException exception)
            {
                /* Clean up. */
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //start udp server
            thdUDPServer = new Thread(new ThreadStart(serverThread));
            thdUDPServer.IsBackground = true;
            thdUDPServer.Start();
            
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {

            if (isRunning)
            {
                
                foreach (PictureBox item in widgetList)
                {
                    //colors24  
                    //itn i=colors24[item.Tag * 3 + 0];
                    //Color.FromArgb(255, 0, 0)
                    int nr=(int)item.Tag;
                    Color c = Color.FromArgb(colors24[nr], colors24[nr + 1], colors24[nr + 2]);
                    e.Graphics.FillEllipse(new System.Drawing.SolidBrush(c), new Rectangle(item.Left, item.Top, item.Width, item.Height));
                }

            }

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            
        }

        private void stopToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            StopServer();
        }

    }
}
