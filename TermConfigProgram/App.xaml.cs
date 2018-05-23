﻿using ABC.Background;
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
            CommServer.Instance.Start();
        }
    }
}
