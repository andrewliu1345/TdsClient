using ABC.abstractFun;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.BackSplint
{
    public class MagneticCard : aFuns
    {
        int _fd = -1;
        public override void SetData(byte[] buffer)
        {
            _fd = DeviceIDs.ReadCard_fd;
            int tag = buffer[5];
            switch (tag)
            {
                case 1:
                    {
                        ReadMagetcCard(buffer);
                        break;
                    }
                default:
                   byte[] sendBuffer = DataDispose.sendErr(new byte[] { 0, 1 });
                    backData(sendBuffer);
                    break;
            }
        }
        private void ReadMagetcCard(byte[] buffer)
        {
            byte[] sendBuffer;
            List<byte[]> lParams = DataDispose.unPackData(buffer, 1);
            int iTimeout = lParams[0].ToIntH();
            byte[] bMag_card = new byte[256];
            int iRet = BSApiHelper.magnetic_read(_fd, iTimeout, ref bMag_card[0]);
            switch (iRet)
            {
                case 0:
                    {
                        sendBuffer = DataDispose.toPackData(bMag_card, bMag_card.Length);
                        break;
                    }
                case -0x0002:
                    {
                        sendBuffer = DataDispose.sendErr(new byte[] { 0, 2 });
                        SysLog.e("磁条刷卡超时：{0}", null, iRet);
                        break;
                    }
                case -0x0003:
                    {
                        sendBuffer = DataDispose.sendErr(new byte[] { 0, 3 });
                        SysLog.e("磁条刷卡失败：{0}", null, iRet);
                        break;
                    }
                default:
                    {
                        sendBuffer = DataDispose.sendErr(new byte[] { 0, 1 });
                        SysLog.e("磁条刷卡失败：{0}", null, iRet);
                        break;
                    }
            }
            backData(sendBuffer);
        }
    }
}
