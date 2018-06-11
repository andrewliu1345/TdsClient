#pragma once
#include "ICardReader.h"
#include "ICardReaderEventHandler.h"
#include "../libNetWorkConnet/TranSocket.h"
#include "../libNetWorkConnet/CSocketDelegete.h"
#define CLASSNAME "MagneticCardReader"
struct MagCard {
	unsigned char track1_len; //1�ŵ����ݳ���
	unsigned char track2_len; //2�ŵ����ݳ���
	unsigned char track3_len; //3�ŵ����ݳ���
	unsigned char track1_data[80]; //1�ŵ�����
	unsigned char track2_data[41]; //2�ŵ�����
	unsigned char track3_data[170]; //3�ŵ�����
};

class MagneticCardReader :
	public ICardReader, public CSocketDelegete
{
private:
	long iReqid = 1;
	TranSocket * transoket;
	MagCard _MANAGED;
	void RevReadCard(UCHAR* buffer);
public:
	MagneticCardReader();
	~MagneticCardReader();

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
};
__declspec(dllimport)  ICardReader*  __stdcall createDevice();
