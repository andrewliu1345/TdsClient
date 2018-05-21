using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ABC.Config
{
    public class IniFileHelper
    {
        public string path;

        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

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
            StringBuilder stringBuilder = new StringBuilder(256);
            int privateProfileString = IniFileHelper.GetPrivateProfileString(Section, Key, defaultValue, stringBuilder, 256, this.path);
            return stringBuilder.ToString();
        }
    }
}
