#pragma once
#include "IPrinter.h"
#include "../libNetWorkConnet/TranSocket.h"
#include "../libNetWorkConnet/CSocketDelegete.h"

#define IPRINTER __declspec(dllexport)

class Printer :
	public IPrinter, public CSocketDelegete
{
private:
	TranSocket * transoket;
public:


	Printer();
	~Printer();
	void socketRevCallBack(unsigned char *buffer);
	void socketSendCallBack(unsigned char *buffer);
	void socketdidConnectCallBack();
	void socketdisConnectCallBack();
	void socketErrCallBack();
	void setDeviceEventHandler(void* pHandler);


	void initialize(XmlParser* pConfig);
	const char* getDeviceId();
	bool isBusy();
	DeviceStatus getDeviceStatus();
	void cancel(int nReqID);
	int transaction(const char* tranID, const void* parameter, int* pReqID);


	//
	// ������ timeout �Ժ���Ϊ��λ���������Ϊ0�����ʾ���޵ȴ�
	//
	int readCard(int timeout, int* reqID);

	// 
	// ������Ƭ���˶�������Ԥ��ʱ��������δ�ɹ����򷵻ز�����ʱ
	// 
	int ejectCard(int* reqID);

	//
	// �̿�
	//
	int retainCard(int* reqID);

	//
	// �����̿�����
	//
	int resetRetainCount(int* reqID);

	// ��ȡ�̿�����
	int getRetainCount();

	//
	// ��ȡ����
	// ��������

#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
	bool getData(const std::string& key, std::string& value);
	bool setData(const std::string& key, const std::string& value);
#else
	const int getData(const char* key, char* value, unsigned int len);
	bool setData(const char* key, const char* value);
#endif

	// ��ȡ���һ���������
	int getLastError();

	// ��ȡ���һ�����������
	// ��ֹ����
	const char* getLastErrorDescription();

	// #if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
	// 	  bool getVendorInfo(std::string& key, std::string& val);
	// #else
	// 	  const int getVendorInfo(const char* key, char* val, unsigned int len);
	// #endif


	int printForm(const char* formName, const char* content, int* pReqID = NULL);


	int printData(const void* data, int nSize, int* pReqID = NULL);

	int read(const int timeout, int* pReqID = 0);

	PaperStatus getPaperStatus(const char* pos = NULL);
};

extern "C" IPRINTER IPrinter* _stdcall createDevice();