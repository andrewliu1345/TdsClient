#pragma once
class CSocketDelegete
{
public:
	CSocketDelegete();
	~CSocketDelegete();
	
	//************************************
	// Method:    数据返回回调
	// FullName:  CSocketDelegete::socketRevCallBack
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: unsigned char * buffer
	//************************************
	virtual void  socketRevCallBack(unsigned char *buffer,int length)=0;


	//************************************
	// Method:    发送时的回调
	// FullName:  CSocketDelegete::socketSendCallBack
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: unsigned char * buffer
	//************************************
	virtual void socketSendCallBack(unsigned char *buffer)=0;


	//************************************
	// Method:    连接成功的回调
	// FullName:  CSocketDelegete::socketdidConnectCallBack
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	//************************************
	virtual void socketdidConnectCallBack()=0;



	//************************************
	// Method:    断开链接的回调
	// FullName:  CSocketDelegete::socketdisConnectCallBack
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	//************************************
	virtual void socketdisConnectCallBack()=0;

	
	virtual void socketErrCallBack() = 0;
};

