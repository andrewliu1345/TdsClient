using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;

namespace ABC.Config
{
    public abstract class BaseConfig
    {
        public const string SERVER = "SERVER";
        public const string BACK_SPLINT = "BACK_SPLINT";
        public const string PRINTER = "PRINTER";
        public const string FORMSPATH = "FORMSPATH";
        public const string PHOTOPATH = "PHOTOPATH";
       // protected Configuration _configuration;
        public abstract void Save();

    }
}
