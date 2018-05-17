using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;

namespace DeviceTest
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        /// <summary>
        /// 读卡器句柄
        /// </summary>
        public static int ReadCard_fd=-1;

        /// <summary>
        /// 打印机句柄
        /// </summary>
        public static int Print_fd = -1;
    }
}
