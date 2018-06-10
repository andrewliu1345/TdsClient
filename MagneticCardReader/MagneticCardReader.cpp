#include "stdafx.h"
#include "MagneticCardReader.h"
#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"

const UCHAR mrcard_CMD[] = { 0x50,0x04 };
const UCHAR *classname = (UCHAR *)"MagneticCardReader";
MagneticCardReader::MagneticCardReader()
{
	Log::i((char *)classname, (char *)"IDCardRead 构造\n");
	transoket = TranSocket::GetInstance();

}


MagneticCardReader::~MagneticCardReader()
{
}


void MagneticCardReader::socketRevCallBack(unsigned char *buffer)
{

}

void MagneticCardReader::socketSendCallBack(unsigned char *buffer)
{

}

void MagneticCardReader::socketdidConnectCallBack()
{

}

void MagneticCardReader::socketdisConnectCallBack()
{

}

void MagneticCardReader::socketErrCallBack()
{

}

void MagneticCardReader::setDeviceEventHandler(void* pHandler)
{
	m_pEventHandler = static_cast<ICardReaderEventHandler*>(pHandler);
}

void MagneticCardReader::initialize(XmlParser* pConfig)
{
	Sleep(100);//让连接线程可以起来
	Log::i((char *)classname, "transoket->GetIsConnected()");
	if (transoket->GetIsConnected())
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_SUCCESS);
	}
	else
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_INTERNAL_ERROR);
	}

}

const char* MagneticCardReader::getDeviceId()
{
	return "SwipeCardReader";
}

bool MagneticCardReader::isBusy()
{
	return false;
}

DeviceStatus MagneticCardReader::getDeviceStatus()
{
	return DeviceStatus();
}

void MagneticCardReader::cancel(int nReqID)
{

}

int MagneticCardReader::transaction(const char* tranID, const void* parameter, int* pReqID)
{
	return 0;
}

int MagneticCardReader::readCard(int timeout, int* reqID)
{
	return 0;
}

int MagneticCardReader::ejectCard(int* reqID)
{
	return 0;
}

int MagneticCardReader::retainCard(int* reqID)
{
	return 0;
}

int MagneticCardReader::resetRetainCount(int* reqID)
{
	return 0;
}

int MagneticCardReader::getRetainCount()
{
	return 0;
}

ICardReader::CardReaderMediaStatus MagneticCardReader::getMediaStatus()
{
	return CardReaderMediaStatus::ENTERING;
}
ICardReader::CardReader_CardType MagneticCardReader::getCardType()
{
	return CardReader_CardType::CRCT_Unknown;
}

const int MagneticCardReader::getData(const char* key, char* value, unsigned int len)
{
	return 0;
}

bool MagneticCardReader::setData(const char* key, const char* value)
{
	return 0;
}

int MagneticCardReader::getLastError()
{
	return 0;
}

const char* MagneticCardReader::getLastErrorDescription()
{
	return 0;
}

ICardReader  * APIENTRY createDevice()
{
	ICardReader *tmp = new MagneticCardReader();
	return tmp;
}