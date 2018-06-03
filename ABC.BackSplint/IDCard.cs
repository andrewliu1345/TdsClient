using ABC.abstractFun;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using System;
using System.Diagnostics;
using System.Text;

namespace ABC.BackSplint
{

    /// <summary>
    /// ID卡
    /// </summary>
    public class IDCard : aFuns
    {
        enum eDataType
        {
            Name = 0,
            Sex_Code = 11,
            Sex_CName = 1,
            IDC = 5,
            NATION_Code = 21,
            NATION_CName = 2,
            BIRTH = 3,
            ADDRESS = 4,
            REGORG = 6,
            STARTDATE = 7,
            ENDDATE = 8,
            PHOTO = 10,
        }

        private LedControl lcd = null;
        private static bool ReadOK = false;
        public override void SetData(byte[] buffer)
        {

//             byte[] bLen = new byte[2];
//             System.Array.Copy(buffer, 1, bLen, 0, 2);
//             int iLen = bLen.ByteArrayToIntH();
            int tag = buffer[5] & 0xff;
            byte[] bDateLen = new byte[1];
            System.Array.Copy(buffer, 6, bDateLen, 0, 1);
            int iDataLen = bDateLen.ToIntH();
            switch (tag)
            {
                case 1://readCard
                    {
                        byte[] bData = new byte[iDataLen];
                        System.Array.Copy(buffer, 7, bData, 0, iDataLen);
                        int itimeout = bData.ToIntH();
                        if (itimeout == 0)
                        {
                            m_TimeOut = 30000;
                        }
                        else
                        {
                            m_TimeOut = itimeout * 1000;
                        }
                        GetFpIDCard();
                        break;

                    }
                case 2://getData
                    {
                        byte[] bData = new byte[iDataLen];
                        System.Array.Copy(buffer, 7, bData, 0, iDataLen);
                        string sData = Encoding.Default.GetString(bData);
                        GetIDCardData(sData);
                        break;
                    }
                default:
                    {
                        byte[] bErrBuff = DataDispose.sendErr(new byte[] { 0, 1 });//失败获取
                        backData(bErrBuff);
                        break;
                    }
            }

        }
        public IDCard()
        {
            lcd = LedControl.Instance(DeviceIDs.ReadCard_fd);
        }
        private void GetBaseIDCard()
        {
            backData(null);
        }
        private void GetFpIDCard()
        {

            ReadOK = false;
            byte[] bSendBuff = null;
            if (DeviceIDs.ReadCard_fd <= 0)
            {
                bSendBuff = DataDispose.sendErr(new byte[] { 0, 1 });//超时

            }
            else
            {
                DeviceApi.BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                lcd.ShowLCD(LCDType.IDCARD);
                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();
                while (true)
                {
                    long lTime = stopwatch.ElapsedMilliseconds;
                    if (lTime > m_TimeOut)
                    {
                        bSendBuff = DataDispose.sendErr(new byte[] { 0, 2 });//超时
                        break;
                    }
                    StringBuilder stringBuilder = new StringBuilder(70);
                    int iRet = BSApiHelper.IDCard_ReadCard_finger(DeviceIDs.ReadCard_fd, stringBuilder);
                    if (iRet == 0)
                    {
                        DeviceApi.BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                        ReadOK = true;
                        string msg = stringBuilder.ToString();
                        SysLog.d("获取身份证返回:{0}", null, msg);
                        bSendBuff = DataDispose.sendOK();

                        break;
                    }

                }
            }
            lcd.ClearALL();
            backData(bSendBuff);
        }

        private void GetIDCardData(string name)
        {
            byte[] bSendBuff = null;
            eDataType type = (eDataType)Enum.Parse(typeof(eDataType), name);//字符串转enum
            if (ReadOK == true && DeviceIDs.ReadCard_fd > 0)
            {

                byte[] bdata = new byte[70];
                switch (type)
                {
                    case eDataType.Sex_Code:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, 1, ref bdata[0]);
                            string sSex = Encoding.Default.GetString(bdata).Replace("\0", "").Trim();
                            int sex = UnBackCode.UnBackSexCode(sSex);
                            bSendBuff = DataDispose.toPackData(new byte[] { (byte)sex }, 1);
                            break;
                        }
                    case eDataType.NATION_Code:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, 2, ref bdata[0]);
                            string sNation = Encoding.Default.GetString(bdata).Replace("\0", "").Trim();
                            int iNation = UnBackCode.UnBackSexCode(sNation);
                            bSendBuff = DataDispose.toPackData(new byte[] { (byte)iNation }, 1);
                            break;
                        }
                    default:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, (int)type, ref bdata[0]);
                            bSendBuff = DataDispose.toPackData(bdata, 70);
                            break;
                        }
                }
            }
            else
            {
                bSendBuff = DataDispose.sendErr(new byte[] { 0, 1 });//失败获取
            }
            backData(bSendBuff);
        }

    }
}
