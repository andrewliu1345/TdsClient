#ifndef TDS_DEVICE_IDEVICE_EVENT_HANDLER_H
#define TDS_DEVICE_IDEVICE_EVENT_HANDLER_H

#include "IDevice.h"

NS_TDS_DEVICE_BEGIN 

class IDeviceEventHandler
{
public:
	// 
	// 设别已经启动完成，可以进行设备操作
	// 设备在被创建以后，initialize函数将被调用。设备应当在initialize函数中
	// 开启设备启动线程。
	//
	virtual void initializeCompleted(const DeviceErrorCode result) = 0;

	//
	// transaction完成事件，其中参数含义如下：
	// nReqID : 异步操作标识号
	// tranID ：transaction函数中输入的交易标识号
	// parameter ： transaction调用时输入的参数，返还给transaction函数调用者
	// result : transaction调用的结果
	//
	virtual void transactionCompleted(const int nReqID, const char* tranID, const void* parameter, const void* result) = 0; 

    virtual ~IDeviceEventHandler() {}
};

NS_TDS_DEVICE_END  

#endif
