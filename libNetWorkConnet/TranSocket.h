#pragma once
#include <winsock2.h>  
#include <thread>
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
	~TranSocket();
private:
	static bool isConnected;
	static TranSocket *m_instance;
	static sockaddr_in serAddr;
	TranSocket();
	static int Connet();
	static SOCKET sclient;
	static DWORD WINAPI Heart_Thead(LPVOID lpParameter);
	HANDLE hThread;
	static HANDLE g_hMutex;//������
	static unsigned char heartData[8];
	static int _write(const char* buffer, int length);
	static int _read(char* refbuffer, int *length,int timeout);
	
};
