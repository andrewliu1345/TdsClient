namespace ABC.Enity
{
    /// <summary>
    /// 全局句柄
    /// </summary>
    public class DeviceIDs
    {
        private static object rc_lock = new object();
        private static object print_lock = new object();
        private static int rc_fd = -1;
        private static int print_fd = -1;
        /// <summary>
        /// 读卡器句柄
        /// </summary>
        public static int ReadCard_fd
        {
            get
            {
                //lock (rc_lock)
                    return rc_fd;
            }
            set
            {
               // lock (rc_lock)
                    rc_fd = value;
            }
        }

        /// <summary>
        /// 打印机句柄
        /// </summary>
        public static int Print_fd
        {
            get
            {
                //lock (print_lock)
                    return print_fd;
            }
            set
            {
                //lock (print_lock)
                    print_fd = value;
            }
        }
    }
}
