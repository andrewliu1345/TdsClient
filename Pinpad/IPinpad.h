#ifndef TDS_DEVICE_IPINPAD_H
#define TDS_DEVICE_IPINPAD_H

#include "../IDevice.h"
#include "IPinpadEventHandler.h"

#include <string>
#define IPINPAD __declspec(dllexport)


NS_TDS_DEVICE_BEGIN

class IPinpad : public IDevice
{
	// ��IDevice�̳ж����ķ���
public:
	virtual const char* getDeviceClass()
	{
		return "PinpadService";
	}


	virtual void initialize(XmlParser* pConfig) = 0;

	virtual void setDeviceEventHandler(void* pHandler) = 0;


	virtual const char* getDeviceId() = 0;
	virtual bool isBusy()=0;
	virtual DeviceStatus getDeviceStatus() = 0;
	virtual void cancel(int nReqID) = 0;

	virtual int transaction(const char* tranID, const void* parameter, int* pReqID = NULL) = 0;

	// ������̲���
public:
	//
	// ������Կ
	// keyname : Ҫ�������Կ����
	// key : Ҫ�������Կ���ݣ�
	// keyuse: ��Կ��;
	// encKey ������֮ǰ�ú���Կ����
	//
	virtual int importKey(const char* keyname, const char* data, const char* keyuse, const char* encKeyName = NULL, int* pReqID = NULL) = 0;

	// 
	// ���������Կ���˲���Ϊͬ������
	//
	virtual int clearKey() = 0;

	// ����
	virtual int encrypt(const char* keyname, const char* algorithm, const char* dataIn, int* pReqID = NULL) = 0;

	// ����
	virtual int decrypt(const char* keyname, const char* algorithm, const char* dataIn, int* pReqID = NULL) = 0;

	// ������ͨ����ģʽ��ֻ��cancel��������ֹ��״̬
	virtual int getData(int* pReqID = NULL) = 0;

	//
	// ������������ģʽ
	// min �� ��С����
	// max �� �������
	// ���û�����Enter���������û�������cancel�󣬴�״̬����
	//
	virtual int getPin(const int min = 3, const int max = 6, int* pReqID = NULL) = 0;

	// �������getPin��ʹ����DEVICE_SUCCESS����ʽ����
	virtual void endGetPin() = 0;

	//
	// ��������
	//
	virtual int getPinblock(const char* keyname, const char* cryptData = NULL, int* pReqID = NULL) = 0;

	// �������¼�
private:


protected:
	IPinpadEventHandler * m_pEventHandler;
	//
};

NS_TDS_DEVICE_END




#endif