using ABC.Background;
using ABC.BackSplint;
using ABC.Config;
using ABC.Printer;
using System.Windows;

namespace ABC.UI
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        public App()
        {
            BluetoothBs.Instance.Start(AppConfig.Instance.BackSplint);//连接背夹蓝牙
            BluetoothPrinter.Instance.Start(AppConfig.Instance.Printer);//连接打印机蓝牙
            CommServer.Instance.Start();
        }
    }
}
