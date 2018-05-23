using ABC.Config;
using ABC.Enity;
using ABC.HelperClass;
using ABC.Listener;
using ABC.Logs;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace ABC.Background
{
    //定义接收客户端发送消息的回调
    public delegate void ReceiveMsgCallBack(byte[] strReceive, int length);

    //定义发送文件的回调
    public delegate void SendFileCallBack(byte[] bf, int length);
    public class CommServer : iCallBackListenner
    {

        static CommServer m_instance = new CommServer();

        /// <summary>
        /// 单例
        /// </summary>
        /// <returns></returns>
        public static CommServer Instance
        {
            get
            {
                return m_instance;
            }

        }


        /// <summary>
        /// 接收回调
        /// </summary>
        public event ReceiveMsgCallBack receiveCallBack;

        /// <summary>
        /// 发送回调
        /// </summary>
        //public event SendFileCallBack sendCallBack;

        //用于通信的Socket
        Socket socketSend;
        //用于监听的SOCKET
        Socket socketWatch;

        //将远程连接的客户端的IP地址和Socket存入集合中
        Dictionary<string, Socket> dicSocket = new Dictionary<string, Socket>();

        //创建监听连接的线程
        Thread AcceptSocketThread;
        //接收客户端发送消息的线程
        Thread threadReceive;

        public void Start()
        {
            var _ip = IniFileRead.Instance.SocketParameter.IP;
            var _point = IniFileRead.Instance.SocketParameter.PORT;
            //当点击开始监听的时候 在服务器端创建一个负责监听IP地址和端口号的Socket
            socketWatch = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            //获取ip地址
            IPAddress ip = IPAddress.Parse(_ip);
            //创建端口号
            IPEndPoint point = new IPEndPoint(ip, _point);
            //绑定IP地址和端口号
            socketWatch.Bind(point);
            SysLog.i("监听成功");

            //开始监听:设置最大可以同时连接多少个请求
            socketWatch.Listen(2);

            //实例化回调
            //             receiveCallBack += ReceiveMsg;
            //             sendCallBack += SendMsg;

            //创建监听连接线程
            AcceptSocketThread = new Thread(new ParameterizedThreadStart(StartListen));
            AcceptSocketThread.IsBackground = true;
            AcceptSocketThread.Start(socketWatch);
        }

        /// <summary>
        /// 等待客户端的连接，并且创建与之通信用的Socket
        /// </summary>
        /// <param name="obj"></param>
        private void StartListen(object obj)
        {
            Socket socketWatch = obj as Socket;
            while (true)
            {
                //等待客户端的连接，并且创建一个用于通信的Socket
                socketSend = socketWatch.Accept();
                //获取远程主机的ip地址和端口号
                string strIp = socketSend.RemoteEndPoint.ToString();
                if (dicSocket.ContainsKey(strIp))
                {
                    dicSocket[strIp] = socketSend;//修改
                }
                else
                {
                    dicSocket.Add(strIp, socketSend);//添加
                }

                //  this.cmb_Socket.Invoke(setCmbCallBack, strIp);
                string strMsg = "远程主机：" + socketSend.RemoteEndPoint + "连接成功";
                SysLog.i(strMsg);
                //使用回调
                // txt_Log.Invoke(setCallBack, strMsg);

                //定义接收客户端消息的线程
                Thread threadReceive = new Thread(new ParameterizedThreadStart(Receive));
                threadReceive.IsBackground = true;
                threadReceive.Start(socketSend);

            }
        }
        /// <summary>
        /// 服务器端不停的接收客户端发送的消息
        /// </summary>
        /// <param name="obj"></param>
        private void Receive(object obj)
        {
            ConnectDev();
            // int n = 0;
            Socket socketSend = obj as Socket;
            byte[] buffer = new byte[4096];
            while (true)
            {
                try
                {
                    //客户端连接成功后，服务器接收客户端发送的消息

                    //实际接收到的有效字节数

                    int count = socketSend.Receive(buffer);

                    if (count == 0)//count 表示客户端关闭，要退出循环
                    {
                        continue;
                    }
                    else
                    {
                        string str = DataConver.bytesToHexString(buffer, count);
                        string strReceiveMsg = "接收：" + socketSend.RemoteEndPoint + "发送的消息:" + str;
                        SysLog.i(strReceiveMsg);
                        //socketSend.Send(new byte[1] { (byte)++n });
                        if (receiveCallBack == null)
                        {
                            continue;
                        }
                        receiveCallBack(buffer, count);


                        //txt_Log.Invoke(receiveCallBack, strReceiveMsg);
                    }
                }
                catch (System.Exception ex)
                {
                    SysLog.e("链接出错", ex);
                    break;
                }

            }
        }


        public void backData(byte[] buffer)
        {
            throw new System.NotImplementedException();
        }

        public void isLegal(byte[] cmd)
        {
            throw new System.NotImplementedException();
        }

        /// <summary>
        /// 连接设备
        /// </summary>
        private void ConnectDev()
        {
            int comBs = IniFileRead.Instance.BackSplint.Com;
            int baudBs = IniFileRead.Instance.BackSplint.Baud;

            int comPrint = IniFileRead.Instance.Printer.Com;
            int baudPrint = IniFileRead.Instance.Printer.Baud;

            if (DeviceIDs.ReadCard_fd > 0)
            {
                DeviceApi.BSApiHelper.device_close(DeviceIDs.ReadCard_fd);
                DeviceIDs.ReadCard_fd = -1;
            }
            DeviceIDs.ReadCard_fd = DeviceApi.BSApiHelper.device_open(comBs - 1, baudBs);
            if (DeviceIDs.Print_fd > 0)
            {
                DeviceApi.PrintApiHelper.device_close_print(DeviceIDs.Print_fd);
                DeviceIDs.Print_fd = -1;
            }
            DeviceIDs.Print_fd = DeviceApi.PrintApiHelper.device_open_print(comPrint, baudPrint);
           
        }
    }
}
