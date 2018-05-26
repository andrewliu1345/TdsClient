using ABC.Listener;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.abstractFun
{
    public abstract class aFuns
    {
        protected int m_TimeOut = 30000;
        protected iCallBackListenner m_callBackListenner;
        public abstract void SetData(byte[] buffer);
        public int SetTimeOut
        {
            set
            {
                m_TimeOut = value;
            }

        }

        public iCallBackListenner SetBackListener
        {
            set
            {
                m_callBackListenner = value;
            }

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
