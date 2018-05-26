using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.HelperClass
{
    public static class DataConver
    {
        /// <summary>
        /// HexString转Byte
        /// </summary>
        /// <param name="s"></param>
        /// <returns></returns>
        public static byte[] HexString2ByteArray(this string s)
        {
            s = s.Replace(" ", "");
            byte[] buffer = new byte[s.Length / 2];
            for (int i = 0; i < s.Length; i += 2)
            {
                buffer[i / 2] = (byte)Convert.ToByte(s.Substring(i, 2), 16);
            }
            return buffer;
        }
        public static String bytesToHexString(this byte[] src, int length)
        {

            StringBuilder stringBuilder = new StringBuilder("");
            if (src == null || src.Length <= 0)
            {
                return null;
            }
            if (src.Length < length)
            {
                length = src.Length;
            }
            for (int i = 0; i < length; i++)
            {
                int v = src[i] & 0xFF;
                String hv = Convert.ToString(v, 16);
                if (hv.Length < 2)
                {
                    stringBuilder.Append(0);
                }
                stringBuilder.Append(hv);
                stringBuilder.Append(" ");
            }
            return stringBuilder.ToString().Trim();
        }
        public static byte[] intToByteArray(this int a)
        {
            return new byte[]{
                (byte) ((a >> 8) & 0xFF),
                (byte) (a & 0xFF)
        };
        }
    }
}
