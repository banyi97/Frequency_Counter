using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FreqCounterA3
{
    public class App
    {
        private Form mainForm;
        private static App theApp;
        private byte startFrame = 0xAA;
        private SerialPort serialPort = null;
        public bool SerialIsConnected { get => serialPort is SerialPort && serialPort.IsOpen;  }
        private char[] segs = new char[4] { ' ', ' ', ' ', ' ' };
        public static App Instance
        {
            get { return theApp; }
        }

        public Control AppPanel
        {
            get { return mainForm; }
        }

        public static void Initialize(Form form)
        {
            theApp = new App();
            theApp.mainForm = form;
        }

        public bool Connect(string port)
        {
            try
            {
                this.serialPort = new SerialPort(port, 115200, Parity.None, 8, StopBits.One);
                this.serialPort.Open();
                serialPort.DataReceived += SerialPort_DataReceived;
                return true;
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
            return false;
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int count = serialPort.BytesToRead;
            byte[] byteArray = new byte[count];
            serialPort.Read(byteArray, 0, count);
            if (byteArray.Length < 3 || byteArray[2] != this.Gencrc(byteArray.Take(2)))
                return;
            char c = (char)((byteArray[1] & 0x0F) + 48);
            if (c < '0' || c > '9')
                return;
            switch (byteArray[1] & 0xF0)
            {
                case 0x10:
                    segs[3] = c;
                    break;
                case 0x20:
                    segs[2] = c;
                    break;
                case 0x40:
                    segs[1] = c;
                    break;
                case 0x80:
                    segs[0] = c;
                    break;
                default:
                    break;
            }
        }

        public void Disconnect()
        {
            serialPort.DataReceived -= SerialPort_DataReceived;
            this.serialPort.Close();
            this.serialPort = null;
        }

        public void Send(short val)
        {
            if (this.SerialIsConnected)
            {
                using (MemoryStream memStream = new MemoryStream(10))
                using (BinaryWriter writer = new BinaryWriter(memStream))
                {
                    writer.Write(this.startFrame);
                    writer.Write(val);
                    writer.Write(this.Gencrc(memStream.ToArray()));
                    var buff = memStream.ToArray();
                    serialPort.Write(buff, 0, buff.Length);
                }
            }
        }

        public string Get7SegVal()
        {
            if (this.SerialIsConnected)
            {
                var bilder = new StringBuilder();
                foreach (var item in segs)
                {
                    bilder.Append(item);
                }
                return bilder.ToString();
            }
            return "";
        }

        private byte Gencrc(IEnumerable<byte> data)
        {
            byte crc = 0xff;
            foreach (var item in data)
            {
                crc ^= item;
                for (int j = 0; j < 8; j++)
                {
                    if ((crc & 0x80) != 0)
                        crc = (byte)((crc << 1) ^ 0x31);
                    else
                        crc <<= 1;
                }
            }
            if(crc == this.startFrame)
                crc ^= 0xFF;
            return crc;
        }
    }
}
