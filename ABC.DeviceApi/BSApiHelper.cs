using System.Runtime.InteropServices;
using System.Text;

namespace ABC.DeviceApi
{
    public class BSApiHelper
    {
        const string DllPath = "DLL\\mtx_32.dll";
        #region 背夹基本操作
        //打开设备
        [DllImport(DllPath, EntryPoint = "device_open")]
        public static extern int device_open(int port, int baud);

        //打开设备
        [DllImport(DllPath, EntryPoint = "device_open_name")]
        public static extern int device_open_name(string pPort, int auxPort, int baud);

        //关闭设备
        [DllImport(DllPath, EntryPoint = "device_close")]
        public static extern int device_close(int icdev);

        [DllImport(DllPath, EntryPoint = "dev_ledcontrol")]
        public static extern int dev_ledcontrol(int icdev, byte[] ledparam);

        /// <summary>
        /// 背夹状态
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="ndev_status"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "get_device_status")]
        public static extern int get_device_status(int icdev, ref byte ndev_status);

        /// <summary>
        /// 同步时间
        /// </summary>
        /// <param name="icdev"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "Set_RCT_Timer")]
        public static extern int Set_RCT_Timer(int icdev);

        /// <summary>
        /// LCD屏显示
        /// </summary>
        /// <param name="icdev">设备句柄</param>
        /// <param name="ControlBit">控制位       1-接触(最低位) 2-非接 3-二代证 4-磁头 5-指纹(最高位)  0x00011111表示指示灯全亮</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "LCD_Status_Show")]
        public static extern int LCD_Status_Show(int icdev, byte ControlBit);


        /// <summary>
        /// 读写器蜂鸣器控制
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="delaytime"></param>
        /// <param name="times"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "device_beep")]
        public static extern int device_beep(int icdev, int delaytime, int times);

        //获取读写器版本信息
        [DllImport(DllPath, EntryPoint = "device_version")]
        public static extern int device_version(int icdev, int module, ref byte verlen, ref byte verdata);
        #endregion
        #region 二代证

        /// <summary>
        /// 设置路径
        /// </summary>
        /// <param name="photoName"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "IDCard_SetPhotoPath")]
        public static extern int IDCard_SetPhotoPath(string photoName);

        /// <summary>
        /// 读取二代证
        /// </summary>
        /// <param name="icdev">句柄</param>
        /// <param name="message">失败消息</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "IDCard_ReadCard_finger")]
        public static extern int IDCard_ReadCard_finger(int icdev, StringBuilder message);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="icdev">句柄</param>
        /// <param name="index">索引值说明：0--姓名，1--性别，2--民族，3--出生日期，4--住址，5--公民身份号码，6--签发机
        ///关，7--有效期起始日期，8--有效期截止日期，9--预留区，10--照片文件名。</param>
        /// <param name="infodata">获取消息</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "IDCard_GetCardInfo")]
        public static extern int IDCard_GetCardInfo(int icdev, int index, ref byte infodata);

        /// <summary>
        /// 获取图片名
        /// </summary>
        /// <param name="cfileName"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "IDCard_GetPhotoFile")]
        public static extern int IDCard_GetPhotoFile(ref byte cfileName);
        #endregion
        #region 银行卡

        /// <summary>
        /// 读取金融IC 卡卡号跟姓名
        /// </summary>
        /// <param name="icdev">句柄</param>
        /// <param name="nCardType">卡片类型0 表示接触式CPU 卡、0xFF 表示非接触式CPU 卡，其他的不认可。</param>
        /// <param name="ICCardNo">金融IC 卡卡号</param>
        /// <param name="ICCardName">金融IC 卡姓名</param>
        /// <param name="lpErrMsg">错误信息</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "iReadICCardNoAndName")]
        public static extern int iReadICCardNoAndName(int icdev, int nCardType, ref byte ICCardNo, ref byte ICCardName, ref byte lpErrMsg);


        /// <summary>
        /// 接触式卡片上电复位
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="delaytime"></param>
        /// <param name="cardno"></param>
        /// <param name="rlen"></param>
        /// <param name="resetdata"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "sam_slt_reset")]
        public static extern int sam_slt_reset(int icdev, int delaytime, byte cardno, ref int rlen, ref byte resetdata);

        /// <summary>
        /// 激活非接触式卡
        /// </summary>
        /// <param name="icdev">通讯设备标识符</param>
        /// <param name="delaytime">delaytime 2 字节等待卡进入感应区时间（单位：毫秒），高位字节在前
        ///0:无需等待，无卡直接返回
        ///0xffff:一直等待</param>
        /// <param name="cardtype">1 字节卡类型0AH A 类卡； 0BH B 类卡</param>
        /// <param name="snrlen">1 字节卡UID 长度</param>
        /// <param name="snr">4 字节卡UID</param>
        /// <param name="rlen">1 字节ATR 长度 </param>
        /// <param name="recdata">recdata: ATR 应答数据</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "open_card")]
        public static extern int open_card(int icdev, int delaytime, ref byte cardtype, ref byte snrlen, ref byte snr, ref int rlen, ref byte recdata);

        /// <summary>
        /// 应用层传输命令
        /// </summary>
        /// <param name="icdev">通讯设备标识符</param>
        /// <param name="cardno">1 字节非接触式卡：FFH，接触式卡：00H-0FH，PSAM 卡：10H-1FH</param>
        /// <param name="slen">发送数据的字节长度</param>
        /// <param name="datasend">要发送的数据</param>
        /// <param name="rlen">返回的数据长度</param>
        /// <param name="datareceive">返回的指令应答信息</param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "card_APDU")]
        public static extern int card_APDU(int icdev, int cardno, int slen, byte[] datasend, ref int rlen, ref byte datareceive);

        #endregion
        #region 密码键盘
        
        /// <summary>
        /// 下载主密钥
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="mainkeyno"></param>
        /// <param name="encryptype"></param>
        /// <param name="keylen"></param>
        /// <param name="mainkey"></param>
        /// <param name="errmessage"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "LoadMasterkey")]
        public static extern int LoadMasterkey(int icdev, int mainkeyno, int encryptype, int keylen, ref byte mainkey, ref byte errmessage);

        /// <summary>
        /// 下载工作密钥
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="mainkeyno"></param>
        /// <param name="workkeylen"></param>
        /// <param name="workkey"></param>
        /// <param name="errmessage"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "LoadWorkkey")]
        public static extern int LoadWorkkey(int icdev, int mainkeyno, int workkeylen, ref byte workkey, ref byte errmessage);

        /// <summary>
        /// 导入PIN密钥
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="mainkeyno"></param>
        /// <param name="pinkeylen"></param>
        /// <param name="userkey"></param>
        /// <param name="errmessage"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "LoadPINkey")]
        public static extern int LoadPINkey(int icdev, int mainkeyno, int pinkeylen,ref byte userkey, ref byte errmessage);


        /// <summary>
        /// 导入MAC密钥	
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="mainkeyno"></param>
        /// <param name="mackeylen"></param>
        /// <param name=""></param>
        /// <param name="mackey"></param>
        /// <param name="errmessage"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "LoadMackey")]
        public static extern int LoadMackey(int icdev, int mainkeyno, int mackeylen,ref byte mackey,ref byte errmessage);

        /// <summary>
        /// 获取PinBlock
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="mainkeyno"></param>
        /// <param name="cardnolen"></param>
        /// <param name="cardno"></param>
        /// <param name="timeout"></param>
        /// <param name="pinlen"></param>
        /// <param name="pinblock"></param>
        /// <param name="errmessage"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "GetPIN")]
        public static extern int GetPinBlock(int icdev, int mainkeyno, int cardnolen,ref byte cardno, int timeout, ref int pinlen, ref byte pinblock, ref byte errmessage);

        /// <summary>
        /// 获取MAC
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="mainkeyno"></param>
        /// <param name="algmodel">0.XOR  1.ANSI9.9    2.ANSI9.19 </param>
        /// <param name="macsourcelen"></param>
        /// <param name="macsourcedata"></param>
        /// <param name="maclen"></param>
        /// <param name="mac"></param>
        /// <param name="errmessage"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "GetMAC")]
        public static extern int GetMAC(int icdev, int mainkeyno, int algmodel, int macsourcelen,ref  byte macsourcedata, ref int maclen, ref byte mac, ref byte errmessage);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="icdev"></param>
        /// <param name="timeout"></param>
        /// <param name="mag_card"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "magnetic_read_ms")]
        public static extern int magnetic_read(int icdev, int timeout, ref byte mag_card);

        /// <summary>
        /// 3DES加密函数
        /// </summary>
        /// <param name="key"></param>
        /// <param name="ptrSource"></param>
        /// <param name="msgLen"></param>
        /// <param name="ptrDest"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "des3_encrypt")]
        public static extern int des3_encrypt(ref byte key, ref byte ptrSource, int msgLen, ref byte ptrDest);

        /// <summary>
        /// 3Des解密函数
        /// </summary>
        /// <param name="key"></param>
        /// <param name="ptrSource"></param>
        /// <param name="msgLen"></param>
        /// <param name="ptrDest"></param>
        /// <returns></returns>
        [DllImport(DllPath, EntryPoint = "des3_decrypt")]
        public static extern int des3_decrypt(ref byte key, ref byte ptrSource, int msgLen, ref byte ptrDest);
        #endregion
    }
}
