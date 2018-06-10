#ifndef TDS_DEVICE_CARDREADER_EVENT_H
#define TDS_DEVICE_CARDREADER_EVENT_H

#include "../IDeviceEventHandler.h"

NS_TDS_DEVICE_BEGIN

class ICardReaderEventHandler : public IDeviceEventHandler
{
public:
	// ִ���¼�
	virtual void readCardCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void ejectCardCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void retainCardCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void resetCountCompleted(const int nReqID) = 0;

	// �豸�����¼��������¼���nReqIDΪ������¼��Ĳ������ص�requestID
	virtual void mediaInserted(const int nReqID) = 0;
	virtual void mediaRemoved(const int nReqID) = 0;
};

NS_TDS_DEVICE_END

#endif