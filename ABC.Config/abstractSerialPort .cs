using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Config
{
    public abstract class abstractSerialPort : BaseConfig
    {
        protected int com;

        /// <summary>
        /// 背夹串口号
        /// </summary>
        public int Com
        {
            get => com;
            set => com = value;
        }

        protected int baud;

        /// <summary>
        /// 波特率
        /// </summary>
        public int Baud
        {
            get => baud;
            set => baud = value;
        }
    }
}
