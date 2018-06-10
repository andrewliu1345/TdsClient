using ABC.BT;
using ABC.DeviceApi;
using ABC.Enity;
using System.Threading;

namespace ABC.BackSplint
{
    public class BluetoothBs : iBluetooth
    {
        static BluetoothBs()
        {
            m_instance = new BluetoothBs();
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
            while (true)
            {
                if (isClosed == true)//关闭标志
                {
                    BSApiHelper.device_close(DeviceIDs.ReadCard_fd);
                    DeviceIDs.ReadCard_fd = -1;
                    return;
                }

                if (DeviceIDs.ReadCard_fd > 0)
                {
                    int iRet = GetDeviceStatus();//检查蓝牙状态
                    if (iRet == 0)
                    {
                        Thread.Sleep(6000);
                        continue;
                    }
                    else
                    {
                        BSApiHelper.device_close(DeviceIDs.ReadCard_fd);
                        DeviceIDs.ReadCard_fd = -1;
                    }
                }
                DeviceIDs.ReadCard_fd = DeviceApi.BSApiHelper.device_open(_Com - 1, _Baud);
                if (DeviceIDs.ReadCard_fd > 0)
                {
                    BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                    BSApiHelper.Set_RCT_Timer(DeviceIDs.ReadCard_fd);
                }
                Thread.Sleep(1000);
            }
        }
        private int GetDeviceStatus()
        {
            byte[] buffer = new byte[1];
            int iRet = BSApiHelper.get_device_status(DeviceIDs.ReadCard_fd, ref buffer[0]);
            if (iRet != 0)
            {
                return -1;
            }
            else
            {
                return buffer[0];
            }
        }
    }
}
