#include "stdafx.h"
#include "TranSocket.h"
#include <WS2tcpip.h>
#include <process.h>
#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"

//CSocketDelegete *TranSocket::socketDeleget = NULL;
UCHAR TranSocket::heartData[8] = { 0x02 ,0x00 ,0x03 ,0x31 ,0x11 ,0x01 ,0x21, 0x03 };

bool TranSocket::isConnected = false;
sockaddr_in TranSocket::serAddr;

sockaddr_in TranSocket::binAddr;

int TranSocket::bind_port = 8090;

SOCKET TranSocket::sclient = NULL;
//����һ��������
HANDLE TranSocket::g_hMutex = CreateMutex(NULL, FALSE, NULL);
UINT TranSocket::g_dwDefThreadId = 0;
UINT TranSocket::g_ReadThreadId = 0;
UINT TranSocket::g_FlushThreadId = 0;

HANDLE TranSocket::hReadThread = NULL;

HANDLE TranSocket::hFlushThread = NULL;

HANDLE TranSocket::hThread = NULL;
TranSocket *TranSocket::m_instance = new TranSocket();
TranSocket * TranSocket::GetInstance()
{
	//socketDeleget = socketEvent;
	//thread_exit= CreateEvent(NULL, TRUE, FALSE, NULL);
	return m_instance;
}

TranSocket* TranSocket::GetInstance(int _binport)
{
	bind_port = _binport;
	return m_instance;
}

bool TranSocket::GetIsConnected()
{
	return isConnected;
}


TranSocket::TranSocket()
{
	WORD sockVersion = MAKEWORD(2, 2);//����Winsock��
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)////��ʼ��socket��Դ  
	{
		WSACleanup();
		isConnected = false;

	}
	Log::i("TranSocket", "TranSocket() ����", NULL);


	//�����߳�
	// 	hThread = CreateThread(NULL,
	// 		0,
	// 		(LPTHREAD_START_ROUTINE)Heart_Thead,
	// 		NULL,
	// 		0,
	// 		NULL);

	hThread = (HANDLE)_beginthreadex(NULL, 0, &Heart_Thead, NULL, 0, &g_dwDefThreadId);
	//Sleep(5000);//���߳�����

}

int TranSocket::ReadData(CSocketDelegete * socketDelegete, int timeout)
{
	setsockopt(sclient, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));
	hReadThread = (HANDLE)_beginthreadex(NULL, 0, &Read_Thead, (LPVOID)socketDelegete, 0, &g_ReadThreadId);//�������߳�
	return 0;
}



TranSocket::~TranSocket()
{
	CloseHandle(hThread);
	delete m_instance;
}

int TranSocket::Connet()
{


	if (sclient > 0 && isConnected == false)
	{
		CloseSocket(&sclient);
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


														 //���ջ�����
	int nRecvBuf = 4 * 1024;//����Ϊ4K
	setsockopt(sclient, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
	//���ͻ�����
	int nSendBuf = 4 * 1024;//����Ϊ4K
	setsockopt(sclient, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));

	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)//��������
	{  //����ʧ��   
	   //printf("connect error !");
		DWORD k = GetLastError();
		Log::i("TranSocket", "connect error ! Err=%u", k);
		CloseSocket(&sclient);
		isConnected = false;
		return -1;
	}
	Log::i("TranSocket", "connect ok !", NULL);
	isConnected = true;
	return 0;
}

int TranSocket::unConnet()
{
	Log::i("TranSocket", "unConnet()", NULL);
	CloseSocket(&sclient);
	sclient = INVALID_SOCKET;
	WSACleanup();//�ͷ�Winsock��

	return 0;
}

int TranSocket::CloseSocket(__in SOCKET *s)
{
	closesocket(*s);
	*s = INVALID_SOCKET;
	return 0;
}

