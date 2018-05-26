using System.Runtime.InteropServices;
using System.Text;

namespace ABC.Config
{
    public class IniFileHelper
    {
        public string path;

        [DllImport("kernel32", EntryPoint = "WritePrivateProfileString", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32", EntryPoint = "GetPrivateProfileString", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int GetPrivateProfileString(string section, string key, string def, ref byte retVal, int size, string filePath);

        public IniFileHelper(string INIPath)
        {
            this.path = INIPath;
        }
        /// <summary>
        /// 写入配置
        /// </summary>
        /// <param name="Section">节点</param>
        /// <param name="Key">键</param>
        /// <param name="defaultValue">值</param>
        public void IniWriteValue(string Section, string Key, string Value)
        {
            IniFileHelper.WritePrivateProfileString(Section, Key, Value, this.path);
        }
        /// <summary>
        /// 读取配置
        /// </summary>
        /// <param name="Section">节点</param>
        /// <param name="Key">键</param>
        /// <param name="defaultValue">值</param>
        /// <returns></returns>
        public string IniReadValue(string Section, string Key, string defaultValue = "")
        {
            byte[] buffer = new byte[256];
            //StringBuilder stringBuilder = new StringBuilder(256);
            //stringBuilder.Clear();
            int privateProfileString = IniFileHelper.GetPrivateProfileString(Section, Key, defaultValue, ref buffer[0], 256, this.path);
            string strValue = Encoding.Default.GetString(buffer).Replace("\0","");
            return strValue;
            //return stringBuilder.ToString();
        }
    }
}
