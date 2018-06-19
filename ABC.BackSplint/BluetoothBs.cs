using ABC.BT;
using ABC.Config;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.Logs;
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

        //  protected override bool IsClosed { get => throw new System.NotImplementedException(); set => throw new System.NotImplementedException(); }

        public override void Run(object obj)
        {
            while (true)
            {
                SysLog.d($"BluetoothBs Run ReadCard_fd={DeviceIDs.ReadCard_fd}", null);
                if (isClosed == true || cts.Token.IsCancellationRequested)//关闭标志
                {
                    SysLog.d($"BluetoothBs.Run isClosed={isClosed},cts={cts.Token.IsCancellationRequested}", null);
                    BSApiHelper.device_close(DeviceIDs.ReadCard_fd);
                    DeviceIDs.ReadCard_fd = -1;
                    isConnet = false;
                    return;
                }

                if (DeviceIDs.ReadCard_fd > 0)
                {
                    int iRet;
                    lock (BackSplintLockObj.lockObj)
                    {
                        SysLog.d($"GetDeviceStatus:进入锁", null);
                        iRet = GetDeviceStatus();//检查设备状态 
                        SysLog.d($"GetDeviceStatus:退出锁,iRet={iRet}", null);
                    }
                    if (iRet == 0)
                    {
                        Thread.Sleep(15000);
                        continue;
                    }
                    else
                    {

                        iRet = BSApiHelper.device_close(DeviceIDs.ReadCard_fd);
                        SysLog.d($"device_close iret={iRet},ReadCard_fd={DeviceIDs.ReadCard_fd}", null);
                        DeviceIDs.ReadCard_fd = -1;
                        Thread.Sleep(1000);
                        isConnet = false;
                        //                         if (iRet == 0)
                        //                         {
                        //                             DeviceIDs.ReadCard_fd = -1;
                        //                         }
                        //                         else
                        //                         {
                        //                             Thread.Sleep(1000);
                        //                             continue;
                        //                         }

                    }
                }

                DeviceIDs.ReadCard_fd = DeviceApi.BSApiHelper.device_open(_Com - 1, _Baud);
                if (DeviceIDs.ReadCard_fd > 0)
                {
                    isConnet = true;
                    LedControl.Instance().ClearALL();//清空显示
                    BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                    BSApiHelper.Set_RCT_Timer(DeviceIDs.ReadCard_fd);
                }
                Thread.Sleep(6000);
            }

        }
        private int GetDeviceStatus()
        {
            byte[] verlen = { 0, 0, 0 };
            byte[] verdata = new byte[1024];
            byte[] ndev_status = new byte[1];
            // int iRet = BSApiHelper.get_device_status(DeviceIDs.ReadCard_fd, ref ndev_status[0]);
            int iRet = BSApiHelper.device_version(DeviceIDs.ReadCard_fd, 0, ref verlen[0], ref verdata[0]);
            if (iRet != 0)
            {
                if (ndev_status[0] > 4)
                {
                    return -1;
                }
            }

            return iRet;

        }
        //         public  void Stop()
        //         {
        //             base.Stop();
        //         }
        //         public override void RestBTConnect(abstractSerialPort config)
        //         {
        //             base.RestBTConnect(config);
        //         }
    }
}
