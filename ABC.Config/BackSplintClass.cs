using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;

namespace ABC.Config
{
    public class BackSplintClass : abstractSerialPort
    {
        
        public BackSplintClass()
        {
             _configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            string config = _configuration.AppSettings.Settings[BACK_SPLINT].Value;
            string[] SConfValueArray = config.Split(',');
            com = Convert.ToInt32(SConfValueArray[0]);
            baud = Convert.ToInt32(SConfValueArray[1]);
        }

     

        public override void Save()
        {
            //Configuration _configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            string sConfig = string.Format("{0},{1}", com, baud);
            _configuration.AppSettings.Settings[BACK_SPLINT].Value = sConfig;
            _configuration.Save();
        }




    }
}
