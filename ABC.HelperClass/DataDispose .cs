﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.HelperClass
{
    public class DataDispose
    {
        ///// <summary>
        ///// 打包返回数据
        ///// </summary>
        ///// <param name="returnData">需要打包的数据</param>
        ///// <param name="writeLen">需要打包的数据的长度</param>
        ///// <returns>打包好的数据</returns>
        //public static byte[] toPackData(byte[] returnData, int writeLen)
        //{
        //    int len = writeLen + 2;
        //    byte[] Writebuffer = new byte[len + 5];
        //    Writebuffer[0] = 2;
        //    Writebuffer[1] = (byte)(len >> 8);
        //    Writebuffer[2] = (byte)(len % 256);
        //    Writebuffer[3] = 0;
        //    Writebuffer[4] = 0;

        //    System.Array.Copy(returnData, 0, Writebuffer, 5, writeLen);
        //    Writebuffer[5 + writeLen] = cr_bcc(returnData);
        //    Writebuffer[6 + writeLen] = 3;
        //    return Writebuffer;
        //}
        /// <summary>
        /// 新的返回格式
        /// </summary>
        /// <param name="cmd"></param>
        /// <param name="flag"></param>
        /// <param name="packdata"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static byte[] toPackData(byte[] cmd, byte[] flag, byte[] packdata, int length)
        {

            int tmplen = cmd.Length + flag.Length + length;
            byte[] rData = new byte[tmplen + 5];
            byte[] tmp = new byte[tmplen];
            int index = 0;
            System.Array.ConstrainedCopy(cmd, 0, tmp, index, cmd.Length);
            index += cmd.Length;
            System.Array.ConstrainedCopy(flag, 0, tmp, index, flag.Length);
            index += flag.Length;
            if (length != 0)
            {
                if (packdata.Length < length)
                {
                    return null;
                }
                System.Array.ConstrainedCopy(packdata, 0, tmp, index, length);
            }

            byte crc = cr_bcc(tmp);
            rData[0] = 2;
            rData[1] = (byte)(tmplen >> 8);
            rData[2] = (byte)(tmplen % 256);
            System.Array.ConstrainedCopy(tmp, 0, rData, 3, tmplen);
            rData[tmplen + 3] = crc;
            rData[tmplen + 4] = (byte)0x03;
            return rData;
        }
        /// <summary>
        /// 解包
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="mun"></param>
        /// <returns></returns>
        public static List<byte[]> unPackData(byte[] buffer, int mun)
        {
            byte[] bBuffLen = new byte[2];
            int iBuffler = 0;
            System.Array.Copy(buffer, 1, bBuffLen, 0, 2);
            iBuffler = bBuffLen.ToIntH();

            int index = 6;
            List<byte[]> list = new List<byte[]>();
            for (int i = 0; i < mun; i++)
            {
                byte[] bLen = new byte[2];
                int iLen = 0;
                System.Array.Copy(buffer, index, bLen, 0, 2);
                iLen = bLen.ToIntH();
                index += 2;

                byte[] bParm = new byte[iLen];
                System.Array.Copy(buffer, index, bParm, 0, iLen);
                index += iLen;
                list.Add(bParm);
                if (index >= iBuffler)
                {
                    break;
                }
            }
            return list;

        }
        private static byte cr_bcc(byte[] data)
        {
            byte temp = 0;
            foreach (var item in data)
            {
                temp ^= item;
            }
            return temp;
        }
        static byte[] ok = { (byte)0x02, (byte)0x00, (byte)0x02, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x03 };
        public static byte[] sendOK()
        {
            return ok;
        }
        /// <summary>
        /// 发送错误代码
        /// </summary>
        /// <param name="errCode"></param>
        /// <returns></returns>
        public static byte[] sendErr(byte[] errCode)
        {
            byte[] Writebuffer = new byte[7];
            Writebuffer[0] = 2;
            Writebuffer[1] = (byte)(2 >> 8);
            Writebuffer[2] = (byte)(2 % 256);
            Writebuffer[3] = errCode[0];
            Writebuffer[4] = errCode[1];
            Writebuffer[5] = cr_bcc(errCode);
            Writebuffer[6] = 3;
            return Writebuffer;
        }

    }
}
