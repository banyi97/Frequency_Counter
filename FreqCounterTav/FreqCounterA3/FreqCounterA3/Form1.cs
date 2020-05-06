using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FreqCounterA3
{
    public partial class Form1 : Form
    {
        private Timer timer;
        public Form1()
        {
            InitializeComponent();
            DoubleBuffered = true;
            this.timer = new Timer();
            this.timer.Interval = 100;
            
            this.timer.Tick += Timer_Tick;
            App.Initialize(this);
            this.timer.Start();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            if (App.Instance.SerialIsConnected)
            {
                this.sevenSegmentArray1.Value = App.Instance.Get7SegVal();
                this.button1.Text = "Disconnect";
                this.comboBox1.Enabled = false;
            }
            else
            {
                this.sevenSegmentArray1.Value = String.Empty;
                this.button1.Text = "Connect";
                this.comboBox1.Enabled = true;
            }
        }

        //Connect
        private void button1_Click(object sender, EventArgs e)
        {
            if (this.comboBox1.SelectedItem is null)
                return;
            if (App.Instance.SerialIsConnected)
            {
                App.Instance.Disconnect();
                this.button1.Text = "Connect";
                this.comboBox1.Enabled = true;
            }
            else
            {
                if (App.Instance.Connect(this.comboBox1.SelectedItem as string))
                {
                    this.button1.Text = "Disconnect";
                    this.comboBox1.Enabled = false;
                }
            }
        }

        //Send
        private void button2_Click(object sender, EventArgs e)
        {
            App.Instance.Send((short)this.numericUpDown1.Value);
        }

        private void comboBox1_DropDown(object sender, EventArgs e)
        {
            this.comboBox1.DataSource = SerialPort.GetPortNames();
        }
    }
}
