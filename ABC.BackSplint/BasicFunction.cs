using ABC.abstractFun;
using ABC.HelperClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.BackSplint
{
    public class BasicFunction : aFuns
    {
        public override void SetData(byte[] buffer)
        {
            byte[] sendBuffer = DataDispose.sendOK();//返回心跳确认包
            backData(sendBuffer);
        }
    }
}
