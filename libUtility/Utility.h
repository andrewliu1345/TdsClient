#pragma once
#include "stdafx.h"
#include <stdio.h> 
#include <cstdarg>
#include <string> 
#include <tchar.h>
//#include <atlstr.h> //Ԥ����ͷ����
#include <list>
#include <iostream>



struct sParam
{
	int ParamLen;
	unsigned char  *ParamData;

};
typedef list<sParam> PARAMLIST;

class Utility
{
public:

	//************************************
	// Method:    װ��
	// FullName:  Utility::toPackData
	// Access:    public static 
	// Returns:   unsigned char *
	// Qualifier:
	// Parameter: unsigned char * cmd
	// Parameter: unsigned char fun
	// Parameter: int count
	// Parameter: ...
	//************************************
	
	static int toPackData(unsigned char *cmd, unsigned char fun, unsigned char * _Dst, int _size, int *outlen, int count, ...);
	
	//************************************
	// Method:    ���
	// FullName:  Utility::unPackData
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: unsigned char * buffer
	// Parameter: int mun
	// Parameter: PARAMLIST params
	//************************************
	static int unPackData(unsigned char * buffer, int mun, PARAMLIST *params);
	//************************************
	// Method:    Int2Byte[]
	// FullName:  Utility::intToByteArray
	// Access:    public static 
	// Returns:   void
	// Qualifier:
	// Parameter: int a
	// Parameter: unsigned char * b
	//************************************
	static void intToByteArray(int a, unsigned char *b);


	//************************************
	// Method:    Byte[] 2 Int
	// FullName:  Utility::ByteArrayToInt
	// Access:    public static 
	// Returns:   void
	// Qualifier:
	// Parameter: unsigned char * b
	// Parameter: int a
	//************************************
	static int ByteArrayToInt(unsigned char *b);
	//************************************
	// Method:    cr_bcc
	// FullName:  Utility::cr_bcc
	// Access:    public static 
	// Returns:   unsigned char
	// Qualifier:
	// Parameter: unsigned char * data
	// Parameter: int datalen
	//************************************
	static unsigned char cr_bcc(unsigned char * data, int datalen);
	//************************************
	// Method:    GB18030ToUTF8
	// FullName:  Utility::GB18030ToUTF8
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: char * pStrGB18030
	// Parameter: unsigned char * * pStrUtf8
	//************************************
	static int GB18030ToUTF8(char *pStrGB18030, unsigned char **pStrUtf8);

	//************************************
	// Method:    UnicodeToUtf8
	// FullName:  Utility::UnicodeToUtf8
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: const wchar_t * unicode
	// Parameter: char * * pStrUtf8
	//************************************
	static int UnicodeToUtf8(const wchar_t* unicode, unsigned char **pStrUtf8);
	//************************************
	// Method:    ByteArry2HexString
	// FullName:  Utility::bytesToHexstring
	// Access:    public static 
	// Returns:   std::string
	// Qualifier:
	// Parameter: unsigned char * bytes
	// Parameter: int bytelength
	//************************************
	static std::string bytesToHexstring(unsigned char *bytes, int bytelength);


	//************************************
	// Method:    HexstringToBytes
	// FullName:  Utility::HexstringToBytes
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: const void * hexstr
	// Parameter: unsigned char * bytes
	// Parameter: int length
	//************************************
	static int HexstringToBytes(const void * hexstr, unsigned char * bytes, int length);

	static void dleteAllMark(string &s, const string &mark);

	
	//************************************
	// Method:    ת��д
	// FullName:  Utility::ToUpper
	// Access:    public static 
	// Returns:   void
	// Qualifier:
	// Parameter: string s
	//************************************
	static string ToUpper(string s);

	//************************************
	// Method:    תСд
	// FullName:  Utility::ToLower
	// Access:    public static 
	// Returns:   void
	// Qualifier:
	// Parameter: string s
	//************************************
	static string ToLower(string s);
	Utility();
	~Utility();
};

