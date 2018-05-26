using System.Runtime.InteropServices;
using System.Text;

namespace ABC.DeviceApi
{
    public class BSApiHelper
    {
        const string DllPath = "DLL\\mtx_32.dll";
        //打开设备
        [DllImport(DllPath, EntryPoint = "device_open")]
        public static extern int device_open(int port, int baud);

        //打开设备
        [DllImport(DllPath, EntryPoint = "device_open_name")]
        public static extern int device_open_name(string pPort, int auxPort, int baud);

        //关闭设备
        [DllImport(DllPath, EntryPoint = "device_close")]
        public static extern int device_close(int icdev);

        /// <summary>
        /// 背夹状态
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="ndev_status"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "get_device_status")]
        public static extern int get_device_status(int icdev, ref byte ndev_status);

        /// <summary>
        /// 同步时间
        /// </summary>
        /// <param name="icdev"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "Set_RCT_Timer")]
        public static extern int Set_RCT_Timer(int icdev);

        /// <summary>
        /// 读取二代证
        /// </summary>
        /// <param name="icdev">句柄</param>
        /// <param name="message">失败消息</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "IDCard_ReadCard_finger")]
        public static extern int IDCard_ReadCard_finger(int icdev, StringBuilder message);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="icdev">句柄</param>
        /// <param name="index">索引值说明：0--姓名，1--性别，2--民族，3--出生日期，4--住址，5--公民身份号码，6--签发机
        ///关，7--有效期起始日期，8--有效期截止日期，9--预留区，10--照片文件名。</param>
        /// <param name="infodata">获取消息</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "IDCard_GetCardInfo")]
        public static extern int IDCard_GetCardInfo(int icdev, int index, StringBuilder infodata);

      
    }
}
