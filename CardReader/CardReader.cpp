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
	Log::i((char *)CLASSNAME, (char *)"ICCardReader 构造\n");
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
		Log::i((const char *)classname, "找到卡读卡，");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_SUCCESS, tag);
		break;
	}
	case 1:
	{
		Log::i((const char *)classname, "找卡失败，");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_HARDWARE_ERROR, tag);
		break;
	}
	case 2://超时
	{
		Log::i((const char *)classname, "找卡超时，");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_TIMEOUT, tag);
		break;
	}
	default:
		Log::i((const char *)classname, "未知错误");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_NOT_EXISTED, tag);
		break;
	}
}

void CardReader::RevApdu(UCHAR * buffer)
{
	UCHAR tag = buffer[7];
	switch (tag)
	{
	case 0:
	{
		Log::i((const char *)classname, "APDU 正常返回数据");
		int datalen = 0;
		PARAMLIST params;
		PARAMLIST::iterator i;
		int iRet = Utility::unPackData(buffer, 1, &params);
		if (iRet != 0)
		{

			Log::i("CardReader::socketRevCallBack", "卡返回 失败：iRet=%s", iRet);
			m_pEventHandler->transactionCompleted(apdup.iReqid, apdup.szCmd, apdup.szApduData, "6D00");
			params.clear();
			return;
		}
		i = params.begin();
		sParam revdata = (sParam)*i;
		//避免后面乱码
		char * cData = new char[revdata.ParamLen + 1];
		memset(cData, 0, revdata.ParamLen + 1);
		memcpy(cData, revdata.ParamData, revdata.ParamLen);

		string strdata(cData);
		//Log::i("CardReader::socketRevCallBack", "revdata.ParamData=%s,revdata.ParamLen=%d", revdata.ParamData, revdata.ParamLen);
		string utfdata;
		unsigned char * b_utf_8;
		datalen = Utility::GB18030ToUTF8((char *)strdata.c_str(), &b_utf_8);
		Log::i("CardReader::socketRevCallBack", "apdup.iReqid= %d apdup.szCmd=%s apdup.szApduData=%s APDU返回：=%s", apdup.iReqid, apdup.szCmd, apdup.szApduData, b_utf_8);
		//string utf_8_data((char *)b_utf_8);
		//memcpy(g_data, b_utf_8, datalen);
		m_pEventHandler->transactionCompleted(apdup.iReqid, apdup.szCmd, apdup.szApduData, b_utf_8);
		delete[]revdata.ParamData;
		delete[]b_utf_8;
		params.clear();
		break;
	}
	case 1:
	{
		Log::i((const char *)classname, "找卡失败，");
		m_pEventHandler->transactionCompleted(apdup.iReqid, apdup.szCmd, apdup.szApduData, "6D00");
		break;
	}
	case 2://超时
	{
		Log::i((const char *)classname, "找卡超时，");
		m_pEventHandler->transactionCompleted(apdup.iReqid, apdup.szCmd, apdup.szApduData, "6D00");
		break;
	}
	default:
		Log::i((const char *)classname, "未知错误");
		m_pEventHandler->transactionCompleted(apdup.iReqid, apdup.szCmd, apdup.szApduData, "6D00");
		break;
	}
}

