#include "stdafx.h"
#include "TranSocket.h"
#include <WS2tcpip.h>
#include <process.h>
#include "../LibLog/Log.h"

//CSocketDelegete *TranSocket::socketDeleget = NULL;
UCHAR TranSocket::heartData[8] = { 0x02 ,0x00 ,0x03 ,0x31 ,0x11 ,0x01 ,0x21, 0x03 };

bool TranSocket::isConnected = false;
sockaddr_in TranSocket::serAddr;
SOCKET TranSocket::sclient = NULL;
HANDLE TranSocket::g_hMutex = NULL;
UINT TranSocket::g_dwDefThreadId = 0;
UINT TranSocket::g_ReadThreadId = 0;
HANDLE TranSocket::hReadThread = NULL;
HANDLE TranSocket::hThread = NULL;
TranSocket *TranSocket::m_instance = new TranSocket();
TranSocket * TranSocket::GetInstance()
{
	//socketDeleget = socketEvent;
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
	Log::i("TranSocket", "�����߳�", NULL);
	hThread = (HANDLE)_beginthreadex(NULL, 0, &Heart_Thead, NULL, 0, &g_dwDefThreadId);

}

int TranSocket::ReadData(CSocketDelegete * socketDelegete, int timeout)
{
	setsockopt(sclient, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));
	hReadThread = (HANDLE)_beginthreadex(NULL, 0, &Read_Thead, (LPVOID)socketDelegete, 0, &g_ReadThreadId);//�������߳�
	return 0;
}

TranSocket::~TranSocket()
{

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
		Log::i("TranSocket", "invalid socket!", NULL);
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
		Log::i("TranSocket", "connect error !", NULL);
		closesocket(sclient);
		isConnected = false;
		return -1;
	}
	Log::i("TranSocket", "connect ok !", NULL);
	isConnected = true;
	return 0;
}

int TranSocket::unConnet()
{
	Log::i("TranSocket", "~TranSocket", NULL);
	closesocket(sclient);
	WSACleanup();//�ͷ�Winsock��
	CloseHandle(hThread);
	return 0;
}

unsigned __stdcall TranSocket::Heart_Thead(LPVOID lpParameter)
{

	while (true)
	{
		int iRet = -1;
		Log::i("TranSocket.Heart_Thead", "isConnected=%d", isConnected);
		if (!isConnected)
		{
			WaitForSingleObject(g_hMutex, INFINITE);
			iRet = Connet();//���Ӻ�̨����
			ReleaseMutex(g_hMutex);
			if (iRet == 0)
			{
				isConnected = true;
				if (hReadThread != NULL)
				{
					CloseHandle(hReadThread);
					hReadThread = NULL;
				}

			}
		}
		WaitForSingleObject(g_hMutex, INFINITE);
		iRet = _write((const char *)heartData, 8);//����������
		Log::i("TranSocket.Heart_Thead", "_write=%d", iRet);
		if (iRet <= 0)
		{
			unConnet();
			isConnected = false;
			continue;
		}
		Sleep(1000);
		unsigned char  rebuff[7] = { 0 };
		int relen = 7;

		iRet = _read((char *)rebuff, &relen, 500);//���շ���������
		Log::i("TranSocket.Heart_Thead", "_read=%d", iRet);
		ReleaseMutex(g_hMutex);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			unConnet();
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
	int iret = send(sclient, buffer, length, 0);
	return iret;
}

int TranSocket::_read(char * refbuffer, int * length, int timeout)
{
	setsockopt(sclient, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));
	*length = recv(sclient, refbuffer, *length, 0);
	return *length;
}

unsigned _stdcall TranSocket::Read_Thead(LPVOID lpParameter)
{
	CSocketDelegete *socketDeleget = (CSocketDelegete *)lpParameter;
	UCHAR * refbuffer = new UCHAR[4096];
	int length = 4096;
	int iRet = 0;
	while (true)
	{
		if (!isConnected)
		{
			break;
		}
		memset(refbuffer, 0, length);
		WaitForSingleObject(g_hMutex, INFINITE);
		iRet = recv(sclient, (char *)refbuffer, length, 0);
		ReleaseMutex(g_hMutex);
		if (socketDeleget == NULL)
			if (iRet > 0)
			{
				socketDeleget->socketRevCallBack(refbuffer);//�յ�����ʱ�Ļص�
				break;
			}
			else if (iRet == 0)
			{
				socketDeleget->socketdisConnectCallBack();//����̿��ص�
				break;
			}
			else if (iRet == SOCKET_ERROR)
			{
				socketDeleget->socketErrCallBack();//���ӳ���ص�
				break;
			}
		Sleep(200);
	}
	delete[]refbuffer;
	return 0;
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

