#ifndef TDS_DEVICE_PINPAD_EVENT_HANDLER_H
#define TDS_DEVICE_PINPAD_EVENT_HANDLER_H

#include "../IDeviceEventHandler.h"

NS_TDS_DEVICE_BEGIN

class IPinpadEventHandler 
	: public IDeviceEventHandler
{
public:
	virtual void importKeyCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void encryptCompleted(const DeviceErrorCode result, const int nReqID, const char* algorithm, const char* dataIn, const char* dataOut) = 0;
	virtual void decryptCompleted(const DeviceErrorCode result, const int nReqID, const char* algorithm, const char* dataIn, const char* dataOut) = 0;
	virtual void getDataCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void getPinCompleted(const DeviceErrorCode result, const int nReqID) = 0;
	virtual void getPinblockCompleted(const DeviceErrorCode result, const int nReqID, const char* pinblock) = 0;
	virtual void keyPressed(const char* key) = 0;
};

NS_TDS_DEVICE_END

#endif