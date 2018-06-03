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
            _fd = DeviceIDs.ReadCard_fd;
            int tag = buffer[5];
            switch (tag)
            {
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
            int MKeyIndex = lParmsArry[0].ByteArrayToIntH();
            int EncryType = lParmsArry[1].ByteArrayToIntH();
            byte[] key = lParmsArry[2];
            byte[] errmsg = new byte[70];
            byte[] sendBuffer = null;
            int iRet = BSApiHelper.LoadMasterkey(_fd, MKeyIndex, EncryType, key.Length, ref key[0], ref errmsg[0]);
            if (iRet == 0)
            {
                sendBuffer = DataDispose.sendOK();
            }
            else
            {
                sendBuffer = DataDispose.sendErr(new byte[] { 0, 1 });
                string errMsg = Encoding.Default.GetString(errmsg);
                SysLog.e("密钥写入失败:{0}", null, errMsg);
            }
            backData(sendBuffer);
        }

        /// <summary>
        /// 下载工作密钥
        /// </summary>
        /// <param name="buffer"></param>
        private void LoadWkey(byte[] buffer)
        {
            List<byte[]> lParmsArry = DataDispose.unPackData(buffer, 4);
            int MKeyIndex = lParmsArry[0].ByteArrayToIntH();
            int WKeyIndex = lParmsArry[1].ByteArrayToIntH();
            int EncryType = lParmsArry[2].ByteArrayToIntH();
            int KeyType = lParmsArry[3].ByteArrayToIntH();
            byte[] key = lParmsArry[4];
            byte[] errmsg = new byte[70];
            byte[] sendBuffer = null;

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
                sendBuffer = DataDispose.sendOK();
            }
            else
            {
                sendBuffer = DataDispose.sendErr(new byte[] { 0, 1 });
                string err = errmsg.GetString();
                SysLog.e("下载工作密钥失败：{0}", null, err);
            }
            backData(sendBuffer);
        }
    }
}
