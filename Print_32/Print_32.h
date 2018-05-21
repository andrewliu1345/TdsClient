#pragma once

/************************************************************************/
/* ��ӡ�������ӿ�                                                       */
/************************************************************************/


//************************************
// Method:    device_open
// FullName:  ���豸
// Access:    public 
// Returns:   HANDLE __stdcall ���
// Qualifier: �򿪴�ӡ�豸
// Parameter: unsigned char * port �˿ں�
// Parameter: unsigned long baud ������
//************************************
HANDLE __stdcall device_open(unsigned int port, unsigned long baud);

//************************************
// Method:    device_close
// FullName:  �ر��豸
// Access:    public 
// Returns:   __int16 __stdcall 
// Qualifier:  �رմ�ӡ���˿�
// Parameter: HANDLE icdev
//************************************
__int16 __stdcall device_close(HANDLE icdev);


//************************************
// Method:    Print_CHS
// FullName:  ��ӡ�ַ���
// Access:    public 
// Returns:   __int16 __stdcall
// Qualifier:
// Parameter: HANDLE icdev ���
// Parameter: unsigned char * str ��Ҫ��ӡ���ַ���
// Parameter: unsigned int length ����
//************************************
__int16 __stdcall Print_CHS(HANDLE icdev, unsigned char * str, unsigned int length);



//************************************
// Method:    Print
// FullName:  ���ʹ�ӡָ��
// Access:    public 
// Returns:   __int16 __stdcall
// Qualifier:
// Parameter: HANDLE icdev
// Parameter: unsigned char * str
// Parameter: unsigned int length ����
//************************************
__int16 __stdcall Print(HANDLE icdev, unsigned char * str, unsigned int length);
