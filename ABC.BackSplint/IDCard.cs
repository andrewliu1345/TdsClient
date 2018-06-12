using ABC.abstractFun;
using ABC.Attribute;
using ABC.Config;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace ABC.BackSplint
{

    /// <summary>
    /// ID卡
    /// </summary>
    [BackSplintAttribute(isBackSplint = true)]
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
            base.SetData(buffer);
            int tag = buffer[5] & 0xff;

            switch (tag)
            {
                case 1://readCard
                    {
                        DeviceApi.BSApiHelper.device_beep(DeviceIDs.ReadCard_fd, 0, 1);
                        lcd.ShowLCD(LCDType.IDCARD);
                        GetFpIDCard(buffer);
                        lcd.ClearALL();
                        break;
                    }
                case 2://getData
                    {
                        GetIDCardData(buffer);
                        break;
                    }
                default:
                    {
                        backErrData(new byte[] { 0, 1 });//失败获取

                        break;
                    }
            }

        }
        public IDCard()
        {
            lcd = LedControl.Instance();
        }
        private void GetBaseIDCard()
        {
            backData(null, 0);
        }
        private void GetFpIDCard(byte[] buffer)
        {
            List<byte[]> lParams = DataDispose.unPackData(buffer, 1);
            int itimeout = lParams[0].ToIntH();
            if (itimeout == 0)
            {
                m_TimeOut = 30000;
            }
            else
            {
                m_TimeOut = itimeout * 1000;
            }


            ReadOK = false;

            if (DeviceIDs.ReadCard_fd <= 0)
            {
                backErrData(new byte[] { 0, 1 });//超时

            }
            else
            {

                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();
                while (true)
                {
                    long lTime = stopwatch.ElapsedMilliseconds;
                    if (lTime > m_TimeOut)
                    {
                        backErrData(new byte[] { 0, 2 });//超时
                        break;
                    }
                    int iRet = BSApiHelper.IDCard_SetPhotoPath(AppConfig.Instance.Path.PhotoPath);
                    StringBuilder stringBuilder = new StringBuilder(70);
                    iRet = BSApiHelper.IDCard_ReadCard_finger(DeviceIDs.ReadCard_fd, stringBuilder);
                    if (iRet == 0)
                    {
                        ReadOK = true;
                        string msg = stringBuilder.ToString();
                        SysLog.d("获取身份证返回:{0}", null, msg);
                        backData(null, 0);
                        break;
                    }

                }
            }


        }

        private void GetIDCardData(byte[] buffer)
        {
            List<byte[]> lParams = DataDispose.unPackData(buffer, 1);
            string name = lParams[0].GetString();
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
                            backData(new byte[] { (byte)sex }, 1);
                            break;
                        }
                    case eDataType.NATION_Code:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, 2, ref bdata[0]);
                            string sNation = Encoding.Default.GetString(bdata).Replace("\0", "").Trim();
                            int iNation = UnBackCode.UnBackSexCode(sNation);
                            backData(new byte[] { (byte)iNation }, 1);
                            break;
                        }
                    case eDataType.PHOTO:
                        {
                            BSApiHelper.IDCard_GetPhotoFile(ref bdata[0]);
                            backData(bdata, 70);
                            break;
                        }
                    default:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, (int)type, ref bdata[0]);
                            backData(bdata, 70);
                            break;
                        }
                }
            }
            else
            {
                backErrData(new byte[] { 0, 1 });//失败获取
            }

        }

    }
}
