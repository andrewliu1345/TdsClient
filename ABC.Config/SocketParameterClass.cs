using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;

namespace ABC.Config
{
    public class SocketParameterClass : BaseConfig
    {
        static int _port = 9988;
        static string _ip = "127.0.0.1";
       
        public SocketParameterClass()
        {
            Configuration _configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            string config = _configuration.AppSettings.Settings[SERVER].Value;
            string[] SConfValueArray = config.Split(':');
            _ip = SConfValueArray[0];
            _port = Convert.ToInt32(SConfValueArray[1]);

        }


        public override void Save()
        {
            Configuration _configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            string sConfig = string.Format("{0}:{1}", _ip, _port);
            _configuration.AppSettings.Settings[SERVER].Value = sConfig;
            _configuration.Save();
        }
        #region socket
        /// <summary>
        /// IP地址
        /// </summary>
        public string IP
        {

            get
            {
                return _ip;
            }
            set
            {
                if (value == string.Empty || value.Equals(null))
                {
                    _ip = "0.0.0.0";
                }
                else
                {
                    _ip = value;
                }
            }
        }

        /// <summary>
        /// 端口
        /// </summary>
        public int PORT
        {
            get
            {
                return _port;
            }
            set
            {
                _port = value;

            }
        }
        #endregion
    }
}
