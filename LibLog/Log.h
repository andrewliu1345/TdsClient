#pragma once
class Log
{
public:
	static void i(const char * classname, const char * msg, ...);
private:
	static void starGetTime(char *pszTime);
	static void starGetDay(char *pszDay);
	static int createDirectory(char * path);
	static HANDLE g_hMutex;//������
};

