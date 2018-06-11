using ABC.abstractFun;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using System.Collections.Generic;

namespace ABC.BackSplint
{
    public class MagneticCard : aFuns
    {
        private LedControl lcd = LedControl.Instance();
        int _fd = -1;
        public override void SetData(byte[] buffer)
        {
            base.SetData(buffer);
            _fd = DeviceIDs.ReadCard_fd;
            int tag = buffer[5];
            switch (tag)
            {
                case 1:
                    {
                        BSApiHelper.device_beep(_fd, 0, 1);
                        lcd.ShowLCD(LCDType.MAGNETICCARD);
                        ReadMagetcCard(buffer);
                        lcd.ClearALL();
                        break;
                    }
                default:
                    backErrData(new byte[] { 0, 1 });
                    
                    break;
            }
        }
        private void ReadMagetcCard(byte[] buffer)
        {
            
            List<byte[]> lParams = DataDispose.unPackData(buffer, 1);
            int itimeout = lParams[0].ToIntH();
            if (itimeout == 0)
            {
                m_TimeOut = 5000;
            }
            else
            {
                m_TimeOut = itimeout;
            }
            byte[] bMag_card = new byte[294];
            int iRet = BSApiHelper.magnetic_read(_fd, m_TimeOut, ref bMag_card[0]);
            switch (iRet)
            {
                case 0:
                    {
                        backData(bMag_card, bMag_card.Length);
                        break;
                    }
                case -0x0002:
                    {
                        backErrData(new byte[] { 0, 2 });
                        SysLog.e("磁条刷卡超时：{0}", null, iRet);
                        break;
                    }
                case -0x0003:
                    {
                        backErrData(new byte[] { 0, 3 });

                        SysLog.e("磁条刷卡失败：{0}", null, iRet);
                        break;
                    }
                default:
                    {
                        backErrData(new byte[] { 0, 1 });

                        SysLog.e("磁条刷卡失败：{0}", null, iRet);
                        break;
                    }
            }
            //backData(sendBuffer);
        }
    }
}
