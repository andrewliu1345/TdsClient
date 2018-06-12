using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;

namespace ABC.Config
{
    public class PathClass : BaseConfig
    {
        string formpath = string.Empty;
        string photopath = string.Empty;
        public PathClass()
        {
            Configuration _configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            formpath = _configuration.AppSettings.Settings[FORMSPATH].Value;
            photopath = _configuration.AppSettings.Settings[PHOTOPATH].Value;
        }
        public override void Save()
        {
            Configuration _configuration = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            _configuration.AppSettings.Settings[FORMSPATH].Value = formpath;
            _configuration.AppSettings.Settings[PHOTOPATH].Value = photopath;
            _configuration.Save();
        }

        public string FormPath
        {
            get => formpath;
            set => formpath = value;
        }
        public string PhotoPath
        {
            get => photopath;
            set => photopath = value;
        }
    }
}
