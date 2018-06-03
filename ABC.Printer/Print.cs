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
            if (DeviceIDs.Print_fd <= 0)
            {
                byte[] bErrBuff = DataDispose.sendErr(new byte[] { 0, 1 });//失败获取
                backData(bErrBuff);
                return;
            }
            PrintMsg(buffer);

        }
        private void PrintMsg(byte[] buffer)
        {
            byte[] sendBuffer;
            List<byte[]> lPrams = DataDispose.unPackData(buffer, 1);
            byte[] data = lPrams[0];
            int iRet = PrintApiHelper.Print_CHS(DeviceIDs.Print_fd, data, data.Length);
            if (iRet == 0)
            {
                sendBuffer = DataDispose.sendOK();
            }
            else
            {
                sendBuffer = DataDispose.sendErr(new byte[] { 0, 1 });
                SysLog.e("打印失败");
            }
            backData(sendBuffer);
        }
    }
}