unsigned __stdcall TranSocket::Heart_Thead(LPVOID lpParameter)
{
	Log::i("TranSocket", "�����߳�", NULL);
	while (true)
	{
		int iRet = -1;
		Log::i("TranSocket.Heart_Thead", "isConnected=%d", isConnected);
		if (!isConnected)
		{
			Log::i("TranSocket.Heart_Thead", "isConnected=false");
			WaitForSingleObject(g_hMutex, INFINITE);
			Log::i("TranSocket.Heart_Thead", "WaitForSingleObject(g_hMutex, INFINITE)");
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
		if (iRet <= 0)
		{
			Log::i("TranSocket.Heart_Thead", "_write err=%d", iRet);
			CloseSocket(&sclient);
			//unConnet();
			isConnected = false;
			ReleaseMutex(g_hMutex);//����ʧ���ͷ��ź�
			Sleep(3000);
			continue;
		}

		unsigned char  rebuff[7] = { 0 };
		int relen = 7;

		iRet = _read((char *)rebuff, &relen, 6000);//���շ���������
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			Log::i("TranSocket.Heart_Thead", "_read err=%d", iRet);
			CloseSocket(&sclient);
			//unConnet();
			isConnected = false;
			ReleaseMutex(g_hMutex);//����ʧ���ͷ��ź�
			Sleep(3000);
			continue;
		}
		ReleaseMutex(g_hMutex);
		Sleep(60000);
	}
}

int TranSocket::_write(const char * buffer, int length)
{
	if (sclient == NULL || !isConnected)
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
	if (sclient == NULL || !isConnected)
	{
		return -1;
	}
	setsockopt(sclient, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));
	*length = recv(sclient, refbuffer, *length, 0);
	return *length;
}

unsigned _stdcall TranSocket::Read_Thead(LPVOID lpParameter)
{
	Log::i("TranSocket.Read_Thead", "Read_Thead  �첽��ȡ�߳̿���");
	CSocketDelegete *socketDeleget = (CSocketDelegete *)lpParameter;
	UCHAR  refbuffer[4096] = { 0 };
	int length = 4096;
	int iRet = 0;

	//Sleep(50);
	while (true)
	{
		if (socketDeleget != NULL) {
			if (!isConnected)
			{
				socketDeleget->socketErrCallBack();//���ӳ���ص�
			}
			memset(refbuffer, 0, length);
			WaitForSingleObject(g_hMutex, INFINITE);
			iRet = recv(sclient, (char *)refbuffer, length, 0);
			ReleaseMutex(g_hMutex);
			string str = Utility::bytesToHexstring(refbuffer, iRet);
			Log::i("TranSocket.Read_Thead", "recv iRet=%d refbuffer=%s ", iRet, str.c_str());


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
			Sleep(10);
		}
	}

	//delete[]refbuffer;
	return 0;
}



unsigned __stdcall TranSocket::Flush_Thead(LPVOID lpParameter)
{
	unsigned char refbuffer[4 * 1024] = { 0 };
	int length = 4 * 1024;
	WaitForSingleObject(g_hMutex, INFINITE);
	int iRet = _read((char *)refbuffer, &length, 1);
	ReleaseMutex(g_hMutex);
	return 0;
}

void TranSocket::Flush()
{
	//hFlushThread = (HANDLE)_beginthreadex(NULL, 0, &Flush_Thead, NULL, 0, &g_FlushThreadId);//�������߳�
	unsigned char refbuffer[4 * 1024] = { 0 };
	int length = 4 * 1024;
	//WaitForSingleObject(g_hMutex, INFINITE);
	int iRet = _read((char *)refbuffer, &length, 1);
	//ReleaseMutex(g_hMutex);
}

int TranSocket::WriteData(unsigned char * buffer, int length)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	Flush();//��ս��ջ���
	string sHexData = Utility::bytesToHexstring(buffer, length);
	Log::i("TranSocket.WriteData", "writedata=%s", sHexData.c_str());
	
	int iRet = _write((const char *)buffer, length);
	Log::i("TranSocket.WriteData", "iRet=%i", iRet);
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

