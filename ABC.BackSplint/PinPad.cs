using ABC.abstractFun;
using System;
using ABC.HelperClass;
using System.Collections.Generic;
using ABC.DeviceApi;
using ABC.Logs;
using System.Text;
using ABC.Enity;

namespace ABC.BackSplint
{
    public class Pinpad : aFuns
    {
        int _fd = -1;
        /// <summary>
        /// 
        /// </summary>
        /// <param name="buffer"></param>
        public override void SetData(byte[] buffer)
        {
            base.SetData(buffer);
            _fd = DeviceIDs.ReadCard_fd;
            int tag = buffer[5];
            DeviceApi.BSApiHelper.device_beep(_fd, 0, 1);
            switch (tag)
            {
                case 1:
                    {
                        GetPinPlock(buffer);
                        break;
                    }
                case 3:
                    {
                        LoadMkey(buffer);
                        break;
                    }
                case 4:
                    {
                        LoadWkey(buffer);
                        break;
                    }
                case 6:
                    {
                        GetMac(buffer);
                        break;
                    }
                default:
                    break;
            }
        }

        /// <summary>
        /// 下载主密钥
        /// </summary>
        /// <param name="buffer"></param>
        private void LoadMkey(byte[] buffer)
        {
            List<byte[]> lParmsArry = DataDispose.unPackData(buffer, 4);
            int MKeyIndex = lParmsArry[0].ToIntH();
            int EncryType = lParmsArry[1].ToIntH();
            byte[] key = lParmsArry[2];
            byte[] errmsg = new byte[70];
            int iRet = BSApiHelper.LoadMasterkey(_fd, MKeyIndex, EncryType, key.Length, ref key[0], ref errmsg[0]);
            if (iRet == 0)
            {
                backData(null,0);
            }
            else
            {
                backErrData(new byte[] { 0, 1 });
                string errMsg = Encoding.Default.GetString(errmsg);
                SysLog.e("密钥写入失败:{0}", null, errMsg);
            }
           
        }

        /// <summary>
        /// 下载工作密钥
        /// </summary>
        /// <param name="buffer"></param>
        private void LoadWkey(byte[] buffer)
        {
            List<byte[]> lParmsArry = DataDispose.unPackData(buffer, 4);
            int MKeyIndex = lParmsArry[0].ToIntH();
            int WKeyIndex = lParmsArry[1].ToIntH();
            int EncryType = lParmsArry[2].ToIntH();
            int KeyType = lParmsArry[3].ToIntH();
            byte[] key = lParmsArry[4];
            byte[] errmsg = new byte[70];

            int iRet = BSApiHelper.LoadWorkkey(_fd, MKeyIndex, key.Length, ref key[0], ref errmsg[0]);
            if (iRet == 0)
            {
                switch (KeyType)
                {
                    case 0:
                        {
                            Array.Clear(errmsg, 0, 70);
                            iRet = BSApiHelper.LoadPINkey(_fd, MKeyIndex, key.Length, ref key[0], ref errmsg[0]);
                            Array.Clear(errmsg, 0, 70);
                            iRet = BSApiHelper.LoadMackey(_fd, MKeyIndex, key.Length, ref key[0], ref errmsg[0]);
                            break;
                        }
                    case 1:
                        {
                            Array.Clear(errmsg, 0, 70);
                            iRet = BSApiHelper.LoadPINkey(_fd, MKeyIndex, key.Length, ref key[0], ref errmsg[0]);
                            break;
                        }
                    case 2:
                        {
                            Array.Clear(errmsg, 0, 70);
                            iRet = BSApiHelper.LoadMackey(_fd, MKeyIndex, key.Length, ref key[0], ref errmsg[0]);
                            break;
                        }
                    default:
                        iRet = -1;
                        break;
                }
            }
            if (iRet == 0)
            {
                backData(null,0);
            }
            else
            {
                backErrData(new byte[] { 0, 1 });
                string err = errmsg.GetString();
                SysLog.e("下载工作密钥失败：{0}", null, err);
            }
           
        }

        /// <summary>
        /// 获取pinblock
        /// </summary>
        /// <param name="buffer"></param>
        private void GetPinPlock(byte[] buffer)
        {
           
            List<byte[]> lParms = DataDispose.unPackData(buffer, 9);
            int iMKeyIndex = lParms[0].ToIntH();
            int iEncryType = lParms[1].ToIntH();
            int iTimes = lParms[2].ToIntH();
            int iAmount = lParms[3].ToIntH();
            byte[] bPan = lParms[4];

            int iLength = lParms[5].ToIntH();
            string sVoice = lParms[6].GetString();
            int iEndType = lParms[7].ToIntH();
            int iTimeout = lParms[8].ToIntH();

            int pinlen = 0;
            byte[] pinblock = new byte[64];
            byte[] errMsg = new byte[70];
            int iRet = BSApiHelper.GetPinBlock(_fd, iMKeyIndex, bPan.Length, ref bPan[0], iTimeout, ref pinlen, ref pinblock[0],ref errMsg[0]);
            if (iRet == 0)
            {
                backData(pinblock, pinlen);
                //sendBuffer = DataDispose.toPackData(pinblock, pinlen);
            }
            else
            {
                backErrData(new byte[] { 0, 1 });
                SysLog.e("获取PinBlock 失败:{0}", null, errMsg.GetString());
            }
            //backData(sendBuffer);
        }

        /// <summary>
        /// 获取mac
        /// </summary>
        /// <param name="buffer"></param>
        private void GetMac(byte[] buffer)
        {
         
            List<byte[]> lParams = DataDispose.unPackData(buffer, 5);
            int iMKeyIndex=lParams[0].ToIntH();
            int iEncryType = lParams[1].ToIntH();
            int iMode = lParams[2].ToIntH();
            byte[] bMacSrc = lParams[3];
            int macSrclen=lParams[4].ToIntH();

            byte[] errMsg = new byte[70];

            byte[] bmac = new byte[70];
            int imaclen = 0;

            int iRet = BSApiHelper.GetMAC(_fd, iMKeyIndex, iMode, macSrclen, ref bMacSrc[0], ref imaclen, ref bmac[0], ref errMsg[0]);
            if (iRet==0)
            {
                backData(bmac, imaclen);
            }
            else
            {
                backErrData(new byte[] { 0, 1 });
                string err = errMsg.GetString();
                SysLog.e("获取MAC失败：{0}", null, err);
            }
        }
    }
}
