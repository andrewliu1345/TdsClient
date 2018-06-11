#ifndef TDS_DEVICE_IPRINTER_H
#define TDS_DEVICE_IPRINTER_H

#include "../IDevice.h"
#include <string>

NS_TDS_DEVICE_BEGIN

class IPrinterEventHandler;

class IPrinter : public IDevice
{
public:
	enum PaperStatus
	{
		PAPER_EMPTY = 0, //��ֽ������
		PAPER_FULL = 1, //��ֽ��ֽ��
		PAPER_LOW = 2, //��ֽ��ֽ����
		PAPER_PRESENT = 3,	//	�����ڴ�ӡ����
		PAPER_JAMMED = 4,	//	����������ӡ����
		PAPER_NOTSUPP = 5,	//	��ӡ�����ܼ�����״̬
		PAPER_UNKNOWN = 6,	//	����״̬����
		PAPER_ENTERING = 7,	//	��������ڴ�
		PAPER_RETRACTED = 8	//	���ʱ�����

	};

	// ��IDevice�̳ж����ķ���
public:
	// ���������ͱ�Ϊ "CardReaderService"
	virtual const char* getDeviceClass()
	{
		return "PrinterService";
	}

	virtual void initialize(XmlParser* pConfig) = 0;

	virtual void setDeviceEventHandler(void* pHandler)
	{
		m_pEventHandler = static_cast<IPrinterEventHandler*>(pHandler);
	}

	virtual const char* getDeviceId() = 0;
	virtual bool isBusy() = 0;
	virtual DeviceStatus getDeviceStatus() = 0;
	virtual void cancel(int nReqID) = 0;
	virtual int transaction(const char* tranID, const void* parameter, int* pReqID = NULL) = 0;

	// ��ӡ���ӿ�
public:
	//
	// ����Ԥ�ȶ����Form��ʽ����ӡ���ݡ�
	// Form��������ũ�кͳ��̹�ͨ�̶���
	// ���з��治��Ҫ���Form�ı�д��ʽ����Ϣ��ֻ��Ҫ
	// �˽�Form�������Լ���ӡ����Ч�����ɡ�
	// Form�ĸ�����֮���� # �ָ���## ���ʾ#��
	// ����:  Bank=��������#Name=�ͻ�
	// ��ӡ��ɺ��豸Ӧ��������ֽ
	//
	// how to print bitmap from js
	virtual int printForm(const char* formName, const char* content, int* pReqID = NULL) = 0;

	//
	// ��ӡ�����ݣ������ݵĸ�ʽ���ݲ�ͬ���豸�ͱ�������˵��
	// 
	virtual int printData(const void* data, int nSize, int* pReqID = NULL) = 0;

	//
	// ��ȡָ�������ֽ״̬
	// �������ӡ����pos����NULL�����������ӡ����pos�������⺬��
	//
	virtual PaperStatus getPaperStatus(const char* pos = NULL) = 0;

	//
	// ���ڴ��۴�ӡ���������ڻ�ȡ�����ϵ�����
	//
#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
	virtual bool getData(const std::string& key, std::string& val) = 0;
#else
        virtual const int getData(const char* key, char* value, unsigned int len) = 0;
#endif
	//
	// �����ۣ� timeoutΪ��ʱʱ��
	//
	virtual int read(const int timeout, int* pReqID = 0) = 0;


protected:
	IPrinterEventHandler* m_pEventHandler;
};

NS_TDS_DEVICE_END

#endif
