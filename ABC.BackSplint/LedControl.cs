using ABC.DeviceApi;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.BackSplint
{
    public enum LCDType
    {
        IC = 1,
        NFC = 2,
        IDCARD = 4,
        MAGNETICCARD = 8,
        FINGER=16,
    }
    public class LedControl
    {
        private static LedControl m_instance = null;
        static LedControl()
        {
            m_instance = new LedControl();
        }
        private LedControl()
        { }
        static int _fd = -1;
        public static LedControl Instance(int fd)
        {
            _fd = fd;
            return m_instance;
        }
        public void ClearALL()
        {
            BSApiHelper.LCD_Status_Show(_fd, 0);
        }

        public void ShowLCD(LCDType type)
        {
            byte t = (byte)(int)type;
            BSApiHelper.LCD_Status_Show(_fd, t);
        }
// 
//         public void ShowIC()
//         {
//             BSApiHelper.LCD_Status_Show(_fd, 1);
//         }
//         public void ShowNfc()
//         {
//             BSApiHelper.LCD_Status_Show(_fd, 2);
//         }
//         public void ShowID()
//         {
//             BSApiHelper.LCD_Status_Show(_fd, 3);
//         }
//         public void ShowMagneticCard()
//         {
//             BSApiHelper.LCD_Status_Show(_fd, 4);
//         }
    }
}
