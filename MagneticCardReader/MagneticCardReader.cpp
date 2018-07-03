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
		//�����������

		memset(&_MANAGED, 0, revdata.ParamLen);
		memcpy(&_MANAGED, revdata.ParamData, revdata.ParamLen);
		Log::i(CLASSNAME, "�����ɹ���");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_SUCCESS, iReqid);
		break;
	}
	case 1:
	{
		Log::i(CLASSNAME, "�豸δ���ӣ�");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_HARDWARE_ERROR, iReqid);
		break;
	}
	case 2://��ʱ
	{
		Log::i(CLASSNAME, "������ʱ��");
		m_pEventHandler->readCardCompleted(DEVICE_ERROR_TIMEOUT, iReqid);
		break;
	}
	default:
		Log::i(CLASSNAME, "����ʧ�ܣ�");
		m_pEventHandler->readCardCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid);
		break;
	}
}

MagneticCardReader::MagneticCardReader()
{
	Log::i((char *)classname, (char *)"MagneticCardReader ����\n");
	transoket = TranSocket::GetInstance();

}


MagneticCardReader::~MagneticCardReader()
{
}


void MagneticCardReader::socketRevCallBack(unsigned char *buffer, int length)
{
	Log::i((const char *)classname, "socketRevCallBack �������� buffer=%s", Utility::bytesToHexstring(buffer, length).c_str());

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
	Sleep(100);//�������߳̿�������
	Log::i((char *)classname, "transoket->GetIsConnected()=%d", transoket->GetIsConnected());
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
	int iReqID;
	iReqID = iReqid;
	*pReqID = iReqID;
	InterlockedIncrement((LPLONG)&iReqid);

	return 0;
}

int MagneticCardReader::readCard(int timeout, int* reqID)
{
	iReqid = 1;
	int _timeout = 10;
	if (timeout != 0)
	{
		_timeout = timeout;
	}
	Log::i((const char *)classname, "timeout��%d ", timeout);
	sParam p1;
	p1.ParamData = new unsigned char[1];
	p1.ParamData[0] = (UCHAR)_timeout;
	p1.ParamLen = 1;
	int len = 0;
	UCHAR  sendbuffer[32] = { 0 };
	Utility::toPackData((UCHAR *)mrcard_CMD, 0x01, sendbuffer, 32, &len, 1, p1);
	int iRet = transoket->SyncTranData(sendbuffer, len, this, _timeout * 1000);
	if (iRet > 0)
	{
		//iRet = transoket->ReadData(this, _timeout*1000);//������ȡ�첽�߳�
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
	m_pEventHandler->ejectCardCompleted(DEVICE_ERROR_OP_NOT_SUPPORTED, iReqid);
	InterlockedIncrement((LPLONG)&iReqid);

	return DEVICE_ERROR_OP_NOT_SUPPORTED;
}

int MagneticCardReader::retainCard(int* reqID)
{
	InterlockedIncrement((LPLONG)&iReqid);

	return DEVICE_ERROR_OP_NOT_SUPPORTED;
}

int MagneticCardReader::resetRetainCount(int* reqID)
{
	InterlockedIncrement((LPLONG)&iReqid);
	return DEVICE_ERROR_OP_NOT_SUPPORTED;
}

int MagneticCardReader::getRetainCount()
{
	return DEVICE_ERROR_OP_NOT_SUPPORTED;
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
	Log::i("MagneticCardReader.getData", "key=%s,len=%d\n", key, len);
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
	if (!strcmp(key, "track1"))
	{
		if (_MANAGED.track1_len != 0) {
			Rlen = _MANAGED.track1_len;
			if (value != NULL)
			{
				memcpy(value, _MANAGED.track1_data, Rlen);
				return DEVICE_ERROR_SUCCESS;
			}

			return Rlen;
		}
	}
	else	if (!strcmp(key, "track2"))
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