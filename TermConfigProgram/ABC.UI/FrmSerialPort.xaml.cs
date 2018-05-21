using ABC.Config;
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
        private void LoadIni()
        {
            cbbBS.SelectedValue = IniFileRead.Instance.BackSplint.Com;
            cbbPrinter.SelectedValue = IniFileRead.Instance.Printer.Com;
        }
        private void SaveIni()
        {
            if (cbbBS.SelectedValue != null)
                IniFileRead.Instance.BackSplint.Com = (int)cbbBS.SelectedValue;

            if (cbbPrinter.SelectedValue != null)
                IniFileRead.Instance.Printer.Com = (int)cbbPrinter.SelectedValue;
        }
        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            LoadComboBox();
            LoadIni();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            SaveIni();
        }
    }
}
