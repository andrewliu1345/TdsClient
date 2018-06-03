using ABC.abstractFun;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.BackSplint
{
    /// <summary>
    /// IC 卡
    /// </summary>
    public class ICCard : aFuns
    {
        static int CardNo = -1;
        int _fd = -1;
        public override void SetData(byte[] buffer)
        {
            _fd = DeviceIDs.ReadCard_fd;
            int tag = buffer[5] & 0xff;
            switch (tag)
            {
                case 6:
                    {
                        ICCardPowerOn(buffer);
                        break;
                    }
                case 7:
                    {
                        ICCardAPDU(buffer);
                        break;
                    }
                default:
                    break;
            }
        }
        /// <summary>
        /// 卡片上电
        /// </summary>
        /// <param name="buffer">参数</param>
        private void ICCardPowerOn(byte[] buffer)
        {
            int index = 7;
            int timeout = 0;
            int iLen = 0;
            byte[] bLen = new byte[2];
            System.Array.Copy(buffer, index, bLen, 0, 2);
            index += 2;
            iLen = bLen.ByteArrayToIntH();
            byte[] bTimeout = new byte[iLen];
            System.Array.Copy(buffer, index, bTimeout, 0, iLen);
            timeout = bTimeout.ByteArrayToIntH() * 1000;//毫秒为单位
            byte[] msg = new byte[1024];
            int length = 0;
            int st = DeviceApi.BSApiHelper.sam_slt_reset(_fd, timeout, 0, ref length, ref msg[0]);
            if (st != 0)
            {
                byte[] cardtype = { 0 };
                byte[] snrlen = { 0 };
                byte[] snr = new byte[4];
                st = DeviceApi.BSApiHelper.open_card(_fd, timeout, ref cardtype[0], ref snrlen[0], ref snr[0], ref length, ref msg[0]);

                if (st == 0)
                {
                    CardNo = 0xFF;
                    SysLog.i("读取到非接卡");
                    byte[] sendBuffer = DataDispose.sendOK();
                    backData(sendBuffer);
                    return;
                }
                else
                {
                    CardNo = -1;
                    SysLog.i("读取失败");
                    byte[] sendBuffer = DataDispose.sendErr(new byte[] { 0, 1 });
                    backData(sendBuffer);
                    return;
                }
            }
            else
            {
                CardNo = 0xFF;
                SysLog.i("读取到接触卡");
                byte[] sendBuffer = DataDispose.sendOK();
                backData(sendBuffer);
                return;
            }
        }

        private void ICCardAPDU(byte[] buffer)
        {
            int index = 6;
            int iLen = 0;
            byte[] bLen = new byte[2];
            System.Array.Copy(buffer, index, bLen, 0, 2);
            index += 2;
            iLen = bLen.ByteArrayToIntH();
            byte[] bApdu = new byte[iLen];
            System.Array.Copy(buffer, index, bApdu, 0, iLen);
//             String sApdu = Encoding.Default.GetString(bApdu);
//             byte[] sendApdu = sApdu.HexString2ByteArray();
            byte[] recBuffer = new byte[2048];
            int reclen = 0;
            int iRet = DeviceApi.BSApiHelper.card_APDU(_fd, CardNo, bApdu.Length, bApdu, ref reclen, ref recBuffer[0]);
            if (iRet == 0)
            {
                byte[] sendBuffer = DataDispose.toPackData(recBuffer, reclen);
                backData(sendBuffer);
            }
            else
            {
                byte[] sendBuffer = DataDispose.sendErr(new byte[] { 0, 1 });
                backData(sendBuffer);
            }
        }
    }
}
