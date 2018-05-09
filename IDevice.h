#ifndef TDS_DEIVCE_IDEVICE_H
#define TDS_DEIVCE_IDEVICE_H

#define NS_TDS_DEVICE_BEGIN namespace nsTDS { namespace nsDevice {
#define NS_TDS_DEVICE_END   } }

#include <string>
#include "../tds_global.h"

class XmlParser;
class IDeviceEventHandler;

#ifndef NULL
#define NULL 0
#endif

#define DEVICE_INTERFACE_VERSION_01 
#define DEVICE_INTERFACE_VERSION_02
#define DEVICE_INTERFACE_VERSION_ANDROID


//#define DEVICE_INTERFACE_VERSION_ANDROID

NS_TDS_DEVICE_BEGIN

////所有枚举类型权证IDevice.h里定义，不然，事件编译有问题
//enum PaperStatus
//{
//	PAPER_EMPTY, 
//	PAPER_FULL,
//	PAPER_LOW
//};

enum CardIssuerType
{
	/// <summary>
	/// 支持IC卡
	/// </summary>
	CIT_ICCard = 1,

	/// <summary>
	/// 支持磁条卡
	/// </summary>
	CIT_MegCard = 2,

	// 所有的都支持
	CIT_All = 3
};

//
// 设备操作错误码
//
enum DeviceErrorCode
{
	///////////////////////////////////////////////
	// 通用错误码
	DEVICE_ERROR_SUCCESS = 0,   // 执行成功
	DEVICE_ERROR_CANCELD = -4,  // 操作被取消
	DEVICE_ERROR_HARDWARE_ERROR = -54,   // 设备硬件错误
	DEVICE_ERROR_INTERNAL_ERROR = -14,   // SP程序内部错误
	DEVICE_ERROR_NOT_EXISTED,            // 设备部存在，此错误与SP无关

	// 对于多个逻辑设备共享一个物理设备的情况，此错误标识有其他逻辑设备正在独占
	// 访问此物理设备，造成本逻辑设备目前暂时无法完成此动作
	// 此错误码为通用错误码，对于每一类设备，定义其自身的Busy错误码。
	DEVICE_ERROR_BUSY,
	DEVICE_ERROR_OP_NOT_SUPPORTED,  // 该型别设备不支持此项操作
	DEVICE_ERROR_TIMEOUT,           // 操作在一定时间内未能完成

	///////////////////////////////////////////////////////
	// 读卡器类型错误
	DEVICE_CARDREADER_INVALID_MEDIA,    // 读卡器中插入了无效的介质
	DEVICE_CARDREADER_MEDIA_PRESENT,    // 卡已经在读卡器中
	DEVICE_CARDREADER_READ_IN_PROGRESS, // 已经在读卡
	DEVICE_CARDREADER_BUSY_MEDIA_PRESENT,  // 其他逻辑读卡器已经有卡在卡槽当中

	/////////////////////////////////////////////////////////
	// 密码键盘类型错误
	DEVICE_PINPAD_KEY_NOT_EXISTED,   // 密钥不存在
	DEVICE_PINPAD_KEY_INVALID_KEY,   // 密钥无效
	DEVICE_PINPAD_KEY_COUNT_ERROR,   // 输入的密码位数错误


	DEVICE_ERROR_END
};

//
// 设备总体状态
//
enum DeviceStatus
{
	// 正常状态
	DEVICE_STATUS_ONLINE,

	// 设备故障
	DEVICE_STATUS_HWERROR,

	DEVICE_STATUS_END
};

