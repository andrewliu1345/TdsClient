#pragma once
class CSocketDelegete
{
public:
	CSocketDelegete();
	~CSocketDelegete();
	
	//************************************
	// Method:    ���ݷ��ػص�
	// FullName:  CSocketDelegete::socketRevCallBack
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: unsigned char * buffer
	//************************************
	virtual void  socketRevCallBack(unsigned char *buffer,int length)=0;


	//************************************
	// Method:    ����ʱ�Ļص�
	// FullName:  CSocketDelegete::socketSendCallBack
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: unsigned char * buffer
	//************************************
	virtual void socketSendCallBack(unsigned char *buffer)=0;


	//************************************
	// Method:    ���ӳɹ��Ļص�
	// FullName:  CSocketDelegete::socketdidConnectCallBack
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	//************************************
	virtual void socketdidConnectCallBack()=0;



	//************************************
	// Method:    �Ͽ����ӵĻص�
	// FullName:  CSocketDelegete::socketdisConnectCallBack
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	//************************************
	virtual void socketdisConnectCallBack()=0;

	
	virtual void socketErrCallBack() = 0;
};

