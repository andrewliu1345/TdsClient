#include "stdafx.h"
#include "MagneticCardReader.h"
#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"

const UCHAR mrcard_CMD[] = { 0x32,0x60 };
const UCHAR *classname = (UCHAR *)"MagneticCardReader";

void MagneticCardReader::RevReadCard(UCHAR* buffer)
{
	UCHAR tag = buffer[7];
	switch (tag)
	{
	case 0:
	{
		int datalen = 0;
		PARAMLIST params;
		PARAMLIST::iterator i;
		int iRet = Utility::unPackData(buffer, 1, &params);
		if (iRet != 0)
		{
			m_pEventHandler->readCardCompleted(DEVICE_CARDREADER_INVALID_MEDIA, tag);
		}
		i = params.begin();
		sParam revdata = (sParam)*i;
		//避免后面乱码

		memset(&_MANAGED, 0, revdata.ParamLen);
		memcpy(&_MANAGED, revdata.ParamData, revdata.ParamLen);
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

MagneticCardReader::MagneticCardReader()
{
	Log::i((char *)classname, (char *)"MagneticCardReader 构造\n");
	transoket = TranSocket::GetInstance();

}


MagneticCardReader::~MagneticCardReader()
{
}


void MagneticCardReader::socketRevCallBack(unsigned char *buffer)
{
	UCHAR cmd[2] = { 0 };
	memcpy(cmd, &buffer[3], 2);
	if (cmd[0] != mrcard_CMD[0] || cmd[1] != mrcard_CMD[1])
	{
		return;
	}
	UCHAR tag = buffer[5];
	switch (tag)
	{
	case 1:
	{
		RevReadCard(buffer);
		break;
	}
	default:
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_HARDWARE_ERROR, 1);
		break;
	}
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
	if (transoket->GetIsConnected())
	{
		return DEVICE_STATUS_ONLINE;
	}
	else
	{
		return DEVICE_STATUS_HWERROR;
	}
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

	int _timeout = 30;
	if (timeout != 0)
	{
		_timeout = timeout;
	}
	Log::i((const char *)classname, "timeout：%d ", timeout);
	sParam p1;
	p1.ParamData = new unsigned char[1];
	p1.ParamData[0] = (UCHAR)_timeout;
	p1.ParamLen = 1;
	int len = 0;
	UCHAR  sendbuffer[32] = { 0 };
	Utility::toPackData((UCHAR *)mrcard_CMD, 0x01, sendbuffer, 32, &len, 1, p1);
	int iRet = transoket->WriteData(sendbuffer, len);
	if (iRet > 0)
	{
		iRet = transoket->ReadData(this, _timeout*1000);//开启读取异步线程
		InterlockedIncrement((LPLONG)&iReqid);
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_INTERNAL_ERROR, 1);
		return DEVICE_ERROR_HARDWARE_ERROR;
	}

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
	Log::i("IDCardRead.getData", "getDatakeylen=%s,len=%d\n", key, len);
	// 	if (value != NULL || len != 0)
	// 	{
	// 		if (len > 70)
	// 		{
	// 			return DEVICE_ERROR_INTERNAL_ERROR;
	// 		}
	// 		
	// 		return DEVICE_ERROR_SUCCESS;
	// 	}
	int Rlen = 0;
	if (!strcmp(key, "track2"))
	{
		if (_MANAGED.track2_len != 0) {
			Rlen = _MANAGED.track2_len;
			if (value != NULL)
			{
				memcpy(value, _MANAGED.track2_data, Rlen);
				return DEVICE_ERROR_SUCCESS;
			}

			return Rlen;
		}
	}
	else if (!strcmp(key, "track3"))
	{
		if (_MANAGED.track3_len != 0) {
			Rlen = _MANAGED.track3_len;
			if (value != NULL)
			{
				memcpy(value, _MANAGED.track3_data, Rlen);
				return DEVICE_ERROR_SUCCESS;
			}

			return Rlen;
		}
	}
	else {

		return 0;
	}

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