#pragma once
#include "ICardReader.h"
#include "ICardReaderEventHandler.h"
#include "../libNetWorkConnet/TranSocket.h"
#include "../libNetWorkConnet/CSocketDelegete.h"
#define CLASSNAME "MagneticCardReader"
struct MagCard {
	unsigned char track1_len; //1磁道数据长度
	unsigned char track2_len; //2磁道数据长度
	unsigned char track3_len; //3磁道数据长度
	unsigned char track1_data[80]; //1磁道数据
	unsigned char track2_data[41]; //2磁道数据
	unsigned char track3_data[170]; //3磁道数据
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

	void socketRevCallBack(unsigned char *buffer, int length);

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
	// 读卡， timeout 以毫秒为单位，如果设置为0，则表示无限等待
	//
	int readCard(int timeout, int* reqID);

	// 
	// 弹出卡片，此动作尝试预定时间后，如果仍未成功，则返回操作超时
	// 
	int ejectCard(int* reqID);

	//
	// 吞卡
	//
	int retainCard(int* reqID);

	//
	// 重置吞卡张数
	//
	int resetRetainCount(int* reqID);

	// 获取吞卡张数
	int getRetainCount();

	//
	// 获取卡状态
	//
	CardReaderMediaStatus getMediaStatus();

	// 获取位于卡槽中的卡的类型，如果卡槽中无卡，则返回 Unknown
	CardReader_CardType getCardType();

	//
	// 获取数据
	// 设置数据

#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
	bool getData(const std::string& key, std::string& value);
	bool setData(const std::string& key, const std::string& value);
#else
	const int getData(const char* key, char* value, unsigned int len);
	bool setData(const char* key, const char* value);
#endif

	// 获取最后一个错误代码
	int getLastError();

	// 获取最后一个错误的描述
	// 禁止返回
	const char* getLastErrorDescription();

	// #if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
	// 	  bool getVendorInfo(std::string& key, std::string& val);
	// #else
	// 	  const int getVendorInfo(const char* key, char* val, unsigned int len);
	// #endif
};
__declspec(dllimport)  ICardReader*  __stdcall createDevice();
