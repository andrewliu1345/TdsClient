﻿using ABC.abstractFun;
using ABC.BackSplint;
using ABC.Listener;
using System;
using System.Net.Sockets;
using System.Configuration;
using ABC.Logs;
using ABC.HelperClass;
using System.Reflection;

namespace ABC.Factory
{
    public class FunFactory : iCallBackListenner
    {
        private static FunFactory m_instance = null;
        public static FunFactory Instance
        {
            get { return m_instance; }
        }
        private FunFactory()
        {
        }
        static FunFactory()
        {
            m_instance = new FunFactory();
        }
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
                fun.SetBackListener = this;
                fun.SetData(buffer);
            }
        }
        private aFuns GetFuns(byte[] buffer)
        {
            aFuns t = null;
            string sConfkey = GetTypeCode(buffer);

            string sConfValue = ConfigurationManager.AppSettings[sConfkey];
            string[] SConfValueArray = sConfValue.Split(';');
            string sNamespaceName = SConfValueArray[0];
            string sClassName = SConfValueArray[1];

            SysLog.d($"sConfkey={0}，sConfValue={1}", null, sConfkey, sConfValue);

            Assembly assembly = Assembly.Load(sNamespaceName);
            object obj = assembly.CreateInstance(sClassName);
            //ConfigurationManager.AppSettings[]; 
            if (obj is aFuns)
            {
                t = obj as aFuns;
            }
            return t;
        }

        public void backData(byte[] buffer)
        {
            string str = buffer.bytesToHexString(buffer.Length);
            string strend = string.Format("向发送：{0},发送的消息:{1}", m_socket.RemoteEndPoint, str);
            SysLog.d(strend);

            m_socket.Send(buffer);
        }

        public void isLegal(byte[] cmd)
        {
            throw new NotImplementedException();
        }
        private string GetTypeCode(byte[] buffer)
        {
            byte[] bcode = new byte[2];
            System.Array.Copy(buffer, 3, bcode, 0, 2);
            string scode = bcode.bytesToHexString(2);// DataConver.bytesToHexString(bcode, 2);
            return scode.ToUpper().Trim();
        }
    }
}
