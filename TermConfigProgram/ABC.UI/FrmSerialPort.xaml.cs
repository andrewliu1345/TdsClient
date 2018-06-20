using ABC.Config;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Printer;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Windows;

namespace ABC.UI
{
    /// <summary>
    /// FrmSerialPort.xaml 的交互逻辑
    /// </summary>
    public partial class FrmSerialPort : Window
    {

        string[] _SerialPortS;
        Dictionary<string, int> _SerialPortList = new Dictionary<string, int>();
        public FrmSerialPort()
        {
            InitializeComponent();
            LoadComboBox();
            LoadIni();
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
            cbbBS.ItemsSource = list;
            cbbBS.DisplayMemberPath = "Key";
            cbbBS.SelectedValuePath = "Value";
            cbbBS.SelectedIndex = 0;

            cbbPrinter.ItemsSource = list;
            cbbPrinter.DisplayMemberPath = "Key";
            cbbPrinter.SelectedValuePath = "Value";
            cbbPrinter.SelectedIndex = 0;
        }
        BackSplintClass backSplint = AppConfig.Instance.BackSplint;
        PrinterClass printer = AppConfig.Instance.Printer;

       

        private void LoadIni()
        {

            cbbBS.SelectedValue = backSplint.Com;
            cbbPrinter.SelectedValue = printer.Com;
        }
        private void SaveIni()
        {
            if (cbbBS.SelectedValue != null)
            {
                backSplint.Com = (int)cbbBS.SelectedValue;
                backSplint.Baud = 112500;
                backSplint.Save();
            }


            if (cbbPrinter.SelectedValue != null)
            {
                printer.Com = (int)cbbPrinter.SelectedValue;
                printer.Baud = 115200;
                printer.Save();
            }
            MessageBox.Show("保存成功");
        }
        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            LoadComboBox();
            LoadIni();
            MessageBox.Show("刷新成功");
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            SaveIni();
        }

        private void btnPrinter_Click(object sender, RoutedEventArgs e)
        {
            string formName = "PB33652.xml";
            string data = "jymc=短信服务注册#cph=#rzh= 326650753#jyrq=20171013-143119#zdh=050LM001#czy=刘寿丽#fhy=高丽#zhjlx=身份证#zhjhm=12010919670108003X#khmc=罗跃亭#qdje=10#jfms=包月#xsye=显示余额#";
            LoadFormData load = new LoadFormData();
            byte [] prdata= load.FormData(formName, data);
            PrintApiHelper.Print_CHS(DeviceIDs.Print_fd, prdata, prdata.Length);
            byte[] numdata = "123".ToByteArry();
            PrintApiHelper.PrintBarcode(DeviceIDs.Print_fd, 2, numdata, numdata.Length);
            PrintApiHelper.Print_CHS(DeviceIDs.Print_fd,new byte[] { 0X0A },1);
            MessageBox.Show("打印完成");
        }
    }
}
