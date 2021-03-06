﻿using System;
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
        /// <summary>
        /// 
        /// </summary>
        /// <param name="src"></param>
        /// <param name="length"></param>
        /// <returns></returns>
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
        public static String bytesToHexString(this byte[] src)
        {
            int length = src.Length;
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

        /// <summary>
        /// byte数组转Int 高位要前
        /// </summary>
        /// <param name="b"></param>
        /// <returns></returns>
        public static int ToIntH(this byte[] b)
        {
            int sum = 0;
            if (b.Equals(null))
            {
                return sum;
            }

            int ilen = b.Length;
            for (int i = 0; i < ilen; i++)
            {
                sum += (b[i] << (8 * (ilen - 1 - i))) & 0xFFFF;
            }
            return sum;
        }

        /// <summary>
        /// byte数组转Int 低位要前
        /// </summary>
        /// <param name="b"></param>
        /// <returns></returns>
        public static int ToIntL(this byte[] b)
        {
            int sum = 0;
            if (b.Equals(null))
            {
                return sum;
            }

            int ilen = b.Length;
            for (int i = 0; i < ilen; i++)
            {
                sum += (b[i] << (8 * (i - 1))) & 0xFFFF;
            }
            return sum;
        }
        public static string GetString(this byte[] b)
        {
            return Encoding.Default.GetString(b);
        }
        public static string GetString(this byte[] b, string s)
        {
            return Encoding.GetEncoding(s).GetString(b);
        }

        public static byte[] ToByteArry(this string str)
        {
            return System.Text.Encoding.Default.GetBytes(str);
        }
    }
}
