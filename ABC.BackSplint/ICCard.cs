using ABC.abstractFun;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.BackSplint
{
    /// <summary>
    /// IC 卡
    /// </summary>
    public class ICCard : aFuns
    {
        public override void SetData(byte[] buffer)
        {
            throw new NotImplementedException();
        }
        /// <summary>
        /// 上电
        /// </summary>
        private void ICCardPowerOn(int timeout)
        {
            byte[] msg = new byte[1024];
            int length = 0;
        }
    }
}
