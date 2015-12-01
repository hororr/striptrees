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

    public partial class Form_Generate : Form
    {
        public int StartX, StartY, MyWidth, MyHeight, Nr;
        public Form_Generate()
        {
            InitializeComponent();

        }

        public void SetData()
        {

            textBox1.Text = StartX.ToString();
            textBox2.Text = StartY.ToString();
            textBox3.Text = MyWidth.ToString();
            textBox4.Text = MyHeight.ToString();
            textBox5.Text = Nr.ToString();

        }
        private void button1_Click(object sender, EventArgs e)
        {
            StartX = Int32.Parse(textBox1.Text);
            StartY = Int32.Parse(textBox2.Text);
            MyWidth = Int32.Parse(textBox3.Text);
            MyHeight = Int32.Parse(textBox4.Text);
            Nr = Int32.Parse(textBox5.Text);

            Close();

        }
    }
}
