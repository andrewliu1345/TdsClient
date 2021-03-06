// IDCardReader.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ICardReader.h"
#include "IDCardReader.h"
#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"
const UCHAR idcard_CMD[] = { 0xd0,0x07 };
const UCHAR *classname = (UCHAR *)"IDCardReader";

void IDCardRead::socketRevCallBack(unsigned char *buffer, int length)
{
	UCHAR cmd[2] = { 0 };
	::memcpy(cmd, &buffer[3], 2);
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
	m_pEventHandler->readCardCompleted(DEVICE_ERROR_TIMEOUT, iReqid);
}

IDCardRead::IDCardRead()
{
	Log::i((char *)CLASSNAME, (char *)"IDCardRead 构造\n");
	IDCardRead::transoket = TranSocket::GetInstance();
}

IDCardRead::~IDCardRead()
{
	//transoket->GetIsConnected();
}

void IDCardRead::setDeviceEventHandler(void * pHandler)
{
	Log::i((char *)CLASSNAME, (char *)"setDeviceEventHandler");
	m_pEventHandler = static_cast<ICardReaderEventHandler*>(pHandler);
}

void IDCardRead::initialize(XmlParser * pConfig)
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
	Log::i((char *)CLASSNAME, (char *)"getDeviceStatus");
	if (transoket->GetIsConnected())
	{
		return DEVICE_STATUS_ONLINE;
	}
	else
	{
		return DEVICE_STATUS_HWERROR;
	}

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
	//iReqid = 1;
	//*reqID = iReqid;
	memset(g_data, 0, 70);
	int _timeout = 30;
	if (timeout != 0)
	{
		_timeout = timeout;
	}

	Log::i((const char *)classname, "超时时间：%d", timeout);
	sParam p1;
	p1.ParamData = new unsigned char[1];
	p1.ParamData[0] = (UCHAR)_timeout;
	p1.ParamLen = 1;
	int len = 0;
	UCHAR  sendbuffer[256] = { 0 };
	Utility::toPackData((UCHAR *)idcard_CMD, 0x01, sendbuffer, 256, &len, 1, p1);
	delete[] p1.ParamData;
	int iRet = transoket->SyncTranData(sendbuffer, len, this, _timeout * 1000);

	if (iRet > 0)
	{
		//iRet = transoket->ReadData(this, _timeout * 1000);//开启读取异步线程
		InterlockedIncrement((LPLONG)&iReqid);
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_INTERNAL_ERROR, iReqid);
		return DEVICE_ERROR_HARDWARE_ERROR;
	}
	//return DEVICE_ERROR_HARDWARE_ERROR;
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

	int datalen = strlen((char *)g_data);
	Log::i("IDCardRead.getData", "getDatakeylen=%s,len=%d datalen=%d\n", key, len, datalen);
	if ((value != NULL || len != 0) && datalen > 0)
	{
		if (len > 38863)
		{
			len = 38863;
		}
		memset(value, 0, len);
		memcpy(value, g_data, len);
		memset(g_data, 0, 38863);
		Log::i("IDCardRead.getData", "%s=%s datalen=%d\n", key, value, len);
		return DEVICE_ERROR_SUCCESS;
	}
	//int datalen = 0;

	sParam p1;
	p1.ParamLen = strlen(key);
	p1.ParamData = new unsigned char[p1.ParamLen];

	memset(p1.ParamData, 0, p1.ParamLen);
	memcpy(p1.ParamData, key, p1.ParamLen);
	UCHAR tmp[38863] = { 0 };
	int tmplen = 38863;
	UCHAR  sendbuffer[256] = { 0 };
	int _len = 0;
	Utility::toPackData((UCHAR *)idcard_CMD, 0x02, sendbuffer, 256, &_len, 1, p1);
	// 	int iRet = transoket->WriteData(sendbuffer, _len);
	// 	iRet = transoket->ReadData(tmp, &tmplen, 6000);
	int iRet = transoket->TranData(sendbuffer, _len, tmp, &tmplen, 6000);
	if (iRet == 0)
	{
		PARAMLIST params;
		PARAMLIST::iterator i;
		iRet = Utility::unPackData(tmp, 1, &params);
		if (iRet != 0)
		{
			return DEVICE_ERROR_INTERNAL_ERROR;
		}
		i = params.begin();
		sParam revdata = (sParam)*i;
		//避免后面乱码
		char * cData = new char[revdata.ParamLen + 1];
		memset(cData, 0, revdata.ParamLen + 1);
		memcpy(cData, revdata.ParamData, revdata.ParamLen);

		string strdata(cData);
		string utfdata;
		unsigned char * b_utf_8;
		//Log::i("IDCardRead.getData", "%s=%s\n", key, strdata.c_str());
		datalen = Utility::GB18030ToUTF8((char *)strdata.c_str(), &b_utf_8);
		//string utf_8_data((char *)b_utf_8);
		memset(g_data, 0, 38863);
		memcpy(g_data, b_utf_8, datalen);
		if (value != NULL || len != 0)
		{
			if (len > 38863)
			{
				len = 38863;
			}

			memset(value, 0, len);
			memcpy(value, g_data, datalen + 1);
			memset(g_data, 0, 38863);
			Log::i("IDCardRead.getData", "%s=%s\n datalen=%d", key, value, datalen);
			return DEVICE_ERROR_SUCCESS;
		}
		delete[]p1.ParamData;
		delete[]revdata.ParamData;
		delete[]b_utf_8;
		params.clear();
		return datalen;
	}
	else
	{
		delete[]p1.ParamData;
		return DEVICE_ERROR_INTERNAL_ERROR;
	}
	

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
	return "";
}
void IDCardRead::RevReadCard(UCHAR * buffer)
{
	UCHAR tag = buffer[7];
	switch (tag)
	{
	case 0:
	{
		Log::i(CLASSNAME, "读卡成功！");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_SUCCESS, iReqid);
		break;
	}
	case 1:
	{
		Log::i(CLASSNAME, "设备未链接！");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_HARDWARE_ERROR, iReqid);
		break;
	}
	case 2://超时
	{
		Log::i(CLASSNAME, "读卡超时！");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_TIMEOUT, iReqid);
		break;
	}
	default:
		Log::i(CLASSNAME, "读卡失败！");
		m_pEventHandler->readCardCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid);
		break;
	}
}

unsigned char IDCardRead::g_data[38863] = { 0 };

#endif

ICardReader * APIENTRY createDevice()
{
	ICardReader *tmp = new IDCardRead();
	return tmp;
}
