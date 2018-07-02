#include "stdafx.h"
#include "Pinpad.h"

#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"
#define  MAINKEY "MAINKEY"
#define WORKKEY "WORKKEY"

const UCHAR Pinpad_CMD[] = { 0xC6,0x03 };
void Pinpad::socketRevCallBack(unsigned char *buffer, int length)
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
	case 1://获取密码（密文）pinBlock
	{
		RevGetPinBlock(buffer);
		break;
	}
	case 2:
	{

		break;
	}
	case 3://主密钥
	{
		RevImport(buffer);
		break;
	}
	case 4://工作密钥
	{
		RevImport(buffer);
		break;
	}
	case 7:
	{
		RevGetPinBlock2(buffer);
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
	Log::i(CLASSNAME, (char *)"Pinpad socketErrCallBack\n");
	m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_TIMEOUT, iReqid, NULL);
}

Pinpad::Pinpad()
{
	Log::i(CLASSNAME, (char *)"Pinpad 构造\n");
	transoket = TranSocket::GetInstance();
}


Pinpad::~Pinpad()
{
}

void Pinpad::initialize(XmlParser * pConfig)

{
	Sleep(100);//让连接线程可以起来
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
	string skeyname(keyname);//mainkey-主密钥 workkey-工作密钥
	string skeydata(data);
	string skeyuse;
	string senckeyname;//空时为明文下载

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
	Log::i(CLASSNAME, "skeyname=%s,skeyuse=%s,senckeyname=%s,key=%s", skeyname.c_str(), skeyuse.c_str(), senckeyname.c_str(), data);//打印日志
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
	Utility::toPackData((UCHAR *)Pinpad_CMD, 0x07, sendbuffer, 256, &len, 2, spKeyName, spCryptData);
	//资源回收
	delete[] spKeyName.ParamData;
	delete[] spCryptData.ParamData;

	int iRet = transoket->SyncTranData(sendbuffer, len, this, 30000);
	if (iRet > 0)
	{
		//iRet = transoket->ReadData(this,30000);//开启读取异步线程
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
	//资源回收
	delete[] spMkey.ParamData;
	delete[] spEncIndex.ParamData;

	int iRet = transoket->SyncTranData(sendbuffer, len, this, 6000);
	if (iRet > 0)
	{
		//iRet = transoket->ReadData(this, 6000);//开启读取异步线程
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
	//资源回收
	delete[] spWkey.ParamData;
	delete[] spEncIndex.ParamData;
	delete[] spKeyUse.ParamData;
	int iRet = transoket->SyncTranData(sendbuffer, len, this, 10000);

	if (iRet > 0)
	{
		//iRet = transoket->ReadData(this, 10000);//开启读取异步线程
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
		Log::i(CLASSNAME, "成功！");
		m_pEventHandler->importKeyCompleted(DEVICE_ERROR_SUCCESS, iReqid);
		break;
	}
	case 1:
	{
		Log::i(CLASSNAME, "设备未链接！");
		m_pEventHandler->importKeyCompleted(DEVICE_ERROR_HARDWARE_ERROR, iReqid);
		break;
	}
	case 2://超时
	{
		Log::i(CLASSNAME, "超时！");
		m_pEventHandler->importKeyCompleted(DEVICE_ERROR_TIMEOUT, iReqid);
		break;
	}
	default:
		Log::i(CLASSNAME, "失败！");
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

		Log::i(CLASSNAME, "成功！hexstrData=%s", hexstrData.c_str());
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_SUCCESS, iReqid, hexstrData.c_str());
		break;
	}
	case 1:
	{
		Log::i(CLASSNAME, "设备未链接！");
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_HARDWARE_ERROR, iReqid, NULL);
		break;
	}
	case 2://超时
	{
		Log::i(CLASSNAME, "超时！");
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_TIMEOUT, iReqid, NULL);
		break;
	}
	default:
		Log::i(CLASSNAME, "失败！");
		m_pEventHandler->getPinblockCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid, NULL);
		break;
	}
}
void Pinpad::RevGetPinBlock2(UCHAR * buffer)
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
		//string hexstrData = Utility::bytesToHexstring(revdata.ParamData, revdata.ParamLen);
		char * cData = new char[revdata.ParamLen + 1];//避免后面乱码
		memset(cData, 0, revdata.ParamLen + 1);
		memcpy(cData, revdata.ParamData, revdata.ParamLen);
		Log::i(CLASSNAME, "成功！cData=%s,revdata.ParamLen =%d", cData, revdata.ParamLen);
		unsigned char * b_utf_8;
		Utility::GB18030ToUTF8(cData, &b_utf_8);
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_SUCCESS, iReqid, (char *)b_utf_8);
		delete[]b_utf_8;
		delete[] cData;
		break;
	}
	case 1:
	{
		Log::i(CLASSNAME, "设备未链接！");
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_HARDWARE_ERROR, iReqid, NULL);
		break;
	}
	case 2://超时
	{
		Log::i(CLASSNAME, "超时！");
		m_pEventHandler->getPinblockCompleted(DEVICE_ERROR_TIMEOUT, iReqid, NULL);
		break;
	}
	default:
		Log::i(CLASSNAME, "失败！");
		m_pEventHandler->getPinblockCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid, NULL);
		break;
	}
}
