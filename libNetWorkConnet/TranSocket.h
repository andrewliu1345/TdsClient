#pragma once
#include <winsock2.h>  
#include <thread>
#include "CSocketDelegete.h"
class TranSocket
{
public:


	//************************************
	// Method:    ����
	// FullName:  TranSocket::GetInstance
	// Access:    public static 
	// Returns:   TranSocket*
	// Qualifier:
	//************************************
	static TranSocket* GetInstance();
	static TranSocket* GetInstance(int _binport);
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
	// Method:    ������
	// FullName:  TranSocket::ReadData
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: unsigned char * refbuffer ��ȡ��������
	// Parameter: int * reflegth ��ȡ�������ݵĳ���
	// Parameter: unsigned long timeout ��ʱʱ��
	//************************************
	int ReadData(unsigned char * refbuffer, int* reflegth, unsigned long timeout);


	int ReadData(CSocketDelegete * socketDelegete, int timeout);

	
	//************************************
	// Method:    ���ݴ���
	// FullName:  TranSocket::TranData
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: unsigned char * buffer
	// Parameter: int length
	// Parameter: unsigned char * refbuffer
	// Parameter: int * reflegth
	// Parameter: unsigned long timeout
	//************************************
	int TranData(unsigned char * buffer, int length, unsigned char * refbuffer, int* reflegth, unsigned long timeout);

	//************************************
	// Method:    ���ݴ��䣨�첽��
	// FullName:  TranSocket::SyncTranData
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: unsigned char * buffer
	// Parameter: int length
	// Parameter: CSocketDelegete * socketDelegete
	// Parameter: int timeout
	//************************************
	int SyncTranData(unsigned char * buffer, int length, CSocketDelegete * socketDelegete, int timeout);
	~TranSocket();
private:
	static bool isConnected;
	static TranSocket *m_instance;
	static sockaddr_in serAddr;
	static sockaddr_in binAddr;
	static int bind_port;
	TranSocket();
	static int Connet();
	static int unConnet();
	static int CloseSocket(__in SOCKET *s);
	static SOCKET sclient;
	static unsigned __stdcall Heart_Thead(LPVOID lpParameter);
	static HANDLE hThread;
	static HANDLE hReadThread;
	static HANDLE hFlushThread;
	static HANDLE g_hMutex;//������
	static UINT g_dwDefThreadId;//�첽�����߳�ID��
	static UINT g_ReadThreadId;
	static UINT g_FlushThreadId;
	static UCHAR heartData[8];
	static int _write(const char* buffer, int length);
	static int _read(char* refbuffer, int *length, int timeout);
	//static CSocketDelegete *socketDeleget;
	static unsigned __stdcall Read_Thead(LPVOID lpParameter);
	static unsigned __stdcall Flush_Thead(LPVOID lpParameter);
	void Flush();

};

