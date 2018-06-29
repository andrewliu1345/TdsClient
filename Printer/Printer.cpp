#include "stdafx.h"
#include "Printer.h"
#include "../LibLog/Log.h"
#include "../libUtility/Utility.h"
#include "IPrinterEventHandler.h"
#define CLASSNAME "Printer"
const UCHAR print_CMD[] = { 0xC8,0x01 };

void Printer::RevReadCard(UCHAR * buffer)
{
	UCHAR tag = buffer[7];
	switch (tag)
	{
	case 0:
	{
		Log::i(CLASSNAME, "打印成功！");
		m_pEventHandler->printFormCompleted(DEVICE_ERROR_SUCCESS, iReqid);
		break;
	}
	case 1:
	{
		Log::i(CLASSNAME, "设备未链接！");
		m_pEventHandler->printFormCompleted(DEVICE_ERROR_HARDWARE_ERROR, iReqid);
		break;
	}
	case 2://超时
	{
		Log::i(CLASSNAME, "打印超时！");
		m_pEventHandler->printFormCompleted(DEVICE_ERROR_TIMEOUT, iReqid);
		break;
	}
	default:
		Log::i(CLASSNAME, "打印失败！");
		m_pEventHandler->printFormCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid);
		break;
	}
}

Printer::Printer()
{
	Log::i(CLASSNAME, (char *)"Printer 构造\n");
	transoket = TranSocket::GetInstance();
}


Printer::~Printer()
{
}

void Printer::socketRevCallBack(unsigned char *buffer, int length)
{
	UCHAR cmd[2] = { 0 };
	memcpy(cmd, &buffer[3], 2);
	if (cmd[0] != print_CMD[0] || cmd[1] != print_CMD[1])
	{
		return;
	}
	UCHAR tag = buffer[5];
	switch (tag)
	{
	case 1://打印
	case 2:
	{
		RevReadCard(buffer);
		break;
	}

	default:
		m_pEventHandler->printFormCompleted(DEVICE_CARDREADER_INVALID_MEDIA, iReqid);
		break;
	}
}

void Printer::socketSendCallBack(unsigned char * buffer)
{
}

void Printer::socketdidConnectCallBack()
{
}

void Printer::socketdisConnectCallBack()
{
}

void Printer::socketErrCallBack()
{
}

void Printer::setDeviceEventHandler(void * pHandler)
{
	m_pEventHandler = static_cast<IPrinterEventHandler*>(pHandler);
}

void Printer::initialize(XmlParser * pConfig)
{
	Sleep(100);//让连接线程可以起来
	Log::i((char *)CLASSNAME, "Printer::initialize");
	if (transoket->GetIsConnected())
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_SUCCESS);
	}
	else
	{
		m_pEventHandler->initializeCompleted(DEVICE_ERROR_INTERNAL_ERROR);
	}
}

const char * Printer::getDeviceId()
{
	return "ReceiptPrinter";
}

bool Printer::isBusy()
{
	return false;
}

DeviceStatus Printer::getDeviceStatus()
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

void Printer::cancel(int nReqID)
{
}

int Printer::transaction(const char * tranID, const void * parameter, int * pReqID)
{
	return 0;
}

int Printer::readCard(int timeout, int * reqID)
{
	return 0;
}

int Printer::ejectCard(int * reqID)
{
	return 0;
}

int Printer::retainCard(int * reqID)
{
	return 0;
}

int Printer::resetRetainCount(int * reqID)
{
	return 0;
}

int Printer::getRetainCount()
{
	return 0;
}
#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
bool Printer::getData(const std::string & key, std::string & value)
{
	return false;
}

bool Printer::setData(const std::string & key, const std::string & value)
{
	return false;
}
#else
const int Printer::getData(const char * key, char * value, unsigned int len)
{
	return 0;
}

bool Printer::setData(const char * key, const char * value)
{
	return false;
}
int Printer::getLastError()
{
	return 0;
}
const char * Printer::getLastErrorDescription()
{
	return nullptr;
}
int Printer::printForm(const char * formName, const char * content, int * pReqID)
{
	Log::i("Printer.printForm", "formName=%s,content=%s\n", formName, content);
	sParam p1;
	p1.ParamLen = strlen(formName);
	p1.ParamData = new unsigned char[p1.ParamLen + 1];

	sParam p2;
	p2.ParamLen = strlen(content) + 1;
	p2.ParamData = new unsigned char[p2.ParamLen];

	memset(p1.ParamData, 0, p1.ParamLen);
	memcpy(p1.ParamData, formName, p1.ParamLen);

	memset(p2.ParamData, 0, p2.ParamLen);
	memcpy(p2.ParamData, content, p2.ParamLen);

	UCHAR tmp[256] = { 0 };
	//int tmplen = 256;
	UCHAR  sendbuffer[2048] = { 0 };
	int _len = 0;
	Utility::toPackData((UCHAR *)print_CMD, 0x02, sendbuffer, 2048, &_len, 2, p1, p2);
	int iRet = transoket->SyncTranData(sendbuffer, _len, this, 2000);
	InterlockedIncrement((LPLONG)&iReqid);
	if (iRet > 0)
	{
		//iRet = transoket->ReadData(this, 2000);//开启读取异步线程
		return DEVICE_ERROR_SUCCESS;
	}
	else
	{
		return DEVICE_ERROR_HARDWARE_ERROR;
	}
	return 0;
}
int Printer::printData(const void * data, int nSize, int * pReqID)
{
	return 0;
}
int Printer::read(const int timeout, int * pReqID)
{
	return 0;
}
IPrinter::PaperStatus Printer::getPaperStatus(const char * pos)
{
	return PAPER_NOTSUPP;
}
#endif
IPRINTER IPrinter* APIENTRY createDevice()
{

	return new Printer;
}