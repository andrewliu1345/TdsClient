#pragma once
#include "ICardReader.h"
#include "ICardReaderEventHandler.h"
#include <TranSocket.h>
#include "../libNetWorkConnet/CSocketDelegete.h"
#define CLASSNAME "IDCardRead"
class IDCardRead :public ICardReader, public CSocketDelegete
{
public:

	void socketRevCallBack(unsigned char *buffer, int length);

	void socketSendCallBack(unsigned char *buffer);

	void socketdidConnectCallBack();

	void socketdisConnectCallBack();

	void socketErrCallBack();

	IDCardRead();
	~IDCardRead();

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
	// ��ȡ��״̬
	//
	CardReaderMediaStatus getMediaStatus();

	// ��ȡλ�ڿ����еĿ������ͣ�����������޿����򷵻� Unknown
	CardReader_CardType getCardType();

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
private:
	TranSocket * transoket;
	void RevReadCard(UCHAR* buffer);
	static unsigned char g_data[38863];
	long iReqid = 1;
};
__declspec(dllimport)   ICardReader * APIENTRY  createDevice();