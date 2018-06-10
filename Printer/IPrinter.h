#ifndef TDS_DEVICE_IPRINTER_H
#define TDS_DEVICE_IPRINTER_H

#include "../IDevice.h"
#include <string>

#define IPRINTER __declspec(dllexport)

NS_TDS_DEVICE_BEGIN

class IPrinterEventHandler;

//////////////////
class IPrinter : public IDevice
{
public:
	enum PaperStatus
	{
		PAPER_EMPTY = 0, //供纸器空了
		PAPER_FULL = 1, //供纸器纸满
		PAPER_LOW = 2, //供纸器纸将尽
		PAPER_PRESENT = 3,	//	介质在打印机内
		PAPER_JAMMED = 4,	//	介质阻塞打印机内
		PAPER_NOTSUPP = 5,	//	打印机不能检测介质状态
		PAPER_UNKNOWN = 6,	//	介质状态不明
		PAPER_ENTERING = 7,	//	介质在入口处
		PAPER_RETRACTED = 8	//	介质被回收

	};

	/*	enum PrintDataStatus
		{
			WFS_SUCCESS,					//	成功
			WFS_ERR_HARDWARE_ERROR,			//	硬件故障
			WFS_ERR_INVALID_DATA,			//	数据无效，比如为空
			WFS_ERR_PRR_DATA_TOO_LONG		//	数据太长

		};*/

		// 从IDevice继承而来的方法
public:
	// 读卡器的型别为 "CardReaderService"
	virtual const char* getDeviceClass()//获取设备类别标识
	{
		return "PrinterService";
	}


	virtual void initialize(XmlParser* pConfig) = 0;//对设备进行初始化

	virtual void setDeviceEventHandler(void* pHandler) = 0;


	virtual const char* getDeviceId() = 0;
	virtual bool isBusy() = 0;
	virtual DeviceStatus getDeviceStatus() = 0;
	virtual void cancel(int nReqID) = 0;
	virtual int transaction(const char* tranID, const void* parameter, int* pReqID = NULL) = 0;


	// 打印机接口
public:
	//
	// 按照预先定义的Form格式来打印数据。
	// Form的内容由农行和厂商沟通商定。
	// 银行方面不需要理解Form的编写格式等信息，只需要
	// 了解Form的名称以及打印出的效果即可。
	// Form的各个域之间用 # 分隔，## 则表示#号
	// 例如:  Bank=北京分行#Name=客户
	// 打印完成后，设备应当自行切纸
	//
	// how to print bitmap from js
	virtual int printForm(const char* formName, const char* content, int* pReqID = NULL) = 0;

	//
	// 打印裸数据，裸数据的格式根据不同的设备型别作特殊说明
	// 
	virtual int printData(const void* data, int nSize, int* pReqID = NULL) = 0;

	//
	// 获取指定区域的纸状态
	// 非特殊打印机，pos传入NULL，对于特殊打印机，pos有其特殊含义
	virtual PaperStatus getPaperStatus(const char* pos = NULL) = 0;



	//
	// 对于存折打印机，可用于获取存折上的数据
	//
#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
	virtual bool getData(const std::string& key, std::string& val) = 0;
#else
	virtual const int getData(const char* key, char* value, unsigned int len) = 0;
#endif
	//
	// 读存折， timeout为超时时间
	//
	virtual int read(const int timeout, int* pReqID = 0) = 0;


protected:
	IPrinterEventHandler * m_pEventHandler;
	//	HANDLE hMutex;
};

NS_TDS_DEVICE_END



#endif
