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
		PRESENT = 0,   // ���Ѿ��������������
		LATCHED = 1,   // IC���Ѿ��ϵ�
		ENTERING = 2,  // �����뿨��
		NOTEXIST = 3,   //  �����ڶ�������
		NOTSUPPORTED = 4 // ���ͱ��������֧�ֽ���״̬
	};

	enum CardReader_CardType
	{
		CRCT_Unknown = 0, 
		CRCT_MegCard = 1,
		CRCT_ICCard = 2
	};

	// ��IDevice�̳ж����ķ���
public:
	// ���������ͱ�Ϊ "CardReaderService"
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

	// ����������
public:
	//
	// ������ timeout �Ժ���Ϊ��λ���������Ϊ0�����ʾ���޵ȴ�
	//
	virtual int readCard(int timeout = 0, int* reqID = NULL) = 0;

	// 
	// ������Ƭ���˶�������Ԥ��ʱ��������δ�ɹ����򷵻ز�����ʱ
	// 
	virtual int ejectCard(int* reqID = NULL) = 0;

	//
	// �̿�
	//
	virtual int retainCard(int* reqID = NULL) = 0;

	//
	// �����̿�����
	//
	virtual int resetRetainCount(int* reqID = NULL) = 0;

	// ��ȡ�̿�����
	virtual int getRetainCount() = 0;

	//
	// ��ȡ��״̬
	//
	virtual CardReaderMediaStatus getMediaStatus() = 0;

	// ��ȡλ�ڿ����еĿ������ͣ�����������޿����򷵻� Unknown
	virtual CardReader_CardType getCardType() = 0;

	//
	// ��ȡ����
	// ��������

#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
        virtual bool getData(const std::string& key, std::string& value) = 0;
        virtual bool setData(const std::string& key, const std::string& value) = 0;
#else
        virtual const int getData(const char* key, char* value, unsigned int len) = 0;
        virtual bool setData(const char* key, const char* value) = 0;
#endif

	// �������¼�
protected:
	ICardReaderEventHandler* m_pEventHandler;
};

NS_TDS_DEVICE_END

#endif
