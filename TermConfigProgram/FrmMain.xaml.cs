using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TermConfigProgram
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
            SetWindowLocation();//程序初始位置
            SetNotifyIcon();//设置托盘图标
            hideMethod();//隐藏界面
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
            //this.exitMethod();
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

        }

        private void btnSerialPort_Click(object sender, RoutedEventArgs e)//串口管理
        {

        }

        private void btnBt_Click(object sender, RoutedEventArgs e)//蓝牙重连
        {

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
                //                 if (this.Pro != null)
                //                 {
                //                     MainWindow.portSettingWindow.Close();
                //                 }
                //                 if (MainWindow.serialPortWindow != null)
                //                 {
                //                     MainWindow.serialPortWindow.Close();
                //                 }
                //                 if (this.loadWindow != null)
                //                 {
                //                     this.loadWindow.Close();
                //                 }
                e.Cancel = false;
              //  base.Close();
            }
        }
    }
}
