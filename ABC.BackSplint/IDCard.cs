using ABC.abstractFun;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.BackSplint
{
    public class IDCard : aFuns
    {


        public override void SetData(byte[] buffer)
        {

        }

        private void GetBaseIDCard()
        {
            backData(null);
        }
        private void GetFpIDCard()
        {
            backData(null);
        }
    }
}
