﻿using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;

namespace ABC.Config
{
    public class BackSplintClass : BaseConfig
    {
        static int _com = 2;
        static int _baud = 115200;
        public BackSplintClass()
        {
            Configuration _configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            string config = _configuration.AppSettings.Settings[BACK_SPLINT].Value;
            string[] SConfValueArray = config.Split(',');
            _com = Convert.ToInt32(SConfValueArray[0]);
            _baud = Convert.ToInt32(SConfValueArray[1]);
        }
        public override void Save()
        {
            Configuration _configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            string sConfig = string.Format("{0},{1}", _com, _baud);
            _configuration.AppSettings.Settings[BACK_SPLINT].Value = sConfig;
            _configuration.Save();
        }

        #region  背夹串口
        /// <summary>
        /// 背夹串口号
        /// </summary>
        public int Com
        {
            get
            {
                return _com;
            }
            set
            {
                _com = value;
            }
        }

        /// <summary>
        /// 波特率
        /// </summary>
        public int Baud
        {
            get
            {
                return _baud;
            }
            set
            {
                _baud = value;
            }
        }
        #endregion
    }
}
