#ifndef TDS_DEVICE_ICARDREADER_H
#define TDS_DEVICE_ICARDREADER_H

#include "../IDevice.h"
#include "ICardReaderEventHandler.h"

#include <string>

NS_TDS_DEVICE_BEGIN

class ICardReader : public IDevice
{
public:
	ICardReader()
	{
	}

public:
	enum CardReaderMediaStatus
	{
		PRESENT = 0,   // 卡已经被吸入读卡器中
		LATCHED = 1,   // IC卡已经上电
		ENTERING = 2,  // 卡在入卡口
		NOTEXIST = 3,   //  卡不在读卡器内
		NOTSUPPORTED = 4 // 此型别读卡器不支持介质状态
	};

	enum CardReader_CardType
	{
		CRCT_Unknown = 0,
		CRCT_MegCard = 1,
		CRCT_ICCard = 2
	};

	// 从IDevice继承而来的方法
public:
	// 读卡器的型别为 "CardReaderService"
	virtual const char* getDeviceClass()
	{
		return "CardReaderService";
	}

	virtual void initialize(XmlParser* pConfig) = 0;

	virtual void setDeviceEventHandler(void* pHandler)
	{
		m_pEventHandler = static_cast<ICardReaderEventHandler*>(pHandler);
	}

	virtual const char* getDeviceId() = 0;
	virtual bool isBusy() = 0;
	virtual DeviceStatus getDeviceStatus() = 0;
	virtual void cancel(int nReqID) = 0;
	virtual int transaction(const char* tranID, const void* parameter, int* pReqID = NULL) = 0;

	// 读卡器操作
public:
	//
	// 读卡， timeout 以毫秒为单位，如果设置为0，则表示无限等待
	//
	virtual int readCard(int timeout = 0, int* reqID = NULL) = 0;

	// 
	// 弹出卡片，此动作尝试预定时间后，如果仍未成功，则返回操作超时
	// 
	virtual int ejectCard(int* reqID = NULL) = 0;

	//
	// 吞卡
	//
	virtual int retainCard(int* reqID = NULL) = 0;

	//
	// 重置吞卡张数
	//
	virtual int resetRetainCount(int* reqID = NULL) = 0;

	// 获取吞卡张数
	virtual int getRetainCount() = 0;

	//
	// 获取卡状态
	//
	virtual CardReaderMediaStatus getMediaStatus() = 0;

	// 获取位于卡槽中的卡的类型，如果卡槽中无卡，则返回 Unknown
	virtual CardReader_CardType getCardType() = 0;

	//
	// 获取数据
	// 设置数据

#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
	virtual bool getData(const std::string& key, std::string& value) = 0;
	virtual bool setData(const std::string& key, const std::string& value) = 0;
#else
	virtual const int getData(const char* key, char* value, unsigned int len) = 0;
	virtual bool setData(const char* key, const char* value) = 0;
#endif

	// 读卡器事件
protected:
	ICardReaderEventHandler * m_pEventHandler;
};

NS_TDS_DEVICE_END

#endif
