#include "stdafx.h"
#include "Pinpad.h"
#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"

void Pinpad::socketRevCallBack(unsigned char * buffer)
{
}

void Pinpad::socketSendCallBack(unsigned char * buffer)
{
}

void Pinpad::socketdidConnectCallBack()
{
}

void Pinpad::socketdisConnectCallBack()
{
}

void Pinpad::socketErrCallBack()
{
}

Pinpad::Pinpad()
{
	Log::i(CLASSNAME, (char *)"Pinpad ����\n");
	transoket = TranSocket::GetInstance();
}


Pinpad::~Pinpad()
{
}

void Pinpad::initialize(XmlParser * pConfig)

{
	Sleep(100);//�������߳̿�������
	Log::i((char *)CLASSNAME, "Pinpad::initialize");
	if (transoket->GetIsConnected())
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_SUCCESS);
	}
	else
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_INTERNAL_ERROR);
	}
}
void Pinpad::setDeviceEventHandler(void * pHandler)
{
	Log::i((char *)CLASSNAME, (char *)"setDeviceEventHandler");
	m_pEventHandler = static_cast<IPinpadEventHandler*>(pHandler);
}
const char * Pinpad::getDeviceId()
{
	return "Pinpad";
}
bool Pinpad::isBusy()
{
	return false;
}
DeviceStatus Pinpad::getDeviceStatus()
{
	if (transoket->GetIsConnected())
	{
		return DEVICE_STATUS_ONLINE;
	}
	else
	{
		return DEVICE_STATUS_HWERROR;
	}
}
void Pinpad::cancel(int nReqID)
{
}
int Pinpad::transaction(const char * tranID, const void * parameter, int * pReqID)
{
	return 0;
}
int Pinpad::getLastError()
{
	return 0;
}
const char * Pinpad::getLastErrorDescription()
{
	return nullptr;
}
int Pinpad::importKey(const char * keyname, const char * data, const char * keyuse, const char * encKeyName, int * pReqID)
{
	
	string skeyname(keyname);
	

	return 0;
}
int Pinpad::clearKey()
{
	return 0;
}
int Pinpad::encrypt(const char * keyname, const char * algorithm, const char * dataIn, int * pReqID)
{
	return 0;
}
int Pinpad::decrypt(const char * keyname, const char * algorithm, const char * dataIn, int * pReqID)
{
	return 0;
}
int Pinpad::getData(int * pReqID)
{
	return 0;
}
int Pinpad::getPin(const int min, const int max, int * pReqID)
{
	return 0;
}
void Pinpad::endGetPin()
{
}
int Pinpad::getPinblock(const char * keyname, const char * cryptData, int * pReqID)
{
	return 0;
}
void Pinpad::RevReadCard(UCHAR * buffer)
{
}
IPinpad  * APIENTRY createDevice()
{
	IPinpad *tmp = new Pinpad();
	return tmp;
}