#ifndef TDS_DEVICE_PRINTER_EVENT_HANDLER_H
#define TDS_DEVICE_PRINTER_EVENT_HANDLER_H

#include "../IDeviceEventHandler.h"


NS_TDS_DEVICE_BEGIN

class IPrinterEventHandler 
	: public IDeviceEventHandler
{
	// 设备执行事件
public:
	virtual void printFormCompleted(const DeviceErrorCode result, const int nReqID)=0;
	virtual void printDataCompleted(const DeviceErrorCode result, const int nReqID)=0;
	virtual void readCompleted(const DeviceErrorCode result, const int nReqID) = 0;

	// 设备报警事件 
public:
	// 设备缺纸
	virtual void paperThreshold(const char* pos, const IPrinter::PaperStatus status)=0;
};

NS_TDS_DEVICE_END

#endif
