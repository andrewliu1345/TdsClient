#pragma once
#include "IPinpad.h"
#include "../libNetWorkConnet/TranSocket.h"
#include "../libNetWorkConnet/CSocketDelegete.h"
//#include <atlstr.h>
#define CLASSNAME "Pinpad"

class Pinpad :
	public IPinpad, public CSocketDelegete
{
public:
	Pinpad();
	~Pinpad();

	void socketRevCallBack(unsigned char *buffer);

	void socketSendCallBack(unsigned char *buffer);

	void socketdidConnectCallBack();

	void socketdisConnectCallBack();

	void socketErrCallBack();




	void initialize(XmlParser* pConfig);

	void setDeviceEventHandler(void* pHandler);


	const char* getDeviceId();
	bool isBusy();
	DeviceStatus getDeviceStatus();
	void cancel(int nReqID);

	int transaction(const char* tranID, const void* parameter, int* pReqID = NULL);

	// 获取最后一个错误代码
	int getLastError();

	// 获取最后一个错误的描述
	// 禁止返回
	const char* getLastErrorDescription();

	// 密码键盘操作

	//
	// 导入密钥
	// keyname : 要导入的密钥名称
	// key : 要导入的密钥数据，
	// keyuse: 密钥用途
	// encKey ：导入之前用何密钥解密
	//
	int importKey(const char* keyname, const char* data, const char* keyuse, const char* encKeyName = NULL, int* pReqID = NULL);

	// 
	// 清除所有密钥，此操作为同步操作
	//
	int clearKey();

	// 加密
	int encrypt(const char* keyname, const char* algorithm, const char* dataIn, int* pReqID = NULL);

	// 解密
	int decrypt(const char* keyname, const char* algorithm, const char* dataIn, int* pReqID = NULL);

	// 开启普通键盘模式，只有cancel操作能终止此状态
	int getData(int* pReqID = NULL);

	//
	// 开启密码输入模式
	// min ： 最小输入
	// max ： 最大输入
	// 在用户按了Enter按键或者用户调用了cancel后，此状态结束
	//
	int getPin(const int min = 3, const int max = 6, int* pReqID = NULL);

	// 软件结束getPin，使其以DEVICE_SUCCESS的形式返回
	void endGetPin();

	//
	// 计算密码
	//
	int getPinblock(const char* keyname, const char* cryptData = NULL, int* pReqID = NULL);
private:
	long iReqid = 1;
	TranSocket * transoket;
	void RevReadCard(UCHAR* buffer);

	//************************************
	// Method:    下载主密钥
	// FullName:  Pinpad::LoadMKey
	// Access:    private 
	// Returns:   int
	// Qualifier:
	// Parameter: string Mkey 密钥
	// Parameter: string encIndex 密钥加密索引号
	//************************************
	int LoadMKey(string Mkey, string encIndex);

	//************************************
	// Method:    LoadWKey
	// FullName:  Pinpad::LoadWKey
	// Access:    private 
	// Returns:   int
	// Qualifier:
	// Parameter: string Wkey 密钥
	// Parameter: string keyuse 工作密钥类型“FUNCTION”:计算PINBLOCK
//“MACING”:计算MAC
//	“CRYPT” : 加解密计算
//	“KEYENCKEY” : 对密钥值解密以间接导入密钥
//	“SVENCKEY” : 在加解密时，可能需要对IV值进行解密。
//
	// Parameter: string encIndex 密钥加密索引号
	//************************************
	int LoadWKey(string Wkey, string keyuse, string encIndex);
	int ClearAllKey();
};

extern "C" IPINPAD IPinpad* _stdcall createDevice();