// Print_32.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Print_32.h"


HANDLE __stdcall device_open(unsigned int  port, unsigned long baud)
{

	wchar_t szDCBparam[50] = { 0 };
	swprintf_s(szDCBparam, L"baud=%d parity=N data=8 stop=1", baud);

	wchar_t szPort[50] = { 0 };
	swprintf_s(szPort, L"\\\\.\\COM%d", port);//打开10以上串口

	/**打开串口*/
	HANDLE m_hComm = CreateFile(szPort,  /** 设备名,COM1,COM2等 */
		GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */
		0,                            /** 共享模式,0表示不共享 */
		NULL,                         /** 安全性设置,一般使用NULL */
		OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */
		0,
		0);
	bool bIsSuccess = false;
	if (m_hComm != INVALID_HANDLE_VALUE)
	{

		/** 设置串口的超时时间,均设为0,表示不使用超时限制 */
		COMMTIMEOUTS  CommTimeouts;
		CommTimeouts.ReadIntervalTimeout = 0;
		CommTimeouts.ReadTotalTimeoutMultiplier = 0;
		CommTimeouts.ReadTotalTimeoutConstant = 0;
		CommTimeouts.WriteTotalTimeoutMultiplier = 0;
		CommTimeouts.WriteTotalTimeoutConstant = 0;

		bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);
		DCB  dcb;
		if (bIsSuccess)
		{
			/** 获取当前串口配置参数,并且构造串口DCB参数 */
			bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(szDCBparam, &dcb);
			/** 开启RTS flow控制 */
			dcb.fRtsControl = RTS_CONTROL_ENABLE;
		}
		if (bIsSuccess)
		{
			/** 使用DCB参数配置串口状态 */
			bIsSuccess = SetCommState(m_hComm, &dcb);
		}
		/**  清空串口缓冲区 */
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}
	return m_hComm;
}

__int16 __stdcall device_close(HANDLE icdev)
{
	if (icdev != INVALID_HANDLE_VALUE)
	{
		CloseHandle(icdev);
		icdev = INVALID_HANDLE_VALUE;
	}
	return 0;
}

__int16 __stdcall Print_CHS(HANDLE icdev, unsigned char * str, unsigned int length)
{
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;

	bResult = WriteFile(icdev, str, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** 清空串口缓冲区 */
		PurgeComm(icdev, PURGE_RXCLEAR | PURGE_RXABORT);
		return -1;
	}
	return 0;
}

__int16 __stdcall Print(HANDLE icdev, unsigned char * str, unsigned int length)
{
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;

	bResult = WriteFile(icdev, str, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** 清空串口缓冲区 */
		PurgeComm(icdev, PURGE_RXCLEAR | PURGE_RXABORT);
		return -1;
	}
	return 0;
}

__int16 __stdcall PrintPic(HANDLE icdev, unsigned char * photodata, unsigned int length)
{
	int include = 0;
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;
	unsigned char * printbuffer = new unsigned char[length + 4];
	memset(printbuffer, 0, length + 4);
	printbuffer[include++] = 0x1f;
	printbuffer[include++] = 0x10;
	printbuffer[include++] = length & 0xff;//XL 低位
	printbuffer[include++] = (length & 0xff00) >> 8;//XH 高位
	memcpy(&printbuffer[include], photodata, length);
	include += length;
	bResult = WriteFile(icdev, printbuffer, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** 清空串口缓冲区 */
		PurgeComm(icdev, PURGE_RXCLEAR | PURGE_RXABORT);
		return -1;
	}
	return 0;
}

__int16 __stdcall PrintBarcode(HANDLE icdev, int enc, unsigned char * Data, unsigned int length)
{
	int include = 0;
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;
	unsigned char * printbuffer = new unsigned char[length + 4];
	memset(printbuffer, 0, length + 4);
	printbuffer[include++] = 0x1d;
	printbuffer[include++] = 0x6d;
	printbuffer[include++] = enc;
	printbuffer[include++] =length & 0xff;//XH 高位
	memcpy(&printbuffer[include], Data, length);
	include += length;
	bResult = WriteFile(icdev, printbuffer, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		/** 清空串口缓冲区 */
		PurgeComm(icdev, PURGE_RXCLEAR | PURGE_RXABORT);
		return -1;
	}
	return 0;
}
