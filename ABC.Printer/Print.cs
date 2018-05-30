using ABC.abstractFun;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using System;

namespace ABC.Printer
{
    public class Print : aFuns
    {
        public override void SetData(byte[] buffer)
        {
            if (DeviceIDs.Print_fd<=0)
            {
                byte[] bErrBuff = DataDispose.sendErr(new byte[] { 0, 1 });//失败获取
                backData(bErrBuff);
                return;
            }

        }
        private void PrintMsg(byte[] data)
        {
            PrintApiHelper.Print_CHS(DeviceIDs.Print_fd,data,data.Length);
        }
    }
}
