using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ABC.DeviceApi
{
    public class PrintApiHelper
    {
        const string DllPath = "DLL\\Print_32.dll";

        //打开设备
        [DllImport(DllPath, EntryPoint = "device_open")]
        public static extern int device_open_print(int port, int baud);

        //关闭设备
        [DllImport(DllPath, EntryPoint = "device_close")]
        public static extern int device_close_print(int icdev);

        //打印
        [DllImport(DllPath, EntryPoint = "Print_CHS")]
        public static extern int Print_CHS(int icdev, string str, int length);
    }
}
