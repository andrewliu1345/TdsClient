using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Config
{
    public class IniFileRead
    {



        const string SERVER = "SERVER";
        const string BACK_SPLINT = "BACK_SPLINT";
        const string PRINTER = "PRINTER";
        const string _IP = "ip";
        const string _PORT = "port";
        const string COM = "COM";
        const string BAUD = "BAUD";
        private static readonly IniFileRead m_instance = new IniFileRead();
        static IniFileHelper inf;
        const string file_path = "Dev.cfg";
        /// <summary>
        /// 单例
        /// </summary>
        /// <returns></returns>
        public static IniFileRead Instance
        {
            get
            {
                return m_instance;
            }

        }

        private IniFileRead()
        {
            inf = new IniFileHelper(file_path);
        }


        /// <summary>
        /// 串口
        /// </summary>
        public class SocketParameterClass
        {
            #region socket
            /// <summary>
            /// IP地址
            /// </summary>
            public string IP
            {
                get
                {
                    string _ip = "127.0.0.1";
                    _ip = inf.IniReadValue(SERVER, _IP, "0.0.0.0");
                    Logs.SysLog.i(string.Format("获取ip={0}", _ip));
                    return _ip;
                }
                set
                {
                    string _ip;

                    if (value == string.Empty || value.Equals(null))
                    {
                        _ip = "0.0.0.0";
                    }
                    else
                    {
                        _ip = value;
                    }
                    Logs.SysLog.i(string.Format("写入ip={0}", _ip));
                    inf.IniWriteValue(SERVER, _IP, value);
                }
            }

            /// <summary>
            /// 端口
            /// </summary>
            public int PORT
            {
                get
                {
                    int _port = 9988;
                    var str = inf.IniReadValue(SERVER, _PORT, "9988");
                    Logs.SysLog.i(string.Format("获取port={0}", str));
                    if (str.Equals(null) || str == null)
                    {
                        _port = 0;
                    }
                    else
                    {
                        _port = int.Parse(str);
                    }
                    return _port;
                }
                set
                {
                    int _port = value;
                    Logs.SysLog.i(string.Format("写入port={0}", _port));
                    inf.IniWriteValue(SERVER, _PORT, _port.ToString());
                }
            }
            #endregion
        }

        /// <summary>
        /// 背夹
        /// </summary>
        public class BackSplintClass
        {
            #region  背夹串口
            /// <summary>
            /// 背夹串口号
            /// </summary>
            public int Com
            {
                get
                {
                    int _com = 11;
                    var str = inf.IniReadValue(BACK_SPLINT, IniFileRead.COM, "11");
                    Logs.SysLog.i(string.Format("获取COM={0}", str));
                    if (str.Equals(null) || str == null)
                    {
                        _com = 0;
                    }
                    else
                    {
                        _com = int.Parse(str);
                    }
                    return _com;
                }
                set
                {
                    int _com = value;
                    Logs.SysLog.i(string.Format("写入ip={0}", _com));
                    inf.IniWriteValue(BACK_SPLINT, IniFileRead.COM, _com.ToString());
                }
            }

            /// <summary>
            /// 波特率
            /// </summary>
            public int Baud
            {
                get
                {
                    int _baud = 11;
                    var str = inf.IniReadValue(BACK_SPLINT, IniFileRead.BAUD, "11");
                    Logs.SysLog.i(string.Format("获取COM={0}", str));
                    if (str.Equals(null) || str == null)
                    {
                        _baud = 0;
                    }
                    else
                    {
                        _baud = int.Parse(str);
                    }
                    return _baud;
                }
                set
                {
                    int _baud = value;
                    Logs.SysLog.i(string.Format("写入ip={0}", _baud));
                    inf.IniWriteValue(BACK_SPLINT, IniFileRead.BAUD, _baud.ToString());
                }
            }
            #endregion
        }

        /// <summary>
        /// 打印机
        /// </summary>
        public class PrinterClass
        {
            #region  背夹串口
            /// <summary>
            /// 背夹串口号
            /// </summary>
            public int Com
            {
                get
                {
                    int _com = 11;
                    var str = inf.IniReadValue(PRINTER, IniFileRead.COM, "11");
                    Logs.SysLog.i(string.Format("获取_com={0}", str));
                    if (str.Equals(null) || str == null)
                    {
                        _com = 0;
                    }
                    else
                    {
                        _com = int.Parse(str);
                    }
                    return _com;
                }
                set
                {
                    int _com = value;
                    Logs.SysLog.i(string.Format("写入_com={0}", _com));
                    inf.IniWriteValue(PRINTER, IniFileRead.COM, _com.ToString());
                }
            }

            /// <summary>
            /// 波特率
            /// </summary>
            public int Baud
            {
                get
                {
                    int _baud = 11;
                    var str = inf.IniReadValue(PRINTER, IniFileRead.BAUD, "11");
                    Logs.SysLog.i(string.Format("获取_baud={0}", str));
                    if (str.Equals(null) || str == null)
                    {
                        _baud = 0;
                    }
                    else
                    {
                        _baud = int.Parse(str);
                    }
                    return _baud;
                }
                set
                {
                    int _baud = value;
                    Logs.SysLog.i(string.Format("写入_baud={0}", _baud));
                    inf.IniWriteValue(PRINTER, IniFileRead.BAUD, _baud.ToString());
                }
            }
            #endregion
        }


        public PrinterClass Printer
        {
            get
            {
                return new PrinterClass();
            }
        }

        public SocketParameterClass SocketParameter
        {
            get
            {
                return new SocketParameterClass();
            }
        }

        public BackSplintClass BackSplint
        {
            get
            {
                return new BackSplintClass();
            }
        }
    }
}
