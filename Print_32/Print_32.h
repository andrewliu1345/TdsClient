#pragma once

/************************************************************************/
/* 打印机基本接口                                                       */
/************************************************************************/

enum Encoding
{
	 EAN_8 = 2,
	 EAN_13 = 3,
};

//************************************
// Method:    device_open
// FullName:  打开设备
// Access:    public 
// Returns:   HANDLE __stdcall 句柄
// Qualifier: 打开打印设备
// Parameter: unsigned char * port 端口号
// Parameter: unsigned long baud 波特率
//************************************
HANDLE __stdcall device_open(unsigned int port, unsigned long baud);

//************************************
// Method:    device_close
// FullName:  关闭设备
// Access:    public 
// Returns:   __int16 __stdcall 
// Qualifier:  关闭打印机端口
// Parameter: HANDLE icdev
//************************************
__int16 __stdcall device_close(HANDLE icdev);


//************************************
// Method:    Print_CHS
// FullName:  打印字符串
// Access:    public 
// Returns:   __int16 __stdcall
// Qualifier:
// Parameter: HANDLE icdev 句柄
// Parameter: unsigned char * str 需要打印的字符串
// Parameter: unsigned int length 长度
//************************************
__int16 __stdcall Print_CHS(HANDLE icdev, unsigned char * str, unsigned int length);



//************************************
// Method:    Print
// FullName:  发送打印指令
// Access:    public 
// Returns:   __int16 __stdcall
// Qualifier:
// Parameter: HANDLE icdev
// Parameter: unsigned char * str
// Parameter: unsigned int length 长度
//************************************
__int16 __stdcall Print(HANDLE icdev, unsigned char * str, unsigned int length);


//************************************
// Method:    打印图片
// FullName:  PrintPic
// Access:    public 
// Returns:   __int16 __stdcall
// Qualifier:
// Parameter: HANDLE icdev
// Parameter: unsigned char * photodata
// Parameter: unsigned int length
//************************************
__int16 __stdcall PrintPic(HANDLE icdev, unsigned char * photodata, unsigned int length);


//************************************
// Method:    PrintBarcode
// FullName:  PrintBarcode
// Access:    public 
// Returns:   __int16 __stdcall
// Qualifier: 打印一维码
// Parameter: HANDLE icdev
// Parameter: unsigned char * photodata
// Parameter: unsigned int length
//************************************
__int16 __stdcall PrintBarcode(HANDLE icdev, int enc, unsigned char * Data, unsigned int length);

