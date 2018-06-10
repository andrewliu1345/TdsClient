#ifndef TDS_DEVICE_CARDREADER_EVENT_H
#define TDS_DEVICE_CARDREADER_EVENT_H

#include "../IDeviceEventHandler.h"

NS_TDS_DEVICE_BEGIN

class ICardReaderEventHandler : public IDeviceEventHandler
{
public:
	// 执行事件
	virtual void readCardCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void ejectCardCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void retainCardCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void resetCountCompleted(const int nReqID) = 0;

	// 设备服务事件，服务事件的nReqID为引起此事件的操作返回的requestID
	virtual void mediaInserted(const int nReqID) = 0;
	virtual void mediaRemoved(const int nReqID) = 0;
};

NS_TDS_DEVICE_END

#endif