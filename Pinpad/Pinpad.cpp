#include "stdafx.h"
#include "Pinpad.h"

#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"
#define  MAINKEY "MAINKEY"
#define WORKKEY "WORKKEY"

const UCHAR Pinpad_CMD[] = { 0xC6,0x03 };
void Pinpad::socketRevCallBack(unsigned char * buffer)
{
	UCHAR cmd[2] = { 0 };
	memcpy(cmd, &buffer[3], 2);
	if (cmd[0] != Pinpad_CMD[0] || cmd[1] != Pinpad_CMD[1])
	{
		//m_pEventHandler->importKeyCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid);
		return;
	}
	UCHAR tag = buffer[5];
	switch (tag)
	{
	case 1://��ȡ���루���ģ�pinBlock
	{
		RevGetPinBlock(buffer);
		break;
	}
	case 2:
	{

		break;
	}
	case 3://����Կ
	{
		RevImport(buffer);
		break;
	}
	case 4://������Կ
	{
		RevImport(buffer);
		break;
	}
	default:
		break;
	}
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
	m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_TIMEOUT, iReqid, NULL);
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

	//CString cskeyname;
	string skeyname(keyname);//mainkey-����Կ workkey-������Կ
	string skeydata(data);
	string skeyuse;
	string senckeyname;//��ʱΪ��������

	if (keyuse != NULL)
	{
		skeyuse = keyuse;
	}
	else
	{
		skeyuse = "";
	}
	if (encKeyName != NULL)
	{
		senckeyname = encKeyName;
	}
	else
	{
		senckeyname = "";
	}
	skeyname = Utility::ToUpper(skeyname);
	skeyuse = Utility::ToUpper(skeyuse);
	senckeyname = Utility::ToUpper(senckeyname);
	Log::i(CLASSNAME, "skeyname=%s,skeyuse=%s,senckeyname=%s", skeyname.c_str(), skeyuse.c_str(), senckeyname.c_str());//��ӡ��־
	int iRet = DEVICE_ERROR_HARDWARE_ERROR;
	if (skeyname == MAINKEY)
	{
		iRet = LoadMKey(skeydata, senckeyname);
	}
	else if (skeyname == WORKKEY)
	{
		iRet = LoadWKey(skeydata, skeyuse, senckeyname);
	}


	InterlockedIncrement((LPLONG)&iReqid);
	return iRet;
}
int Pinpad::clearKey()
{
	return DEVICE_ERROR_SUCCESS;
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
	sParam spKeyName;
	sParam spCryptData;

	string strKeyName(keyname);
	string strCrytData(cryptData);

	int KeyNamelen = strlen(strKeyName.c_str());
	int strCrytDatalen = strlen(strCrytData.c_str());

	spKeyName.ParamData = new unsigned char[KeyNamelen + 1];
	spCryptData.ParamData = new unsigned char[strCrytDatalen + 1];

	memset(spKeyName.ParamData, 0, KeyNamelen + 1);
	memset(spCryptData.ParamData, 0, strCrytDatalen + 1);

	spKeyName.ParamLen = KeyNamelen;
	spCryptData.ParamLen = strCrytDatalen;

	memcpy(spKeyName.ParamData, strKeyName.c_str(), KeyNamelen);
	memcpy(spCryptData.ParamData, strCrytData.c_str(), strCrytDatalen);

	UCHAR  sendbuffer[256] = { 0 };
	int len = 0;
	Utility::toPackData((UCHAR *)Pinpad_CMD, 0x01, sendbuffer, 256, &len, 2, spKeyName, spCryptData);
	//��Դ����
	delete[] spKeyName.ParamData;
	delete[] spCryptData.ParamData;

	int iRet = transoket->WriteData(sendbuffer, len);
	if (iRet > 0)
	{
		iRet = transoket->ReadData(this,30000);//������ȡ�첽�߳�
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		return DEVICE_ERROR_HARDWARE_ERROR;
	}
	return 0;
}


int Pinpad::LoadMKey(string Mkey, string encIndex)
{
	sParam spMkey;
	sParam spEncIndex;

	int Mkeylen = strlen(Mkey.c_str());
	int encIndexlen = strlen(encIndex.c_str());

	spMkey.ParamData = new unsigned char[Mkeylen + 1];
	spEncIndex.ParamData = new unsigned char[encIndexlen + 1];

	memset(spMkey.ParamData, 0, Mkeylen + 1);
	memset(spEncIndex.ParamData, 0, encIndexlen + 1);

	spMkey.ParamLen = Mkeylen;
	spEncIndex.ParamLen = encIndexlen;

	memcpy(spMkey.ParamData, Mkey.c_str(), Mkeylen);
	memcpy(spEncIndex.ParamData, encIndex.c_str(), encIndexlen);

	UCHAR  sendbuffer[256] = { 0 };
	int len = 0;
	Utility::toPackData((UCHAR *)Pinpad_CMD, 0x03, sendbuffer, 256, &len, 2, spMkey, spEncIndex);
	//��Դ����
	delete[] spMkey.ParamData;
	delete[] spEncIndex.ParamData;

	int iRet = transoket->WriteData(sendbuffer, len);
	if (iRet > 0)
	{
		iRet = transoket->ReadData(this, 6000);//������ȡ�첽�߳�
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		return DEVICE_ERROR_HARDWARE_ERROR;
	}

}

