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
                backErrData(new byte[] { 0, 1 });//失败获取
                return;
            }
            PrintMsg(buffer);

        }
        private void PrintMsg(byte[] buffer)
        {
            //byte[] sendBuffer;
            List<byte[]> lPrams = DataDispose.unPackData(buffer, 1);
            byte[] data = lPrams[0];
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
            //backData(sendBuffer);
        }
    }
}
