using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Listener
{
    /// <summary>
    /// 数据返回接口
    /// </summary>
    public interface iCallBackListenner
    {
         void backData(byte[] buffer);
         void isLegal(byte[] cmd);
    }
}