int Pinpad::LoadWKey(string Wkey, string keyuse, string encIndex)
{
	sParam spWkey;
	sParam spKeyUse;
	sParam spEncIndex;

	int Wkeylen = strlen(Wkey.c_str());
	int encIndexlen = strlen(encIndex.c_str());
	int keyuselen = strlen(keyuse.c_str());

	spWkey.ParamData = new unsigned char[Wkeylen + 1];
	spEncIndex.ParamData = new unsigned char[encIndexlen + 1];
	spKeyUse.ParamData = new unsigned char[keyuselen + 1];

	memset(spWkey.ParamData, 0, Wkeylen + 1);
	memset(spEncIndex.ParamData, 0, encIndexlen + 1);
	memset(spKeyUse.ParamData, 0, keyuselen + 1);

	spWkey.ParamLen = Wkeylen;
	spEncIndex.ParamLen = encIndexlen;
	spKeyUse.ParamLen = keyuselen;

	memcpy(spWkey.ParamData, Wkey.c_str(), Wkeylen);
	memcpy(spEncIndex.ParamData, encIndex.c_str(), encIndexlen);
	memcpy(spKeyUse.ParamData, keyuse.c_str(), keyuselen);

	UCHAR  sendbuffer[256] = { 0 };
	int len = 0;
	Utility::toPackData((UCHAR *)Pinpad_CMD, 0x04, sendbuffer, 256, &len, 3, spWkey, spEncIndex, spKeyUse);
	//��Դ����
	delete[] spWkey.ParamData;
	delete[] spEncIndex.ParamData;
	delete[] spKeyUse.ParamData;
	int iRet = transoket->WriteData(sendbuffer, len);

	if (iRet > 0)
	{
		iRet = transoket->ReadData(this, 6000);//������ȡ�첽�߳�
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		return DEVICE_ERROR_HARDWARE_ERROR;
	}
}

IPinpad  * APIENTRY createDevice()
{
	IPinpad *tmp = new Pinpad();
	return tmp;
}
void Pinpad::RevImport(UCHAR * buffer)
{
	UCHAR tag = buffer[7];
	switch (tag)
	{
	case 0:
	{
		Log::i(CLASSNAME, "�ɹ���");
		m_pEventHandler->importKeyCompleted(DEVICE_ERROR_SUCCESS, iReqid);
		break;
	}
	case 1:
	{
		Log::i(CLASSNAME, "�豸δ���ӣ�");
		m_pEventHandler->importKeyCompleted(DEVICE_ERROR_HARDWARE_ERROR, iReqid);
		break;
	}
	case 2://��ʱ
	{
		Log::i(CLASSNAME, "��ʱ��");
		m_pEventHandler->importKeyCompleted(DEVICE_ERROR_TIMEOUT, iReqid);
		break;
	}
	default:
		Log::i(CLASSNAME, "ʧ�ܣ�");
		m_pEventHandler->importKeyCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid);
		break;
	}
}

void Pinpad::RevGetPinBlock(UCHAR * buffer)
{
	UCHAR tag = buffer[7];
	switch (tag)
	{
	case 0:
	{
		PARAMLIST params;
		PARAMLIST::iterator i;
		int iRet = Utility::unPackData(buffer, 1, &params);
		if (iRet != 0)
		{
			m_pEventHandler->getPinblockCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid, NULL);
			return;
		}
		i = params.begin();
		sParam revdata = (sParam)*i;
		string hexstrData = Utility::bytesToHexstring(revdata.ParamData, revdata.ParamLen);

		Log::i(CLASSNAME, "�ɹ���hexstrData=%s", hexstrData.c_str());
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_SUCCESS, iReqid, hexstrData.c_str());
		break;
	}
	case 1:
	{
		Log::i(CLASSNAME, "�豸δ���ӣ�");
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_HARDWARE_ERROR, iReqid, NULL);
		break;
	}
	case 2://��ʱ
	{
		Log::i(CLASSNAME, "��ʱ��");
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_TIMEOUT, iReqid, NULL);
		break;
	}
	default:
		Log::i(CLASSNAME, "ʧ�ܣ�");
		m_pEventHandler->getPinblockCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid, NULL);
		break;
	}
}
