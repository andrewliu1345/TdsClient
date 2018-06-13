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
		return;
	}
	UCHAR tag = buffer[5];
	switch (tag)
	{
	case 1://获取密码（密文）pinBlock
	{
		
		break;
	}
	case 2:
	{
		break;
	}
	case 3:
	{
		break;
	}
	case 4:
	{
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
	Log::i(CLASSNAME, "skeyname=%s,skeyuse=%s,senckeyname=%s", skeyname.c_str(), skeyuse.c_str(), senckeyname.c_str());//打印日志
	int iRet = DEVICE_ERROR_HARDWARE_ERROR;
	if (skeyname == MAINKEY)
	{
		iRet=LoadMKey(skeydata, senckeyname);
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
	return 0;
}
void Pinpad::RevReadCard(UCHAR * buffer)
{
}

int Pinpad::LoadMKey(string Mkey, string encIndex)
{
	sParam spMkey;
	sParam spEncIndex;

	int Mkeylen = strlen(Mkey.c_str());
	int encIndexlen = strlen(encIndex.c_str());

	spMkey.ParamData = new unsigned char(Mkeylen);
	spEncIndex.ParamData = new unsigned char(encIndexlen);

	memset(spMkey.ParamData, 0, Mkeylen);
	memset(spEncIndex.ParamData, 0, encIndexlen);

	memcpy(spMkey.ParamData, Mkey.c_str(), Mkeylen);
	memcpy(spEncIndex.ParamData, encIndex.c_str(), encIndexlen);

	UCHAR  sendbuffer[256] = { 0 };
	int len = 0;
	Utility::toPackData((UCHAR *)Pinpad_CMD, 0x03, sendbuffer, 256, &len, 1, spMkey, spEncIndex);
	//资源回收
	delete[]spMkey.ParamData;
	delete[] spEncIndex.ParamData;

	int iRet = transoket->WriteData(sendbuffer, len);
	if (iRet > 0)
	{
		iRet = transoket->ReadData(this, 6000);//开启读取异步线程
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		return DEVICE_ERROR_HARDWARE_ERROR;
	}

}

int Pinpad::LoadWKey(string Wkey, string keyuse, string encIndex)
{
	sParam spMkey;
	sParam spKeyUse;
	sParam spEncIndex;

	int Wkeylen = strlen(Wkey.c_str());
	int encIndexlen = strlen(encIndex.c_str());
	int keyuselen = strlen(keyuse.c_str());

	spMkey.ParamData = new unsigned char(Wkeylen);
	spEncIndex.ParamData = new unsigned char(encIndexlen);
	spKeyUse.ParamData = new unsigned char(keyuselen);

	memset(spMkey.ParamData, 0, Wkeylen);
	memset(spEncIndex.ParamData, 0, encIndexlen);
	memset(spKeyUse.ParamData, 0, keyuselen);

	memcpy(spMkey.ParamData, Wkey.c_str(), Wkeylen);
	memcpy(spEncIndex.ParamData, encIndex.c_str(), encIndexlen);
	memcpy(spKeyUse.ParamData, keyuse.c_str(), keyuselen);

	UCHAR  sendbuffer[256] = { 0 };
	int len = 0;
	Utility::toPackData((UCHAR *)Pinpad_CMD, 0x04, sendbuffer, 256, &len, 1, spMkey, spKeyUse,spEncIndex);
	//资源回收
	delete[]spMkey.ParamData;
	delete[] spEncIndex.ParamData;

	int iRet = transoket->WriteData(sendbuffer, len);
	if (iRet > 0)
	{
		iRet = transoket->ReadData(this, 6000);//开启读取异步线程
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