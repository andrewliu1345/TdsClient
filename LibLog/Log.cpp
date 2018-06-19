#include "stdafx.h"
#include "Log.h"
#include <iostream>
#include <io.h>
#include <direct.h>


HANDLE Log::g_hMutex = CreateMutex(NULL, FALSE, NULL);

void Log::i(const char * classname, const char * msg, ...)
{

	FILE *fp;
	va_list argp;
	char szTime[32] = { 0 };
	char szDay[32] = { 0 };
	starGetTime(szTime);
	starGetDay(szDay);
	char filepath[256] = { 0 };
	sprintf_s(filepath, "c:\\mlog\\%s.log", szDay);
	//CString filepath=CString.Format("c:\\mlog\\%s", szTime)
	if (createDirectory(filepath) != 0)
	{
		return;
	}
	WaitForSingleObject(g_hMutex, INFINITE);
	fopen_s(&fp, filepath, "a+");
	fprintf(fp, "[%s]", szTime);
	fprintf(fp, "[%s]", classname);
	va_start(argp, msg);
	vfprintf(fp, msg, argp);
	va_end(argp);
	fprintf(fp, "\n");
	fclose(fp);
	ReleaseMutex(g_hMutex);
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
void Log::starGetDay(char *pszDay)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf_s(pszDay, 32, "%04d%02d%02d", sys.wYear,
		sys.wMonth, sys.wDay);
	//pszDay[17] = 0x00;
}

int Log::createDirectory(char * fileName)
{
	
	char *tag;
	for (tag = fileName; *tag; tag++)
	{
		if (*tag == '\\')
		{
			char buf[1000], path[1000];
			strcpy_s(buf, fileName);
			buf[strlen(fileName) - strlen(tag) + 1] = NULL;
			strcpy_s(path, buf);
			if (_access(path, 6) == -1)
			{
				_mkdir(path);
				
			}

		}
	}
	return 0;

}

