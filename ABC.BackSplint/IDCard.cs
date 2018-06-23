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
using System.Threading;

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
            NAME = 0,
            SEX_CODE = 11,
            SEX_CNAME = 1,
            IDC = 5,
            NATION_CODE = 21,
            NATION_CNAME = 2,
            NATION = 2,
            BIRTH = 3,
            ADDRESS = 4,
            REGORG = 6,
            STARTDATE = 7,
            ENDDATE = 8,
            PHOTO = 10,
            PHOTODATA = 101,
            BIRTHDAY_YEAR = 31,
            BIRTHDAY_MONTH = 32,
            BIRTHDAY_DAY = 33,
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
                        SysLog.d("开始读卡", null);
                        GetFpIDCard(buffer);
                        SysLog.d("开始结束", null);
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
                try
                {
                    Stopwatch stopwatch = new Stopwatch();
                    stopwatch.Start();
                    int iRet = BSApiHelper.IDCard_SetPhotoPath(AppConfig.Instance.Path.PhotoPath);
                    while (true)
                    {
                        long lTime = stopwatch.ElapsedMilliseconds;
                        if (lTime > m_TimeOut)
                        {
                            SysLog.d($"获取身份证返回超时", null);
                            backErrData(new byte[] { 0, 2 });//超时
                            break;
                        }

                        //StringBuilder stringBuilder = new StringBuilder(70);
                        byte[] bMsg = new byte[71];
                        iRet = BSApiHelper.IDCard_ReadCard_finger(DeviceIDs.ReadCard_fd, ref bMsg[0]);
                        //SysLog.d($"获取身份证返回:iret={iRet}", null);
                        if (iRet == 0)
                        {
                            ReadOK = true;
                            string msg = bMsg.GetString();
                            SysLog.d("获取身份证返回:{0}", null, msg);
                            backData(null, 0);
                            break;
                        }
                        Thread.Sleep(100);
                    }
                }
                catch (Exception ex)
                {
                    SysLog.d($"GetFpIDCard出错", ex);
                }


            }


        }

        private void GetIDCardData(byte[] buffer)
        {
            List<byte[]> lParams = DataDispose.unPackData(buffer, 1);
            string name = lParams[0].GetString().ToUpper().Trim();
            SysLog.d($"GetIDCardData 下发Name={name}");
            eDataType type = 0;
            try
            {
                type = (eDataType)Enum.Parse(typeof(eDataType), name);//字符串转enum
            }
            catch (Exception ex)
            {
                SysLog.e($"GetIDCardData {name} 不在Enum中", ex);
            }

            if (ReadOK == true && DeviceIDs.ReadCard_fd > 0)
            {

                byte[] bdata = new byte[70];
                switch (type)
                {
                    case eDataType.SEX_CODE:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, 1, ref bdata[0]);
                            string sSex = Encoding.Default.GetString(bdata).Replace("\0", "").Trim();
                            int sex = UnBackCode.UnBackSexCode(sSex);
                            backData(new byte[] { (byte)sex }, 1);
                            break;
                        }
                    case eDataType.NATION_CODE:
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
                    case eDataType.BIRTHDAY_YEAR:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, 3, ref bdata[0]);
                            byte[] year = new byte[4];
                            System.Array.Copy(bdata, year, 4);
                            backData(year, 4);
                            break;
                        }
                    case eDataType.BIRTHDAY_MONTH:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, 3, ref bdata[0]);
                            byte[] month = new byte[2];
                            System.Array.Copy(bdata, 4, month, 0, 2);
                            backData(month, 2);
                            break;
                        }
                    case eDataType.BIRTHDAY_DAY:
                        {
                            BSApiHelper.IDCard_GetCardInfo(DeviceIDs.ReadCard_fd, 3, ref bdata[0]);
                            byte[] day = new byte[2];
                            System.Array.Copy(bdata, 6, day, 0, 2);
                            backData(day, 2);
                            break;
                        }
                    case eDataType.PHOTODATA:
                        {
                            StringBuilder stringBuilder = new StringBuilder(38863);
                            //byte[] stringBuilder = new byte[38863];
                            string path = string.Format($"{AppConfig.Instance.Path.PhotoPath}zp.bmp");
                            BSApiHelper.GetFileBase64Buffer(path,  stringBuilder);
                            byte[] b = stringBuilder.ToString().ToByteArry();
                            backData(b, b.Length);
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
