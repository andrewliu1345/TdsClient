#include "stdafx.h"
#include "Utility.h"



Utility::Utility()
{

}


Utility::~Utility()
{
}

int Utility::toPackData(unsigned char *cmd, unsigned char fun, unsigned char * _Dst, int _size, int *outlen, int count, ...)
{
	unsigned char tmp[38862] = { 0 };
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
		memcpy(&tmp[index], p.ParamData, p.ParamLen);// n 参数
		datalen += (p.ParamLen + 2);
		index += p.ParamLen;
	}
	va_end(params);



	index = 0;
	//unsigned char *sendBuffer = new unsigned char[len + datalen];
	memset(_Dst, 0, _size);
	_Dst[index] = 0x02;
	index++;
	intToByteArray(datalen, &_Dst[index]);//数据长度
	index += 2;
	memcpy(&_Dst[index], tmp, datalen);
	index += datalen;
	_Dst[index++] = cr_bcc(tmp, datalen);
	_Dst[index++] = 0x03;
	*outlen = index;
	return 0;
}

//************************************
// Method:    解包
// FullName:  Utility::unPackData
// Access:    public static 
// Returns:   int
// Qualifier:
// Parameter: unsigned char * buffer
// Parameter: int mun
// Parameter: PARAMLIST * params
//************************************
int Utility::unPackData(unsigned char * buffer, int mun, PARAMLIST *params)
{
	int len = 0;
	unsigned char blen[2] = { 0 };
	memcpy(blen, &buffer[1], 2);
	len = ByteArrayToInt(blen);
	int count = 0;
	int tag1 = buffer[6];
	int tag2 = buffer[7];
	if (tag1 != 0 || tag2 != 0)
	{
		return -1;
	}
	int index = 8;
	if (len > 5)
	{
		unsigned char b_crc = buffer[len + 3];
		unsigned char* tmp = new unsigned char[len];
		memset(tmp, 0, len);
		memcpy(tmp, &buffer[3], len);
		unsigned char c_crc = Utility::cr_bcc(tmp, len);
		if (b_crc != c_crc)
		{
			return -2;//校验出错
		}
		for (size_t i = 0; i < mun; i++)
		{
			sParam _param;
			int lenx = 0;
			unsigned char blenx[2] = { 0 };
			memcpy(blenx, &buffer[index], 2);
			lenx = ByteArrayToInt(blenx);
			index += 2;

			_param.ParamData = new unsigned char[lenx];
			memset(_param.ParamData, 0, lenx);
			memcpy(_param.ParamData, &buffer[index], lenx);

			index += lenx;
			_param.ParamLen = lenx;
			params->push_back(_param);
			count++;
		}
		return 0;
	}
	else
	{
		return -1;//数据长度不对
	}


}

void Utility::intToByteArray(int a, unsigned char *b)
{
	unsigned char uc[2] = {
		(unsigned char)((a >> 8) & 0xFF),
		(unsigned char)(a & 0xFF)
	};
	memcpy(b, uc, 2);
}

int Utility::ByteArrayToInt(unsigned char * b)
{
	int a;
	int h = (b[0] & 0xFF) << 8;
	int l = b[1] & 0xFF;
	a = h + l;
	return a;
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
		return -12;

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

	return iLen;
}
int Utility::UnicodeToUtf8(const wchar_t * unicode, unsigned char ** szUtf8)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	*szUtf8 = new BYTE[++len];
	memset(*szUtf8, 0, len);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, (char *)*szUtf8, len, NULL, NULL);
	return len;
}
std::string Utility::bytesToHexstring(unsigned char *bytes, int bytelength) {
	std::string hexstr;
	for (int i = 0; i < bytelength; i++) {
		char hex1;
		char hex2;
		int value = bytes[i];
		int v1 = value / 16;
		int v2 = value % 16;
		if (v1 >= 0 && v1 <= 9)
			hex1 = (char)(48 + v1);
		else
			hex1 = (char)(55 + v1);
		if (v2 >= 0 && v2 <= 9)
			hex2 = (char)(48 + v2);
		else
			hex2 = (char)(55 + v2);

		hexstr = hexstr + hex1 + hex2 + " ";
	}
	return hexstr;
}

int Utility::HexstringToBytes(const void * hexstr, unsigned char * bytes, int length)
{
	return 0;
}

void Utility::dleteAllMark(string &s, const string &mark)
{
	size_t hSize = mark.size();
	while (true)
	{
		size_t pos = s.find(mark);
		if (pos == string::npos)
		{
			return;
		}
		s.erase(pos, hSize);
	}
}
