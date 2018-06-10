#ifndef TDS_DEVICE_PRINTER_EVENT_HANDLER_H
#define TDS_DEVICE_PRINTER_EVENT_HANDLER_H

#include "../IDeviceEventHandler.h"


NS_TDS_DEVICE_BEGIN

class IPrinterEventHandler 
	: public IDeviceEventHandler
{
	// �豸ִ���¼�
public:
	virtual void printFormCompleted(const DeviceErrorCode result, const int nReqID)=0;
	virtual void printDataCompleted(const DeviceErrorCode result, const int nReqID)=0;
	virtual void readCompleted(const DeviceErrorCode result, const int nReqID) = 0;

	// �豸�����¼� 
public:
	// �豸ȱֽ
	virtual void paperThreshold(const char* pos, const IPrinter::PaperStatus status)=0;
};

NS_TDS_DEVICE_END

#endif
