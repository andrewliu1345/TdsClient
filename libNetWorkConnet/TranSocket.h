#pragma once
#include <winsock2.h>  
#include <thread>
#include "CSocketDelegete.h"
class TranSocket
{
public:


	//************************************
	// Method:    单例
	// FullName:  TranSocket::GetInstance
	// Access:    public static 
	// Returns:   TranSocket*
	// Qualifier:
	//************************************
	static TranSocket* GetInstance();

	bool GetIsConnected();

	//************************************
	// Method:    WriteData
	// FullName:  TranSocket::WriteData
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: unsigned char * buffer
	// Parameter: int length
	//************************************
	int WriteData(unsigned char * buffer, int length);

	//************************************
	// Method:    读数据
	// FullName:  TranSocket::ReadData
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: unsigned char * refbuffer 读取到的数据
	// Parameter: int * reflegth 读取到的数据的长度
	// Parameter: unsigned long timeout 超时时间
	//************************************
	int ReadData(unsigned char * refbuffer, int* reflegth, unsigned long timeout);


	int ReadData(CSocketDelegete * socketDelegete, int timeout);
	~TranSocket();
private:
	static bool isConnected;
	static TranSocket *m_instance;
	static sockaddr_in serAddr;
	TranSocket();
	static int Connet();
	static SOCKET sclient;
	static unsigned __stdcall Heart_Thead(LPVOID lpParameter);
	HANDLE hThread;
	static HANDLE hReadThread;
	static HANDLE g_hMutex;//互斥量
	static UINT g_dwDefThreadId;//异步处理线程ID。
	static UINT g_ReadThreadId;
	static UCHAR heartData[8];
	static int _write(const char* buffer, int length);
	static int _read(char* refbuffer, int *length, int timeout);
	//static CSocketDelegete *socketDeleget;
	static unsigned __stdcall Read_Thead(LPVOID lpParameter);
};

