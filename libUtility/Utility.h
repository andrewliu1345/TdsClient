#pragma once
#include <stdio.h> 
#include <cstdarg>
#include <string> 
#include <tchar.h>
//#include <atlstr.h> //Ô¤±àÒëÍ·´íÎó
#include <list>

struct sParam
{
	unsigned char * ParamData;
	int ParamLen;
};
class Utility
{
public:

	static unsigned char * toPackData(unsigned char *cmd, unsigned char fun, int count, ...);
	static void intToByteArray(int a, unsigned char *b);
	static unsigned char cr_bcc(unsigned char * data, int datalen);
	static int GB18030ToUTF8(char *pStrGB18030, unsigned char **pStrUtf8);
	Utility();
	~Utility();
};

