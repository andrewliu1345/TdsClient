#include "stdafx.h"
#include "Utility.h"

#include <iostream>


typedef list<sParam> PARAMLIST;

Utility::Utility()
{
}


Utility::~Utility()
{
}

unsigned char * Utility::toPackData(unsigned char *cmd, unsigned char fun, int count, ...)
{
	unsigned char tmp[4096000] = { 0 };
	int len = 3;//总长度
	int datalen = 0;//数据长度
	int index = 0;//游标
	//PARAMLIST paramarry;
	//PARAMLIST::iterator  param;
	va_list params;
	va_start(params, count);
	memcpy(&tmp[index], cmd, 2);//模块
	datalen += 2;
	index += 2;
	tmp[index] = fun;//功能
	datalen++;
	index++;
	for (size_t i = 0; i < count; i++)
	{
		sParam p = va_arg(params, sParam);

		intToByteArray(p.ParamLen, &tmp[index]);//n 参数长度
		index += 2;
		memcpy(&tmp[index], p.ParamData,  p.ParamLen);// n 参数
		datalen += (p.ParamLen + 2);
		index += p.ParamLen;
	}
	va_end(params);



	index = 0;
	unsigned char *sendBuffer = new unsigned char[len + datalen];
	memset(sendBuffer, 0, len);
	sendBuffer[index] = 0x02;
	index++;
	intToByteArray(datalen, &sendBuffer[index]);//数据长度
	index += 2;
	memcpy(&sendBuffer[index], tmp,  datalen);
	index += datalen;
	sendBuffer[index++] = cr_bcc(tmp, datalen);
	sendBuffer[index] = 0x03;
	return sendBuffer;
}

void Utility::intToByteArray(int a, unsigned char *b)
{
	unsigned char uc[2] = {
		(unsigned char)((a >> 8) & 0xFF),
		(unsigned char)(a & 0xFF)
	};
	memcpy(b, uc, 2);
}

unsigned char Utility::cr_bcc(unsigned char * data, int datalen)
{
	unsigned char crc = 0x00;
	for (size_t i = 0; i < datalen; i++)
	{
		crc ^= data[i];
	}
	return crc;
}

int Utility::GB18030ToUTF8(char * pStrGB18030, unsigned char ** pStrUtf8)
{
	int iLen, iLenUnicode;
	unsigned char *pUnicodeBuf;

	if ((pStrGB18030 == NULL) || (pStrUtf8 == NULL))
		return 12;

	//Convert string from GB18030 to Unicode
	iLen = strlen(pStrGB18030);
	iLenUnicode = MultiByteToWideChar(54936, 0, pStrGB18030, -1, NULL, 0);
	pUnicodeBuf = new BYTE[iLenUnicode * sizeof(wchar_t)];
	memset(pUnicodeBuf, 0, iLenUnicode * sizeof(wchar_t));
	MultiByteToWideChar(54936, 0, pStrGB18030, iLen, (wchar_t *)pUnicodeBuf, iLenUnicode * sizeof(wchar_t));

	//Convert string from Unicode to UTF8
	iLen = WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)pUnicodeBuf, -1, NULL, 0, NULL, NULL);
	*pStrUtf8 = new BYTE[iLen + 1];
	memset(*pStrUtf8, 0, iLen + 1);
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)pUnicodeBuf, iLenUnicode * sizeof(wchar_t), (char *)*pStrUtf8, iLen, NULL, NULL);

	delete[] pUnicodeBuf;

	return 0;
}

