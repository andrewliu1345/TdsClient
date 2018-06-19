using ABC.Config;
using System.Threading;
using System.Threading.Tasks;

namespace ABC.BT
{
    public abstract class iBluetooth
    {

        protected static bool isClosed = true;
        protected int _Com = 0;
        protected int _Baud = 0;
        protected bool isConnet = false;
        protected static iBluetooth m_instance;

        private Task _task;
        protected CancellationTokenSource cts;

        private void LoadConfig(abstractSerialPort config)
        {
            _Com = config.Com;
            _Baud = config.Baud;
        }
        /// <summary>
        /// 开启线程
        /// </summary>
        /// <param name="config">加载参数</param>
        public virtual void Start(abstractSerialPort config)
        {
            cts = new CancellationTokenSource();
            LoadConfig(config);
            isClosed = false;

            TaskFactory _taskFactory = new TaskFactory();
            _task = _taskFactory.StartNew(Run, cts.Token);


            //             LoadConfig(config);
            //             if (_Thread == null)
            //             {
            //                 isClosed = false;
            //                 _Thread = new Thread(new ParameterizedThreadStart(Run));
            //                 _Thread.IsBackground = true;
            //                 _Thread.Start();
            //             }
            //             else if (!_Thread.IsAlive)
            //             {
            //                 _Thread.Start();
            //             }
        }

        /// <summary>
        /// 重启线程
        /// </summary>
        public  void RestBTConnect(abstractSerialPort config)
        {
            Stop();
            Start(config);
        }

        /// <summary>
        /// 关闭线程
        /// </summary>
        public  void Stop()
        {
            isClosed = true;
            //this.isConnet = false;
            //cts.Cancel();//取消任务
            //Thread.Sleep(10000);
            if (_task != null)
            {
                Task.WaitAll(_task);
            }

            //             if (_Thread != null && _Thread.IsAlive)
            //             {
            //                 _Thread.Abort();
            //                 _Thread.Join();
            //                 _Thread = null;
            //             }
        }

        /// <summary>
        /// 运行
        /// </summary>
        /// <param name="obj"></param>
        public abstract void Run(object obj);


    }
}