void CardReader::socketRevCallBack(unsigned char * buffer, int length)
{
	Log::i((const char *)classname, "socketRevCallBack 返回数据 buffer=%s", Utility::bytesToHexstring(buffer, length).c_str());
	UCHAR cmd[2] = { 0 };
	memcpy(cmd, &buffer[3], 2);
	//apdup.iReqid = iReqid;
	if (cmd[0] != iccard_CMD[0] || cmd[1] != iccard_CMD[1])
	{
		Log::i((const char *)classname, "返回数据错误");
		return;
	}
	UCHAR tag = buffer[5];
	switch (tag)
	{
	case 6://找卡返回
	{

		RevReadCard(buffer);
		//InterlockedIncrement((LPLONG)&iReqid);
		break;
	}
	case  7://发送APDU
	{
		RevApdu(buffer);
		//InterlockedIncrement((LPLONG)&iReqid);
		break;
	}
	case 8:
	{
		RevApdu(buffer);
		break;
	}
	default:
	{
		Log::i((const char *)classname, "返回未知数据");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_NOT_EXISTED, tag);
		//InterlockedIncrement((LPLONG)&iReqid);
		break;
	}
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
	Log::i("CardReader::socketErrCallBack", "卡返回 失败：");
	m_pEventHandler->readCardCompleted(DEVICE_ERROR_TIMEOUT, iReqid);
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
	if (transoket->GetIsConnected())
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_SUCCESS);
		return	DEVICE_STATUS_ONLINE;
	}
	else
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_INTERNAL_ERROR);
		return DEVICE_STATUS_HWERROR;
	}
}
void CardReader::cancel(int nReqID)
{
	//InterlockedIncrement((LPLONG)&iReqid);

}
int CardReader::transaction(const char * tranID, const void * parameter, int * pReqID)
{
	int iReqID;
	iReqID = iReqid;
	*pReqID = iReqID;
	Log::i("CardReader::transaction", "pReqID=%d", *pReqID);
	InterlockedIncrement((LPLONG)&iReqid);
	memset(apdup.szCmd, 0, 256);
	memset(apdup.szApduData, 0, 2048);
	if (tranID)
		strcpy_s(apdup.szCmd, tranID);
	if (parameter)
		strcpy_s(apdup.szApduData, (char *)parameter);

	apdup.iReqid = iReqID;

	Log::i("CardReader.transaction", "tranID=%s parameter=%s", tranID, parameter);
	//unsigned char * b_parameter=Utility::bytesToHexstring()
	sParam p1;
	p1.ParamLen = strlen((char *)parameter);
	p1.ParamData = new unsigned char[p1.ParamLen + 1];
	int datalen = 0;

	memset(p1.ParamData, 0, p1.ParamLen + 1);
	memcpy(p1.ParamData, parameter, p1.ParamLen);
	UCHAR tmp[256] = { 0 };
	int tmplen = 256;
	UCHAR  sendbuffer[256] = { 0 };
	int _len = 0;
	Utility::toPackData((UCHAR *)iccard_CMD, 0x07, sendbuffer, 256, &_len, 1, p1);
	int iRet = transoket->SyncTranData(sendbuffer, _len, this, 10000);//以磁条为主
	if (iRet > 0)
	{
		//iRet = transoket->ReadData();//开启读取异步线程

		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		Log::i((const char *)classname, "写入数据失败");
		m_pEventHandler->transactionCompleted(iReqid, tranID, parameter, NULL);
		return DEVICE_ERROR_NOT_EXISTED;
	}

}


int CardReader::readCard(int timeout, int * reqID)
{
	iReqid = 1;
	if (reqID == NULL)
	{
		Log::i("readCard", "reqID=NULL");
	}
	else
	{
		Log::i("readCard", "reqID=%d", *reqID);
	}
	int _timeout = 3;
	if (timeout != 0)
	{
		_timeout = timeout;
	}
	//iReqid = 1;
	//*reqID = iReqid;
	Log::i((const char *)classname, "开始读卡，超时时间：%d", _timeout);
	sParam p1;
	p1.ParamData = new unsigned char[1];
	p1.ParamData[0] = (UCHAR)_timeout;
	p1.ParamLen = 1;
	int len = 0;
	UCHAR  sendbuffer[256] = { 0 };
	Utility::toPackData((UCHAR *)iccard_CMD, 0x06, sendbuffer, 256, &len, 1, p1);
	int iRet = transoket->SyncTranData(sendbuffer, len, this, _timeout * 1000);

	if (iRet > 0)
	{
		//iRet = transoket->ReadData(this, _timeout * 1000); //开启读取异步线程
		InterlockedIncrement((LPLONG)&iReqid);
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_INTERNAL_ERROR, iReqid);
		return DEVICE_ERROR_HARDWARE_ERROR;
	}

}

int CardReader::ejectCard(int * reqID)
{
	int iReqID;

	iReqID = iReqid;
	//	*reqID=iReqID;
	m_pEventHandler->ejectCardCompleted(DEVICE_ERROR_OP_NOT_SUPPORTED, iReqID);
	InterlockedIncrement((LPLONG)&iReqid);
	return DEVICE_ERROR_OP_NOT_SUPPORTED;
}

int CardReader::retainCard(int * reqID)
{
	int iReqID;
	iReqID = iReqid;
	//	*reqID=iReqID;
	InterlockedIncrement((LPLONG)&iReqid);

	return DEVICE_ERROR_OP_NOT_SUPPORTED;
}

int CardReader::resetRetainCount(int * reqID)
{
	int iReqID;
	iReqID = iReqid;
	//	*reqID=iReqID;
	InterlockedIncrement((LPLONG)&iReqid);

	return DEVICE_ERROR_OP_NOT_SUPPORTED;
}

int CardReader::getRetainCount()
{
	return DEVICE_ERROR_OP_NOT_SUPPORTED;
}

ICardReader::CardReaderMediaStatus CardReader::getMediaStatus()
{
	return NOTSUPPORTED;
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