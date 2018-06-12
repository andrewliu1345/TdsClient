using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Config
{
    public class AppConfig
    {
        private static readonly AppConfig m_instance = null;
        public static AppConfig Instance
        {
            get
            {
                return m_instance;
            }
        }
        static AppConfig()
        {
            m_instance = new AppConfig();
        }
        private AppConfig()
        { }

        
        public PrinterClass Printer
        {
            get
            {
                return new PrinterClass();
            }
        }

        public SocketParameterClass SocketParameter
        {
            get
            {
                return new SocketParameterClass();
            }
        }

        public BackSplintClass BackSplint
        {
            get
            {
                return new BackSplintClass();
            }
        }
        public PathClass Path
        {
            get => new PathClass();
        }
    }
}
