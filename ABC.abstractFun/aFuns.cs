using ABC.Enity;
using ABC.HelperClass;
using ABC.Listener;

namespace ABC.abstractFun
{
    public abstract class aFuns
    {
        byte[] bcmd = new byte[3];
        protected int m_TimeOut = 30000;
        protected iCallBackListenner m_callBackListenner;
        public virtual void SetData(byte[] buffer)
        {
            SetCMD(buffer);
        }
        private void SetCMD(byte[] buffer)
        {
            byte[] cmd = new byte[3];
            System.Array.Copy(buffer, 3, cmd, 0, 3);
            bcmd = cmd;
        }
        public int SetTimeOut
        {
            set
            {
                m_TimeOut = value;
            }

        }
        public int GetDevice()
        {
            return 1;
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
        //protected void backData(byte[] buffer)
        //{
        //    if (m_callBackListenner != null)
        //    {
        //        m_callBackListenner.backData(buffer);
        //    }
        //}
        /// <summary>
        /// 返回数据
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="lenght"></param>
        protected void backData(byte[] buffer, int lenght)
        {
            if (m_callBackListenner != null)
            {
                byte[] bsendbuffer = DataDispose.toPackData(bcmd, Common.SUCCEE_CODE, buffer, lenght);
                m_callBackListenner.backData(bsendbuffer);
            }
        }
        protected void backErrData(byte[] errCode)
        {
            if (m_callBackListenner != null)
            {
                byte[] bsendbuffer = DataDispose.toPackData(bcmd, errCode, null, 0);
                m_callBackListenner.backData(bsendbuffer);
            }
        }
      
    }
}