//
// IDevice为自助设备类均需要实现的抽象接口。
// 自助设备将按照功能划分为不同的类别（class），典型的类别包括：
// 读卡器、打印机以及密码键盘灯。
//
//     对于某一种类别的设备，又会根据其应用场景不同，划分为不同的
// 型别（type）。我们规定不同型别的设备，在某一台自助终端上只允许
// 存在最多一个实例，也就是说，我们可以根据设备型别，来唯一标识该
// 设备，因此型别也可称作此设备的ID。
//
//     为了便于设备的控制，在某些情况下需要将一种类型的物理设备分割
// 为多个逻辑设备，以分别控制。
//     例如对于吸入式读卡器而言，可同时读取磁条卡及IC卡。可以将吸入式
// 读卡器划分为吸入式磁条卡读卡器以及吸入式IC卡读卡器两种读卡器。
//
// 读卡器类别设备包含的典型的型别有：
// 吸入式磁条卡读卡器
// 吸入式IC卡读卡器
// 非接触式磁条卡读卡器
// 非接触式IC卡读卡器
// 电卡读卡器
// 水卡读卡器  
// 二代证读卡器
//
class IDevice
{
public:
	IDevice() {}
	virtual ~IDevice() {}

public:
	//
	// 获取接口版本信息。应用程序在调用设备接口之前，首先会判断此接口的版本是否符合要求
	// 对于接口版本不符合要求的，应用程序应当提示设备驱动升级。
	//
	virtual const char* getInterfaceVersion()
	{
		return "v0.2";
	}

	//
	// 对设备进行初始化
	// pConfig为设备配置节点信息
	//
	virtual void initialize(XmlParser* pConfig) = 0;

	virtual void setDeviceEventHandler(void* pHandler) = 0;

	//
	// 获取设备的型别标识
	//
	virtual const char* getDeviceId() = 0;

	//
	// 获取设备类别标识
	//
	virtual const char* getDeviceClass() = 0;

	//
	//     如果此逻辑设备并非独享一个屋里设备，则isBusy返回
	// 该屋里设备是否正在被其他逻辑设备独占访问。
	//     例如对于读卡器而言，如果两个逻辑读卡器共享一个物理
	// 读卡器，则如果有另外类型的逻辑设备正在独占访问物理读卡器
	// （例如有卡在卡槽中），则isBusy函数返回false
	//
	virtual bool isBusy() = 0;

	// 
	// 获取设备状态
	// 
	virtual DeviceStatus getDeviceStatus() = 0;

	// virtual bool getAppStatus();

	//
	//    取消某一异步操作。异步操作调用时，大部分会返回该异步调用的
	// ID号，cancel函数将取消指定ID号的异步操作。
	//    如果nReqID为0，则表示取消掉目前正在排队的所有异步操作。
	//
	virtual void cancel(int nReqID = 0) = 0;

	//
	// 设备操作预留接口，与设备进行自定义交易
	//
	// id: 交易标识号
	// parameter：交易相关输入参数
	// pReqID   ：输出参数，为此异步调用的标识号
	virtual int transaction(const char* tranID, const void* parameter, int* pReqID = NULL) = 0;

	// 获取最后一个错误代码
	virtual int getLastError() = 0;

	// 获取最后一个错误的描述
	// 禁止返回
	virtual const char* getLastErrorDescription() = 0;

	//
	// 获取厂商信息
	// 此版本为 v0.2 接口，需要升级的厂商包括：
	// 长城：重庆离行终端

#if !defined(DEVICE_INTERFACE_VERSION_ANDROID)
	virtual bool getVendorInfo(std::string& key, std::string& val)
	{
		return false;
	}
#else
	virtual const int getVendorInfo(const char* key, char* val, unsigned int len)
	{
		return 0;
	}
#endif

public:
	static const char* ErrorCode2String(DeviceErrorCode code)
	{
		switch (code)
		{
		case DEVICE_ERROR_SUCCESS:
			return "DEVICE_ERROR_SUCCESS";
		case DEVICE_ERROR_CANCELD:
			return "DEVICE_ERROR_CANCELD";
		case DEVICE_ERROR_HARDWARE_ERROR:
			return "DEVICE_ERROR_HARDWARE_ERROR";
		case DEVICE_ERROR_INTERNAL_ERROR:
			return "DEVICE_ERROR_INTERNAL_ERROR";
		case DEVICE_ERROR_NOT_EXISTED:
			return "DEVICE_ERROR_NOT_EXISTED";
		default:
			break;
		}

		return "";
	}
};

NS_TDS_DEVICE_END

using namespace nsTDS::nsDevice;

#endif
