#include "stdafx.h"
#include "Printer.h"


Printer::Printer()
{
}


Printer::~Printer()
{
}

void Printer::socketRevCallBack(unsigned char * buffer)
{
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
}

const char * Printer::getDeviceId()
{
	return nullptr;
}

bool Printer::isBusy()
{
	return false;
}

DeviceStatus Printer::getDeviceStatus()
{
	return DeviceStatus();
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