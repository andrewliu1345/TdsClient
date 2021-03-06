﻿using ABC.abstractFun;
using ABC.Attribute;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

namespace ABC.BackSplint
{
    /// <summary>
    /// IC 卡
    /// </summary>
    [BackSplintAttribute(isBackSplint = true)]
    public class ICCard : aFuns
    {
        private LedControl lcd = LedControl.Instance();
        static int CardNo = -1;
        int _fd = -1;
        public override void SetData(byte[] buffer)
        {
            base.SetData(buffer);
            _fd = DeviceIDs.ReadCard_fd;
            int tag = buffer[5] & 0xff;
            switch (tag)
            {
                case 6:
                    {
                        DeviceApi.BSApiHelper.device_beep(_fd, 0, 1);
                        lcd.ShowLCD(LCDType.IC | LCDType.NFC);
                        ICCardPowerOn(buffer);
                        lcd.ClearALL();
                        break;
                    }
                case 7:
                    {
                        ICCardAPDU(buffer);
                        break;
                    }
                case 8:
                    {
                        ICCardAPDU2(buffer);//解决完法读卡
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
            List<byte[]> lParams = DataDispose.unPackData(buffer, 1);
            int timeout = lParams[0].ToIntH();
            if (timeout == 0)
            {
                timeout = 5000;
            }
            else
            {
                timeout *= 1000;
            }
            //毫秒为单位
            byte[] msg = new byte[1024];
            int length = 0;

            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            CardNo = -1;
            while (true)
            {
                long lTime = stopwatch.ElapsedMilliseconds;
                if (lTime > timeout-1000)
                {
                    backErrData(new byte[] { 0, 2 });//超时
                    return;
                }

                int st = DeviceApi.BSApiHelper.sam_slt_reset(_fd, 0, 0, ref length, ref msg[0]);
                if (st != 0)
                {
                   // timeout -= 500;
                    byte[] cardtype = { 0 };
                    byte[] snrlen = { 0 };
                    byte[] snr = new byte[4];
                    st = DeviceApi.BSApiHelper.open_card(_fd, 0, ref cardtype[0], ref snrlen[0], ref snr[0], ref length, ref msg[0]);

                    if (st == 0)
                    {
                        DeviceApi.BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                        CardNo = 0xFF;
                        SysLog.i("读取到非接卡");
                        //byte[] sendBuffer = DataDispose.sendOK();
                        backData(null, 0);
                        break;
                    }
                    //timeout -= 500;

                }
                else
                {
                    DeviceApi.BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                    CardNo = 0x00;
                    SysLog.i("读取到接触卡");
                    //byte[] sendBuffer = DataDispose.sendOK();
                    backData(null, 0);
                    break;
                }
                Thread.Sleep(10);
            }
            if (CardNo == -1)
            {
                SysLog.i("读取失败");
                //  byte[] sendBuffer = DataDispose.sendErr(new byte[] { 0, 1 });
                backErrData(new byte[] { 0, 1 });
            }

            stopwatch.Stop();
        }

        private void ICCardAPDU(byte[] buffer)
        {
            List<byte[]> lParams = DataDispose.unPackData(buffer, 1);
            string sApdu = lParams[0].GetString();
            SysLog.d("sApdu={0}", null, sApdu);
            byte[] bApdu = sApdu.HexString2ByteArray();
            byte[] recBuffer = new byte[2048];
            int reclen = 0;
            int iRet = DeviceApi.BSApiHelper.card_APDU(_fd, CardNo, bApdu.Length, bApdu, ref reclen, ref recBuffer[0]);
            if (iRet == 0)
            {
               // DeviceApi.BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                //byte[] sendBuffer = DataDispose.toPackData(recBuffer, reclen);
                string strRecBuffer = recBuffer.bytesToHexString(reclen);
                SysLog.d("reADPU={0}", null, strRecBuffer);
                byte[] bRecBuffer = strRecBuffer.Replace("\0", "").Replace(" ", "").ToByteArry();
                backData(bRecBuffer, bRecBuffer.Length);
            }
            else
            {
                SysLog.e("Adpu iRet={0}",null ,iRet);
                // byte[] sendBuffer = DataDispose.sendErr();
                backErrData(new byte[] { 0, 1 });
            }
        }
        const string rApdu = "6f668408a000000333010101a55a500a50424f432044656269748701015f2d047a68656e9f1101019f120d494342432050626f63436172649f381b9f66049f02069f03069f1a0295055f2a029a039c019f37049f2103bf0c0fd1023132c204494342439f4d020b0a9000";
        private void ICCardAPDU2(byte[] buffer)
        {
            List<byte[]> lParams = DataDispose.unPackData(buffer, 1);
            string sApdu = lParams[0].GetString();
            if (sApdu.Equals("00A4040008A000000333010101"))
            {
                byte[] bRecBuffer = rApdu.Replace("\0", "").Replace(" ", "").ToByteArry();
                backData(bRecBuffer, bRecBuffer.Length);
                return;
            }
            SysLog.d("sApdu={0}", null, sApdu);
            byte[] bApdu = sApdu.HexString2ByteArray();
            byte[] recBuffer = new byte[2048];
            int reclen = 0;
            int iRet = DeviceApi.BSApiHelper.card_APDU(_fd, CardNo, bApdu.Length, bApdu, ref reclen, ref recBuffer[0]);
            if (iRet == 0)
            {
                // DeviceApi.BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                //byte[] sendBuffer = DataDispose.toPackData(recBuffer, reclen);
                string strRecBuffer = recBuffer.bytesToHexString(reclen);
                SysLog.d("reADPU={0}", null, strRecBuffer);
                byte[] bRecBuffer = strRecBuffer.Replace("\0", "").Replace(" ", "").ToByteArry();
                backData(bRecBuffer, bRecBuffer.Length);
            }
            else
            {
                SysLog.e("Adpu iRet={0}", null, iRet);
                // byte[] sendBuffer = DataDispose.sendErr();
                backErrData(new byte[] { 0, 1 });
            }
        }
    }
}
