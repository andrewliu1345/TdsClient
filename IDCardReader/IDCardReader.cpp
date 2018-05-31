// IDCardReader.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ICardReader.h"
#include "IDCardReader.h"


IDCardRead::IDCardRead()
{
	IDCardRead::transoket = TranSocket::GetInstance();
}

void IDCardRead::setDeviceEventHandler(void * pHandler)
{
	m_pEventHandler = static_cast<ICardReaderEventHandler*>(pHandler);
}

void IDCardRead::initialize(XmlParser * pConfig)
{
	if (transoket->GetIsConnected())
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_SUCCESS);
	}
	else
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_INTERNAL_ERROR);
	}
}

const char * IDCardRead::getDeviceId()
{
	return "IDCard";
}

bool IDCardRead::isBusy()
{
	return false;
}

DeviceStatus IDCardRead::getDeviceStatus()
{
	return DeviceStatus();
}

void IDCardRead::cancel(int nReqID)
{
}

int IDCardRead::transaction(const char * tranID, const void * parameter, int * pReqID)
{
	return 0;
}

int IDCardRead::readCard(int timeout, int * reqID)
{
	return 0;
}

int IDCardRead::ejectCard(int * reqID)
{
	return 0;
}

int IDCardRead::retainCard(int * reqID)
{
	return 0;
}

int IDCardRead::resetRetainCount(int * reqID)
{
	return 0;
}

int IDCardRead::getRetainCount()
{
	return 0;
}
ICardReader::CardReaderMediaStatus IDCardRead::getMediaStatus()
{
	return CardReaderMediaStatus::ENTERING;
}

ICardReader::CardReader_CardType IDCardRead::getCardType()
{
	return CardReader_CardType::CRCT_Unknown;
}

#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
bool IDCardRead::getData(const std::string & key, std::string & value)
{
	return false;
}

bool IDCardRead::setData(const std::string & key, const std::string & value)
{
	return false;
}
#else
const int IDCardRead::getData(const char * key, char * value, unsigned int len)
{
	return 0;
}

bool IDCardRead::setData(const char * key, const char * value)
{
	return false;
}
int IDCardRead::getLastError()
{
	return 0;
}
const char * IDCardRead::getLastErrorDescription()
{
	return nullptr;
}
#endif

ICardReader * APIENTRY createDevice()
{
	ICardReader *tmp = new IDCardRead();
	return tmp;
}