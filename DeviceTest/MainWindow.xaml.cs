using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace DeviceTest
{
    public delegate void MessageDelegate(string msg);
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        int deiceID = -1;
        int deiceIDPrin = -1;
        string[] _SerialPortS;
        Dictionary<string, int> _SerialPortList = new Dictionary<string, int>();

        public MainWindow()
        {
            InitializeComponent();
            LoadComboBox();

        }
        private void LoadComboBox()
        {
            _SerialPortS = SerialPort.GetPortNames();
            _SerialPortList.Clear();
            _SerialPortList.Add("USB", 0);
            foreach (var item in _SerialPortS)
            {
                string _ssp = item.ToUpper().Replace("COM", "");
                int _sp = int.Parse(_ssp);
                _SerialPortList.Add(item, _sp);
            }
            var list = _SerialPortList.OrderBy(o => o.Value);
            //  cbbSerialPort.Items.Clear();
            cbbSerialPort.ItemsSource = list;
            cbbSerialPort.DisplayMemberPath = "Key";
            cbbSerialPort.SelectedValuePath = "Value";
            cbbSerialPort.SelectedIndex = 0;

            cbbSerialPortPrint.ItemsSource = list;
            cbbSerialPortPrint.DisplayMemberPath = "Key";
            cbbSerialPortPrint.SelectedValuePath = "Value";
            cbbSerialPortPrint.SelectedIndex = 0;
        }

        private void btnConn_Click(object sender, RoutedEventArgs e)
        {

            string portname = cbbSerialPort.Text;
            int port = (int)cbbSerialPort.SelectedValue - 1;
            if (deiceID > 0)
            {
                int iRet = Api.device_close(deiceID);
                deiceID = -1;
            }
            if (port < 0)
            {
                deiceID = Api.device_open_name("USB", 0, 115200);
            }
            else
            {
                deiceID = Api.device_open(port, 115200);
            }

            if (deiceID > 0)
            {
                var msg = string.Format("端口:{0},连接成功。iRet={1}", port, deiceID);
                MessageBox.Show(msg);
            }
            else
            {
                var msg = string.Format("端口:{0},连接失败。iRet={1}", port, deiceID);
                MessageBox.Show(msg);
            }
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            int iRet = Api.device_close(deiceID);
        }

        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            LoadComboBox();
        }

        private void btnGetStatus_Click(object sender, RoutedEventArgs e)
        {
            byte[] buffer = new byte[1];
            int iRet = Api.get_device_status(deiceID, ref buffer[0]);
            if (iRet == 0)
            {
                string msg = string.Format("设备正常，iRet={0},buffer={1}", iRet, (int)buffer[0]);
                MessageBox.Show(msg);
            }
            else
            {
                string msg = string.Format("设备未连接，iRet={0},buffer={1}", iRet, (int)buffer[0]);
                MessageBox.Show(msg);

            }
        }

        private void btnSynRCT_Click(object sender, RoutedEventArgs e)
        {
            int iRet = Api.Set_RCT_Timer(deiceID);
            if (iRet == 0)
            {
                string msg = string.Format("同步成功，iRet={0}", iRet);
                MessageBox.Show(msg);
            }
            else
            {
                string msg = string.Format("同步失败，iRet={0}", iRet);
                MessageBox.Show(msg);

            }
        }

        private void btnConntPrint_Click(object sender, RoutedEventArgs e)
        {
            string portname = cbbSerialPortPrint.Text;
            int port = (int)cbbSerialPortPrint.SelectedValue;
            if (deiceIDPrin > 0)
            {
                int iRet = Api.device_close_print(deiceIDPrin);
                deiceIDPrin = -1;
            }

            deiceIDPrin = Api.device_open_print(port, 115200);


            if (deiceIDPrin > 0)
            {
                var msg = string.Format("端口:{0},连接成功。iRet={1}", port, deiceIDPrin);
                MessageBox.Show(msg);
            }
            else
            {
                var msg = string.Format("端口:{0},连接失败。iRet={1}", port, deiceIDPrin);
                MessageBox.Show(msg);
            }
        }

        private void btnClosePrint_Click(object sender, RoutedEventArgs e)
        {
            int iRet = Api.device_close_print(deiceIDPrin);
        }

        private void btnPrint_Click(object sender, RoutedEventArgs e)
        {
            string str = txtPrintInfo.Text;
            int iRet = Api.Print_CHS(deiceIDPrin, str, str.Length);
            iRet = Api.Print_CHS(deiceIDPrin, "\n", 1);
            if (deiceIDPrin > 0)
            {
                var msg = string.Format("端口:{0},打印成功。iRet={1}", deiceIDPrin, iRet);
                MessageBox.Show(msg);
            }
            else
            {
                var msg = string.Format("端口:{0},打印失败。iRet={1}", deiceIDPrin, iRet);
                MessageBox.Show(msg);
            }
        }
    }
}
