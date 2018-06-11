#ifndef TDS_DEVICE_IPINPAD_H
#define TDS_DEVICE_IPINPAD_H

#include "../IDevice.h"
#include "IPinpadEventHandler.h"

#include <string>
#define IPINPAD __declspec(dllexport)


NS_TDS_DEVICE_BEGIN

class IPinpad : public IDevice
{
	// 从IDevice继承而来的方法
public:
	virtual const char* getDeviceClass() = 0;


	virtual void initialize(XmlParser* pConfig) = 0;

	virtual void setDeviceEventHandler(void* pHandler) = 0;


	virtual const char* getDeviceId() = 0;
	virtual bool isBusy()=0;
	virtual DeviceStatus getDeviceStatus() = 0;
	virtual void cancel(int nReqID) = 0;

	virtual int transaction(const char* tranID, const void* parameter, int* pReqID = NULL) = 0;

	// 密码键盘操作
public:
	//
	// 导入密钥
	// keyname : 要导入的密钥名称
	// key : 要导入的密钥数据，
	// keyuse: 密钥用途
	// encKey ：导入之前用何密钥解密
	//
	virtual int importKey(const char* keyname, const char* data, const char* keyuse, const char* encKeyName = NULL, int* pReqID = NULL) = 0;

	// 
	// 清除所有密钥，此操作为同步操作
	//
	virtual int clearKey() = 0;

	// 加密
	virtual int encrypt(const char* keyname, const char* algorithm, const char* dataIn, int* pReqID = NULL) = 0;

	// 解密
	virtual int decrypt(const char* keyname, const char* algorithm, const char* dataIn, int* pReqID = NULL) = 0;

	// 开启普通键盘模式，只有cancel操作能终止此状态
	virtual int getData(int* pReqID = NULL) = 0;

	//
	// 开启密码输入模式
	// min ： 最小输入
	// max ： 最大输入
	// 在用户按了Enter按键或者用户调用了cancel后，此状态结束
	//
	virtual int getPin(const int min = 3, const int max = 6, int* pReqID = NULL) = 0;

	// 软件结束getPin，使其以DEVICE_SUCCESS的形式返回
	virtual void endGetPin() = 0;

	//
	// 计算密码
	//
	virtual int getPinblock(const char* keyname, const char* cryptData = NULL, int* pReqID = NULL) = 0;

	// 读卡器事件
private:


protected:
	IPinpadEventHandler * m_pEventHandler;
	//
};

NS_TDS_DEVICE_END




#endif