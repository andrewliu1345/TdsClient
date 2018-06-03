using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace DeviceTest
{
    public class Api
    {
        //打开设备
        [DllImport("DLL\\mtx_32.dll", EntryPoint = "device_open")]
        public static extern int device_open(int port, int baud);

        //打开设备
        [DllImport("DLL\\mtx_32.dll", EntryPoint = "device_open_name")]
        public static extern int device_open_name(string pPort, int auxPort, int baud);

        //关闭设备
        [DllImport("DLL\\mtx_32.dll", EntryPoint = "device_close")]
        public static extern int device_close(int icdev);

        [DllImport("DLL\\mtx_32.dll", EntryPoint = "get_device_status")]
        public static extern int get_device_status(int icdev, ref byte ndev_status);

        [DllImport("DLL\\mtx_32.dll", EntryPoint = "Set_RCT_Timer")]
        public static extern int Set_RCT_Timer(int icdev);

        //打开设备
        [DllImport("DLL\\Print_32.dll", EntryPoint = "device_open")]
        public static extern int device_open_print(int port, int baud);

        //关闭设备
        [DllImport("DLL\\Print_32.dll", EntryPoint = "device_close")]
        public static extern int device_close_print(int icdev);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="timeout"></param>
        /// <param name="mag_card"></param>
        /// <returns></returns>
        [DllImport("DLL\\mtx_32.dll", EntryPoint = "magnetic_read_ms")]
        public static extern int magnetic_read(int icdev, int timeout, ref byte mag_card);

        //打印
        [DllImport("DLL\\Print_32.dll", EntryPoint = "Print_CHS")]
        public static extern int Print_CHS(int icdev, string str, int length);
    }
}
