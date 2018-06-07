#pragma once
#include "stdafx.h"
#include <stdio.h> 
#include <cstdarg>
#include <string> 
#include <tchar.h>
//#include <atlstr.h> //预编译头错误
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
	// Method:    装包
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
	// Method:    结包
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
	// Method:    ByteArry2HexString
	// FullName:  Utility::bytesToHexstring
	// Access:    public static 
	// Returns:   std::string
	// Qualifier:
	// Parameter: unsigned char * bytes
	// Parameter: int bytelength
	//************************************
	static std::string bytesToHexstring(unsigned char *bytes, int bytelength);
	Utility();
	~Utility();
};

