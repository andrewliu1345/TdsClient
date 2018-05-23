using ABC.abstractFun;
using ABC.BackSplint;
using ABC.Listener;
using System;
using System.Net.Sockets;

namespace ABC.Factory
{
    public class FunFactory : iCallBackListenner
    {
        public Socket NetSocket
        {
            set
            {
                m_socket = value;
            }
        }
        Socket m_socket;
        public void SetData(byte[] buffer, int length)
        {
            aFuns fun = GetFuns(buffer);
            if (fun != null)
            {
                fun.SetBackListener(this);
                fun.SetData(buffer);
            }
        }
        public aFuns GetFuns(byte[] buffer)
        {

            return null;
        }

        public void backData(byte[] buffer)
        {
            m_socket.Send(buffer);
        }

        public void isLegal(byte[] cmd)
        {
            throw new NotImplementedException();
        }
    }
}
