using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Listener
{
    public interface iIoListener
    {
        void readBuffer(byte[] buffer, int length);
        void writeBuffer(byte[] buffer, int length);
    }
}
