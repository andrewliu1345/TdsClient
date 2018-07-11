using ABC.Config;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using ABC.Printer;
using ABC.Printer.Decorator;
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
            try
            {
                LoadComboBox();
                LoadIni();
            }
            catch (System.Exception ex)
            {

                SysLog.e($"串口界面加载出错", ex);
            }

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
                if (!_SerialPortList.ContainsKey(item))
                {
                    _SerialPortList.Add(item, _sp);
                }

            }
            var list = _SerialPortList.OrderBy(o => o.Value);//排序
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
            string formName = "PB2926.xml";
            string data = "jymc=查询帐户注册信息#cph=MX11#rzh=554422121#jyrq=20110124/190309#zdh=M0204007#czy=鲜璐#fhy=黄涛#wdh=8041#zhjhm=210112196901250232#custname=崔补222#CardNum=3#$accno0=*5107#$eb0=1#$mb0=0#$cc0=0#$et0=0#$accno1=*6503#$eb1=1#$mb1=0#$cc1=0#$et1=0#$accno2=*8432#$eb2=0#$mb2=0#$cc2=1#$et2=0#";
            AbstractFormData load = new LoadFormData();
            load = new FormHeaderDecorator(load);
            load = new FormBottomDecorator(load);
            byte[] prdata = load.FormData(formName, data).ToByteArry();
            PrintApiHelper.Print_CHS(DeviceIDs.Print_fd, prdata, prdata.Length);
            byte[] numdata = "123".ToByteArry();
//             PrintApiHelper.PrintBarcode(DeviceIDs.Print_fd, 2, numdata, numdata.Length);
//             PrintApiHelper.Print_CHS(DeviceIDs.Print_fd, new byte[] { 0X0A }, 1);
            MessageBox.Show("打印完成");
        }
    }
}
