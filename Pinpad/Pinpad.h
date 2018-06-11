#pragma once
#include "IPinpad.h"
#include "../libNetWorkConnet/TranSocket.h"
#include "../libNetWorkConnet/CSocketDelegete.h"
#define CLASSNAME "Pinpad"

class Pinpad :
	public IPinpad,public CSocketDelegete
{
public:
	Pinpad();
	~Pinpad();

	void socketRevCallBack(unsigned char *buffer);

	void socketSendCallBack(unsigned char *buffer);

	void socketdidConnectCallBack();

	void socketdisConnectCallBack();

	void socketErrCallBack();


	 const char* getDeviceClass();


	 void initialize(XmlParser* pConfig);

	 void setDeviceEventHandler(void* pHandler);


	 const char* getDeviceId();
	 bool isBusy();
	 DeviceStatus getDeviceStatus();
	 void cancel(int nReqID);

	 int transaction(const char* tranID, const void* parameter, int* pReqID = NULL);

	 // ��ȡ���һ���������
	 int getLastError();

	 // ��ȡ���һ�����������
	 // ��ֹ����
	 const char* getLastErrorDescription();

	// ������̲���

	//
	// ������Կ
	// keyname : Ҫ�������Կ����
	// key : Ҫ�������Կ���ݣ�
	// keyuse: ��Կ��;
	// encKey ������֮ǰ�ú���Կ����
	//
	 int importKey(const char* keyname, const char* data, const char* keyuse, const char* encKeyName = NULL, int* pReqID = NULL);

	// 
	// ���������Կ���˲���Ϊͬ������
	//
	 int clearKey();

	// ����
	 int encrypt(const char* keyname, const char* algorithm, const char* dataIn, int* pReqID = NULL);

	// ����
	 int decrypt(const char* keyname, const char* algorithm, const char* dataIn, int* pReqID = NULL);

	// ������ͨ����ģʽ��ֻ��cancel��������ֹ��״̬
	 int getData(int* pReqID = NULL);

	//
	// ������������ģʽ
	// min �� ��С����
	// max �� �������
	// ���û�����Enter���������û�������cancel�󣬴�״̬����
	//
	 int getPin(const int min = 3, const int max = 6, int* pReqID = NULL);

	// ��������getPin��ʹ����DEVICE_SUCCESS����ʽ����
	 void endGetPin();

	//
	// ��������
	//
	 int getPinblock(const char* keyname, const char* cryptData = NULL, int* pReqID = NULL);
private:
	long iReqid = 1;
	TranSocket * transoket;
	void RevReadCard(UCHAR* buffer);
};

extern "C" IPINPAD IPinpad* _stdcall createDevice();