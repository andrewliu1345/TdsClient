// IDCardReader.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ICardReader.h"
#include "IDCardReader.h"
#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"
const UCHAR idcard_CMD[] = { 0xd0,0x07 };
const UCHAR *classname = (UCHAR *)"IDCardReader";
void IDCardRead::socketRevCallBack(unsigned char * buffer)
{
	UCHAR cmd[2] = { 0 };
	memcpy(cmd, &buffer[3], 2);
	if (cmd[0] != idcard_CMD[0] || cmd[1] != idcard_CMD[1])
	{
		return;
	}
	UCHAR tag = buffer[5];
	switch (tag)
	{
	case 1://读卡
	{
		RevReadCard(buffer);
		break;
	}
	case 2://读信息
	{
		break;
	}
	default:
		break;
	}
}

void IDCardRead::socketSendCallBack(unsigned char * buffer)
{

}

void IDCardRead::socketdidConnectCallBack()
{

}

void IDCardRead::socketdisConnectCallBack()
{

}

void IDCardRead::socketErrCallBack()
{
}

IDCardRead::IDCardRead()
{
	Log::i((char *)CLASSNAME, (char *)"IDCardRead\n");
	IDCardRead::transoket = TranSocket::GetInstance();
}

IDCardRead::~IDCardRead()
{
	//transoket->GetIsConnected();
}

void IDCardRead::setDeviceEventHandler(void * pHandler)
{
	Log::i((char *)CLASSNAME, (char *)"setDeviceEventHandler\n");
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
	//UCHAR sendbuffer[] = { 0x02,0x00, 0x0C,0xD0,0x07,0x01,0x00,0x01,0x0F,0xD9,0x03 };
	Log::i((const char *)classname, "超时时间：%d", timeout);
	sParam p1;
	p1.ParamData = new unsigned char[1]{ (UCHAR)timeout };
	p1.ParamLen = 1;
	UCHAR* sendbuffer = Utility::toPackData((UCHAR *)idcard_CMD, 0x01, 1, p1);
	int iRet = transoket->WriteData(sendbuffer, sizeof(sendbuffer));
	if (iRet > 0)
	{
		iRet = transoket->ReadData(this, timeout * 1000);//开启读取异步线程
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		return DEVICE_ERROR_HARDWARE_ERROR;
	}
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
	Log::i((const char *)classname, "getDatakeylen=%s,len=%d\n", key, len);
	sParam p1;
	p1.ParamData = (UCHAR *)key;
	p1.ParamLen = strlen(key);
	UCHAR tmp[2048] = { 0 };
	int tmplen = 0;
	UCHAR* sendbuffer = Utility::toPackData((UCHAR *)idcard_CMD, 0x01, 1, p1);
	int iRet = transoket->WriteData(sendbuffer, sizeof(sendbuffer));
	iRet = transoket->ReadData(tmp, &tmplen, 1000);
	if (iRet == 0)
	{
		PARAMLIST params;
		PARAMLIST::iterator i;
		Utility::unPackData(tmp, 1, &params);
		i = params.begin();
		sParam revdata = (sParam)*i;
		memcpy(value, revdata.ParamData, revdata.ParamLen);
		delete[]revdata.ParamData;
		params.clear();
	}
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
void IDCardRead::RevReadCard(UCHAR * buffer)
{
	UCHAR tag = buffer[7];
	switch (tag)
	{
	case 0:
	{
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_SUCCESS, tag);
		break;
	}
	case 1:
	{
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_HARDWARE_ERROR, tag);
		break;
	}
	case 2://超时
	{
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_TIMEOUT, tag);
		break;
	}
	default:
		break;
	}
}
#endif

ICardReader * APIENTRY createDevice()
{
	ICardReader *tmp = new IDCardRead();
	return tmp;
}
