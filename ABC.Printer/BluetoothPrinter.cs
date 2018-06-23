using ABC.BT;
using ABC.Config;
using ABC.DeviceApi;
using ABC.Enity;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Printer
{
    public class BluetoothPrinter : iBluetooth
    {
        private static iBluetooth m_instance;
        static BluetoothPrinter()
        {
            m_instance = new BluetoothPrinter();
        }
        public static iBluetooth Instance
        {
            get
            {
                return m_instance;
            }

        }
        public override void Run(object obj)
        {
            if (DeviceIDs.Print_fd > 0)
            {
                PrintApiHelper.device_close_print(DeviceIDs.Print_fd);
                DeviceIDs.Print_fd = -1;
            }
            DeviceIDs.Print_fd = PrintApiHelper.device_open_print(this._Com, this._Baud);
        }
        //         public override void Stop()
        //         {
        //             if (DeviceIDs.Print_fd > 0)
        //             {
        //                 PrintApiHelper.device_close_print(DeviceIDs.Print_fd);
        //                 DeviceIDs.Print_fd = -1;
        //             }
        //             //cts.Cancel();//取消任务
        //             //base.Stop();
        //         }
        //         public override void RestBTConnect(abstractSerialPort config)
        //         {
        //             base.RestBTConnect(config);
        //         }
    }
}
