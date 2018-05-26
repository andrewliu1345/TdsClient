using ABC.abstractFun;
using ABC.DeviceApi;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Logs;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ABC.BackSplint
{
    /// <summary>
    /// ID卡
    /// </summary>
    public class IDCard : aFuns
    {


        public override void SetData(byte[] buffer)
        {
            GetFpIDCard();
        }

        private void GetBaseIDCard()
        {
            backData(null);
        }
        private void GetFpIDCard()
        {
            byte[] bSendBuff = null;
            if (DeviceIDs.ReadCard_fd <= 0)
            {
                bSendBuff = DataDispose.sendErr(new byte[] { 0, 1 });//超时
               
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
                        bSendBuff = DataDispose.sendErr(new byte[] { 0, 2 });//超时
                        break;
                    }
                    StringBuilder stringBuilder = new StringBuilder(70);
                    int iRet = BSApiHelper.IDCard_ReadCard_finger(DeviceIDs.ReadCard_fd, stringBuilder);
                    if (iRet == 0)
                    {
                        string msg = stringBuilder.ToString();
                        SysLog.d("获取身份证返回{0}", null, msg);
                        bSendBuff = DataDispose.sendOK();
                    }

                }
            }
            backData(bSendBuff);
        }
    }
}
