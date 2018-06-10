using ABC.Config;
using System.Threading;

namespace ABC.BT
{
    public abstract class iBluetooth
    {
        protected bool isClosed = true;
        protected int _Com = 0;
        protected int _Baud = 0;
        protected bool isConnet = false;
        protected Thread _Thread;
        protected static iBluetooth m_instance;
       

        private void LoadConfig(abstractSerialPort config)
        {
            _Com = config.Com;
            _Baud = config.Baud;
        }
        /// <summary>
        /// 开启线程
        /// </summary>
        /// <param name="config">加载参数</param>
        public void Start(abstractSerialPort config)
        {
            LoadConfig(config);
            if (_Thread == null)
            {
                isClosed = false;
                _Thread = new Thread(new ParameterizedThreadStart(Run));
                _Thread.IsBackground = true;
                _Thread.Start();
            }
            else if (!_Thread.IsAlive)
            {
                _Thread.Start();
            }
        }

        /// <summary>
        /// 重启线程
        /// </summary>
        public void RestBTConnect(abstractSerialPort config)
        {
            Stop();
            Start( config);
        }

        /// <summary>
        /// 关闭线程
        /// </summary>
        public void Stop()
        {
            isClosed = true;
            if (_Thread != null && _Thread.IsAlive)
            {
                _Thread.Abort();
                _Thread.Join();
                _Thread = null;
            }
        }

        /// <summary>
        /// 运行
        /// </summary>
        /// <param name="obj"></param>
        public abstract void Run(object obj);


    }
}
