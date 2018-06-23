using ABC.abstractFun;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using System;
using System.Collections.Generic;

namespace ABC.Printer
{
    public class Print : aFuns
    {
        public override void SetData(byte[] buffer)
        {
            base.SetData(buffer);
            if (DeviceIDs.Print_fd <= 0)
            {
                SysLog.e($"设备未连 DeviceIDs.Print_fd={DeviceIDs.Print_fd}");
                backErrData(new byte[] { 0, 1 });//失败获取
                return;
            }
            PrintMsg(buffer);

        }
        private void PrintMsg(byte[] buffer)
        {
            //byte[] sendBuffer;
            List<byte[]> lPrams = DataDispose.unPackData(buffer, 2);

            byte[] bfileName = lPrams[0];
            byte[] bData = lPrams[1];

            string sfileName = bfileName.GetString();
            string sData = bData.GetString("UTF-8");
            SysLog.d($"PrintMsg sfileName={sfileName} \n sData={sData} \n bData={bData.bytesToHexString()}", null);
            LoadFormData load = new LoadFormData();
            try
            {
                byte[] data = load.FormData(sfileName, sData);
                int iRet = PrintApiHelper.Print_CHS(DeviceIDs.Print_fd, data, data.Length);
                if (iRet == 0)
                {
                    backData(null, 0);
                }
                else
                {
                    backErrData(new byte[] { 0, 1 });
                    SysLog.e("打印失败");
                }
            }
            catch (Exception ex)
            {
                SysLog.e("PrintMsg", ex);
                backErrData(new byte[] { 0, 1 });
            }

            //backData(sendBuffer);
        }
    }
}
