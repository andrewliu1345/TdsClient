// CardReader.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ICardReader.h"
#include "CardReader.h"
#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"
const UCHAR iccard_CMD[] = { 0x50,0x04 };
const UCHAR *classname = (UCHAR *)"ICCardReader";
CardReader::CardReader()
{
	Log::i((char *)CLASSNAME, (char *)"IDCardRead 构造\n");
	CardReader::transoket = TranSocket::GetInstance();
}

CardReader::~CardReader()
{

}

void CardReader::RevReadCard(UCHAR * buffer)
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
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_NOT_EXISTED, tag);
		break;
	}
}

void CardReader::socketRevCallBack(unsigned char * buffer)
{
	UCHAR cmd[2] = { 0 };
	memcpy(cmd, &buffer[3], 2);
	if (cmd[0] != iccard_CMD[0] || cmd[1] != iccard_CMD[1])
	{
		return;
	}
	UCHAR tag = buffer[5];
	switch (tag)
	{
	case 6:
	{
		RevReadCard(buffer);
		break;
	}
	case  7:
	{
		int datalen = 0;
		PARAMLIST params;
		PARAMLIST::iterator i;
		int iRet = Utility::unPackData(buffer, 1, &params);
		if (iRet != 0)
		{
			m_pEventHandler->transactionCompleted(apdup.iReqid, apdup.szCmd, apdup.szApduData, "6D00");
			return;
		}
		i = params.begin();
		sParam revdata = (sParam)*i;
		//避免后面乱码
		char * cData = new char[revdata.ParamLen + 1];
		memset(cData, 0, revdata.ParamLen + 1);
		memcpy(cData, revdata.ParamData, revdata.ParamLen);

		string strdata(cData);
		Log::i("CardReader::socketRevCallBack", "revdata.ParamData=%s,revdata.ParamLen=%d", revdata.ParamData, revdata.ParamLen);
		string utfdata;
		unsigned char * b_utf_8;
		datalen = Utility::GB18030ToUTF8((char *)strdata.c_str(), &b_utf_8);
		Log::i("CardReader::socketRevCallBack", "卡返回：=%s", b_utf_8);
		//string utf_8_data((char *)b_utf_8);
		//memcpy(g_data, b_utf_8, datalen);
		m_pEventHandler->transactionCompleted(apdup.iReqid, apdup.szCmd, apdup.szApduData, b_utf_8);
		delete[]revdata.ParamData;
		delete[]b_utf_8;
		params.clear();


		break;
	}
	default:
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_NOT_EXISTED, tag);
		break;
	}

}

void CardReader::socketSendCallBack(unsigned char * buffer)
{

}

void CardReader::socketdidConnectCallBack()
{

}

void CardReader::socketdisConnectCallBack()
{

}

void CardReader::socketErrCallBack()
{
}

void CardReader::setDeviceEventHandler(void * pHandler)
{
	m_pEventHandler = static_cast<ICardReaderEventHandler*>(pHandler);
}

void CardReader::initialize(XmlParser * pConfig)
{
	Sleep(100);//让连接线程可以起来
	Log::i((char *)CLASSNAME, "transoket->GetIsConnected()");
	if (transoket->GetIsConnected())
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_SUCCESS);
	}
	else
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_INTERNAL_ERROR);
	}
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
	int iReqID;
	iReqID = iReqid;
	*pReqID = iReqID;
	Log::i("CardReader::transaction", "pReqID=%d", *pReqID);
	InterlockedIncrement((LPLONG)&iReqid);
	apdup.szCmd = (char *)tranID;
	apdup.iReqid = iReqID;
	apdup.szApduData = (char *)parameter;
	Log::i("CardReader.transaction", "tranID=%s parameter=%s", tranID, parameter);
	//unsigned char * b_parameter=Utility::bytesToHexstring()
	sParam p1;
	p1.ParamLen = strlen((char *)parameter);
	p1.ParamData = new unsigned char[32];
	int datalen = 0;

	memset(p1.ParamData, 0, 32);
	memcpy(p1.ParamData, parameter, p1.ParamLen);
	UCHAR tmp[256] = { 0 };
	int tmplen = 256;
	UCHAR  sendbuffer[256] = { 0 };
	int _len = 0;
	Utility::toPackData((UCHAR *)iccard_CMD, 0x07, sendbuffer, 256, &_len, 1, p1);
	int iRet = transoket->WriteData(sendbuffer, _len);

	if (iRet > 0)
	{
		iRet = transoket->ReadData(this, 6000);//开启读取异步线程
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		m_pEventHandler->transactionCompleted(iReqid, tranID, parameter, NULL);
	}
	return 0;
}


int CardReader::readCard(int timeout, int * reqID)
{
	Log::i((const char *)classname, "超时时间：%d", timeout);
	sParam p1;
	p1.ParamData = new unsigned char[1];
	p1.ParamData[0] = (UCHAR)timeout;
	p1.ParamLen = 1;
	int len = 0;
	UCHAR  sendbuffer[256] = { 0 };
	Utility::toPackData((UCHAR *)iccard_CMD, 0x06, sendbuffer, 256, &len, 1, p1);
	int iRet = transoket->WriteData(sendbuffer, len);
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

ICardReader  * APIENTRY createDevice()
{
	ICardReader *tmp = new CardReader();
	return tmp;
}