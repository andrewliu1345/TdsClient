using Emrys.FlashLog;
using System;

namespace ABC.Logs
{
    public class SysLog
    {
        static SysLog()
        {
            FlashLogger.Instance().Register();
        }

        ~SysLog()
        {
            FlashLogger.Instance().UnRegister();
        }
        /// <summary>
        /// 日志路径
        /// </summary>
       // private static string LogPath = string.Format("{0}\\ABCLog", Environment.CurrentDirectory);

        /// <summary>
        /// 调试信息
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="numStack"></param>
        public static void d(string msg, Exception ex = null, params object[] numStack)
        {
            var _s = string.Format(msg, numStack);
            FlashLogger.Debug(_s, ex);
        }
        /// <summary>
        /// 错误信息
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="numStack"></param>
        public static void e(string msg, Exception ex = null, params object[] numStack)
        {
            var _s = string.Format(msg, numStack);
            FlashLogger.Error(_s, ex);
        }
        /// <summary>
        /// 记录信息
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="numStack"></param>
        public static void i(string msg, Exception ex = null, params object[] numStack)
        {
            var _s = string.Format(msg, numStack);
            FlashLogger.Info(_s, ex);
        }

        /// <summary>
        /// 调试信息
        /// </summary>
        /// <param name="msg"></param>
        public static void d(string msg, Exception ex = null)
        {
            FlashLogger.Debug(msg, ex);
        }

        /// <summary>
        /// 错误信息
        /// </summary>
        /// <param name="msg"></param>
        public static void e(string msg, Exception ex = null)
        {
            FlashLogger.Error(msg, ex);
        }

        /// <summary>
        /// 记录信息
        /// </summary>
        /// <param name="msg"></param>
        public static void i(string msg, Exception ex = null)
        {
            FlashLogger.Info(msg, ex);
        }

    }
}
