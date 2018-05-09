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
        public MainWindow()
        {
            //this.ShowInTaskbar = false;
            InitializeComponent();
            this.SetNotifyIcon();
            this.hideMethod();
        }
        private NotifyIcon notifyIcon;
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
        private void notifyIcon_MouseDoubleClick(object sender, EventArgs e)
        {
            this.showAndHide();
        }
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
    }
}
