using ABC.Background;
using ABC.BackSplint;
using ABC.Config;
using ABC.HelperClass;
using ABC.Logs;
using ABC.Printer;
using System;
using System.Windows;
using System.Windows.Forms;

namespace ABC.UI
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {

        private NotifyIcon notifyIcon;
        public MainWindow()
        {
            //this.ShowInTaskbar = false;
            InitializeComponent();
            SysLog.d("InitializeComponent");
            SetWindowLocation();//程序初始位置

            SetNotifyIcon();//设置托盘图标

            hideMethod();//隐藏界面
            //int i = Class1.xx();
        }
        protected override void OnClosed(EventArgs e)
        {
            notifyIcon.Visible = false;
            CommServer.Instance.Stop();
            BluetoothBs.Instance.Stop();
            BluetoothPrinter.Instance.Stop();
            base.OnClosed(e);
        }
        /// <summary>
        /// 程序初始位置
        /// </summary>
        private void SetWindowLocation()
        {
            double primaryScreenWidth = SystemParameters.PrimaryScreenWidth;
            double primaryScreenHeight = SystemParameters.PrimaryScreenHeight;
            double num;
            if (primaryScreenWidth % base.Width == 0.0)
            {
                num = primaryScreenWidth / base.Width - 1.0;
            }
            else if (primaryScreenWidth / base.Width > 1.0)
            {
                num = primaryScreenWidth / base.Width - 1.0;
            }
            else
            {
                num = primaryScreenWidth / base.Width;
            }
            double num2;
            if (primaryScreenHeight % base.Height == 0.0)
            {
                num2 = primaryScreenHeight / base.Height - 1.0;
            }
            else if (primaryScreenHeight / base.Height > 1.0)
            {
                num2 = primaryScreenHeight / base.Height - 1.0;
            }
            else
            {
                num2 = primaryScreenHeight / base.Height;
            }
            if (primaryScreenWidth < base.Width)
            {
                base.Left = 50.0;
            }
            else
            {
                base.Left = num * base.Width - 50.0;
            }
            if (primaryScreenHeight < base.Height)
            {
                base.Top = 50.0;
            }
            else
            {
                base.Top = num2 * base.Height - 50.0;
            }
            SysLog.d("SetWindowLocation");
        }
        /// <summary>
        /// 设置托盘图标
        /// </summary>
        private void SetNotifyIcon()
        {
            this.notifyIcon = new NotifyIcon();
            this.notifyIcon.BalloonTipText = "终端配置程序";
            this.notifyIcon.ShowBalloonTip(2000);
            this.notifyIcon.Text = "终端配置程序";
            this.notifyIcon.Icon = Properties.Resources.Logo;

            this.notifyIcon.Visible = true;
            System.Windows.Forms.MenuItem menuItem = new System.Windows.Forms.MenuItem("显示");
            menuItem.Click += show_Click;
            System.Windows.Forms.MenuItem menuItem2 = new System.Windows.Forms.MenuItem("隐藏");
            menuItem2.Click += hide_Click;
            System.Windows.Forms.MenuItem menuItem3 = new System.Windows.Forms.MenuItem("退出");
            menuItem3.Click += exit_Click;
            System.Windows.Forms.MenuItem[] menuItems = new System.Windows.Forms.MenuItem[]
            {
                menuItem,
                menuItem2,
                menuItem3
            };
            this.notifyIcon.ContextMenu = new System.Windows.Forms.ContextMenu(menuItems);
            this.notifyIcon.MouseDoubleClick += notifyIcon_MouseDoubleClick;
            SysLog.d("SetNotifyIcon");
        }
        /// <summary>
        /// 托盘双击
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void notifyIcon_MouseDoubleClick(object sender, EventArgs e)
        {
            this.showAndHide();
        }
        /// <summary>
        /// 退出
        /// </summary>
        private void exitMethod()
        {
            //             if (MainWindow.portSettingWindow != null)
            //             {
            //                 MainWindow.portSettingWindow.Close();
            //             }
            //             if (MainWindow.serialPortWindow != null)
            //             {
            //                 MainWindow.serialPortWindow.Close();
            //             }
            //             if (this.loadWindow != null)
            //             {
            //                 this.loadWindow.Close();
            //             }
            base.Close();
        }
        /// <summary>
        /// 显示
        /// </summary>
        private void showMethod()
        {
            if (base.WindowState == WindowState.Minimized)
            {
                if (WindowState == WindowState.Maximized)
                {
                    base.Show();
                    base.WindowState = WindowState.Maximized;
                }
                else
                {
                    base.Show();
                    base.WindowState = WindowState.Normal;
                }
            }
        }

        /// <summary>
        /// 隐藏
        /// </summary>
        private void hideMethod()
        {
            if (base.WindowState == WindowState.Maximized)
            {
                this.WindowState = WindowState.Maximized;
                base.WindowState = WindowState.Minimized;
                base.Hide();
            }
            else if (base.WindowState == WindowState.Normal)
            {
                this.WindowState = WindowState.Normal;
                base.WindowState = WindowState.Minimized;
                base.Hide();
            }
            SysLog.d("hideMethod");
        }
        private void show_Click(object sender, EventArgs e)
        {
            this.showMethod();
        }

        private void hide_Click(object sender, EventArgs e)
        {
            this.hideMethod();
        }

        private void exit_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        /// <summary>
        /// 显示或隐藏界面
        /// </summary>
        private void showAndHide()
        {
            if (base.WindowState == WindowState.Minimized)
            {
                if (this.WindowState == WindowState.Maximized)
                {
                    base.Show();
                    base.WindowState = WindowState.Maximized;
                }
                else
                {
                    base.Show();
                    base.WindowState = WindowState.Normal;
                }
            }
            else if (base.WindowState == WindowState.Maximized)
            {
                this.WindowState = WindowState.Maximized;
                base.WindowState = WindowState.Minimized;
                base.Hide();
            }
            else if (base.WindowState == WindowState.Normal)
            {
                this.WindowState = WindowState.Normal;
                base.WindowState = WindowState.Minimized;
                base.Hide();
            }
        }

        #region 按键事件

        private void Button_MouseEnter(object sender, System.Windows.Input.MouseEventArgs e)
        {
            System.Windows.Controls.Button button = (System.Windows.Controls.Button)sender;
            //System.Windows.Controls.Image image = (System.Windows.Controls.Image)button.Content;
            button.Height = 75;
            button.Width = 80;
        }

        private void Button_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            System.Windows.Controls.Button button = (System.Windows.Controls.Button)sender;
            // System.Windows.Controls.Image image = (System.Windows.Controls.Image)button.Content;
            button.Height = 70;
            button.Width = 75;
        }
        #endregion

        private void btnUsb_Click(object sender, RoutedEventArgs e)//USB 管理
        {
            string fileName = System.Windows.Forms.Application.StartupPath + "\\usbTool\\Ratool.exe";
            ContrloApp.RunTarget(fileName, true);
        }

        private void btnSerialPort_Click(object sender, RoutedEventArgs e)//串口管理
        {
            FrmSerialPort frm = new FrmSerialPort();
            frm.Show();
        }

        private void btnBt_Click(object sender, RoutedEventArgs e)//蓝牙重连
        {
            BluetoothBs.Instance.RestBTConnect(AppConfig.Instance.BackSplint);
            BluetoothPrinter.Instance.RestBTConnect(AppConfig.Instance.Printer);
            System.Windows.MessageBox.Show(" 完成重连");
        }

        private void btnAPN_Click(object sender, RoutedEventArgs e)
        {

        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {

            MessageBoxResult messageBoxResult = System.Windows.MessageBox.Show("是否关闭窗口？是，关闭。否，最小化到托盘", "提示", MessageBoxButton.YesNo, MessageBoxImage.Asterisk, MessageBoxResult.Yes);
            if (messageBoxResult == MessageBoxResult.No)
            {
                this.hideMethod();
                e.Cancel = true;
            }
            else
            {
                e.Cancel = false;
            }
        }

        private void btnPrinter_Click(object sender, RoutedEventArgs e)
        {
            FrmPath frm = new FrmPath();
            frm.Show();
        }
        protected override void OnStateChanged(EventArgs e)
        {
            if (base.WindowState == WindowState.Minimized)
            {
                base.Hide();
            }
            base.OnStateChanged(e);
        }
    }
}
