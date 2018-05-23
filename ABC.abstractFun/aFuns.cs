using ABC.Listener;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.abstractFun
{
    public abstract class aFuns
    {
      protected iCallBackListenner m_callBackListenner;
    public abstract void SetData(byte[] buffer);

    public void SetBackListener(iCallBackListenner callBackListenner)
    {
        m_callBackListenner = callBackListenner;
    }
    /// <summary>
    /// 返回数据
    /// </summary>
    /// <param name="buffer"></param>
    protected void backData(byte[] buffer)
    {
        if (m_callBackListenner != null)
        {
            m_callBackListenner.backData(buffer);
        }
    }
}
}
