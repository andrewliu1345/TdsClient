using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Enity
{
    /// <summary>
    /// 全局句柄
    /// </summary>
    public class DeviceIDs
    {
        /// <summary>
        /// 读卡器句柄
        /// </summary>
        public static int ReadCard_fd = -1;

        /// <summary>
        /// 打印机句柄
        /// </summary>
        public static int Print_fd = -1;
    }
}
