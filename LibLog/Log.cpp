#include "stdafx.h"
#include "Log.h"
#include <iostream>





void Log::i(char * classname, char * msg, ...)
{
	FILE *fp;
	va_list argp;
	char szTime[32] = { 0 };

	starGetTime(szTime);

	fopen_s(&fp, "c:\\mlog\\starcardreader.log", "a+");
	fprintf(fp, "[%s]", szTime);
	fprintf(fp, "[%s]", classname);
	va_start(argp, msg);
	vfprintf(fp, msg, argp);
	va_end(argp);
	fclose(fp);
}

void Log::starGetTime(char * pszTime)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf_s(pszTime, 32, "%04d%02d%02d%02d%02d%02d%03d", sys.wYear,
		sys.wMonth, sys.wDay, sys.wHour, sys.wMinute,
		sys.wSecond, sys.wMilliseconds);
	pszTime[17] = 0x00;
}
