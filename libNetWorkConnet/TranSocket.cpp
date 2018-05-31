#include "stdafx.h"
#include "TranSocket.h"
#include <WS2tcpip.h>
#include <process.h>

UCHAR TranSocket::heartData[8] = { 0x02 ,0x00 ,0x03 ,0x31 ,0x11 ,0x01 ,0x21, 0x03 };

bool TranSocket::isConnected = false;
sockaddr_in TranSocket::serAddr;
SOCKET TranSocket::sclient = NULL;
HANDLE TranSocket::g_hMutex = NULL;

UINT TranSocket::g_dwDefThreadId = 0;
TranSocket *TranSocket::m_instance = new TranSocket();
TranSocket * TranSocket::GetInstance()
{
	//thread_exit= CreateEvent(NULL, TRUE, FALSE, NULL);
	return m_instance;
}

bool TranSocket::GetIsConnected()
{
	return isConnected;
}


TranSocket::TranSocket()
{
	isConnected = false;
	//����һ��������
	g_hMutex = CreateMutex(NULL, FALSE, NULL);
	//�����߳�
// 	hThread = CreateThread(NULL,
// 		0,
// 		(LPTHREAD_START_ROUTINE)Heart_Thead,
// 		NULL,
// 		0,
// 		NULL);
	hThread= (HANDLE)_beginthreadex(NULL, 0, &Heart_Thead, NULL, 0, &g_dwDefThreadId);

}

TranSocket::~TranSocket()
{
	WSACleanup();//�ͷ�Winsock��
	CloseHandle(hThread);
	delete m_instance;
}
int TranSocket::Connet()
{

	WORD sockVersion = MAKEWORD(2, 2);//����Winsock��
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)////��ʼ��socket��Դ  
	{
		WSACleanup();
		isConnected = false;
		return -1;
	}
	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		//printf("invalid socket!");
		isConnected = false;
		return -1;
	}

	wchar_t ip[] = L"127.0.0.1";
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(9988);//�˿ں�
	InetPton(AF_INET, ip, &serAddr.sin_addr.S_un.S_addr);//Ip��ַ
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)//��������
	{  //����ʧ��   
		//printf("connect error !");
		closesocket(sclient);
		isConnected = false;
		return -1;
	}
	isConnected = true;
	return 0;
}

unsigned __stdcall TranSocket::Heart_Thead(LPVOID lpParameter)
{

	while (true)
	{
		int iRet = -1;
		if (!isConnected)
		{
			WaitForSingleObject(g_hMutex, INFINITE);
			iRet = Connet();//���Ӻ�̨����
			ReleaseMutex(g_hMutex);
			if (iRet == 0)
			{
				isConnected = true;
			}
		}
		iRet = _write((const char *)heartData, 8);//����������
		if (iRet != 0)
		{
			isConnected = false;
			continue;
		}
		Sleep(1000);
		unsigned char  rebuff[7] = { 0 };
		int relen = 7;
		iRet = _read((char *)rebuff, &relen, 2000);//���շ���������
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			isConnected = false;
			continue;
		}
		Sleep(6000);
	}
}

int TranSocket::_write(const char * buffer, int length)
{
	if (sclient == NULL)
	{
		return -1;
	}
	int	nNetTimeout = 1000;
	//����ʱ��
	setsockopt(sclient, SOL_SOCKET, SO_SNDTIMEO, (char *)& nNetTimeout, sizeof(int));
	send(sclient, buffer, length, 0);
	return 0;
}

int TranSocket::_read(char * refbuffer, int * length, int timeout)
{
	setsockopt(sclient, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));
	*length = recv(sclient, refbuffer, *length, 0);
	return *length;
}

int TranSocket::WriteData(unsigned char * buffer, int length)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	int iRet = _write((const char *)buffer, length);
	ReleaseMutex(g_hMutex);
	return iRet;
}

int TranSocket::ReadData(unsigned char * refbuffer, int * reflegth, unsigned long timeout)
{
	if (sclient == NULL)
	{
		return -1;
	}
	//����ʱ��
	WaitForSingleObject(g_hMutex, INFINITE);
	_read((char *)refbuffer, reflegth, timeout);
	ReleaseMutex(g_hMutex);
	return 0;
}
