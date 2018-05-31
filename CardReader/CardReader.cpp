// CardReader.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "CardReader.h"
#include "ICardReader.h"


// void CardReader::setDeviceEventHandler(void * pHandler)
// {
// }

void CardReader::setDeviceEventHandler(void * pHandler)
{
	m_pEventHandler = static_cast<ICardReaderEventHandler*>(pHandler);
}

void CardReader::initialize(XmlParser * pConfig)
{
	//m_pEventHandler->initializeCompleted()
}
const char * CardReader::getDeviceId()
{
	return "ICCardDevice";
}
bool CardReader::isBusy()
{
	return false;
}
DeviceStatus CardReader::getDeviceStatus()
{
	return DeviceStatus();
}
void CardReader::cancel(int nReqID)
{
}
int CardReader::transaction(const char * tranID, const void * parameter, int * pReqID)
{
	return 0;
}


int CardReader::readCard(int timeout, int * reqID)
{
	return 0;
}

int CardReader::ejectCard(int * reqID)
{
	return 0;
}

int CardReader::retainCard(int * reqID)
{
	return 0;
}

int CardReader::resetRetainCount(int * reqID)
{
	return 0;
}

int CardReader::getRetainCount()
{
	return 0;
}

ICardReader::CardReaderMediaStatus CardReader::getMediaStatus()
{
	return CardReaderMediaStatus::ENTERING;
}

ICardReader::CardReader_CardType CardReader::getCardType()
{
	return CardReader_CardType::CRCT_Unknown;
}


#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)

bool CardReader::getData(const std::string & key, std::string & value)
{
	return false;
}

bool CardReader::setData(const std::string & key, const std::string & value)
{
	return false;
}
#else
const int CardReader::getData(const char * key, char * value, unsigned int len)
{
	return 0;
}

bool CardReader::setData(const char * key, const char * value)
{
	return false;
}
int CardReader::getLastError()
{
	return 0;
}
const char * CardReader::getLastErrorDescription()
{
	return nullptr;
}

#endif

ICardReader * APIENTRY createDevice()
{
	ICardReader *tmp = new CardReader();
	return tmp;
}