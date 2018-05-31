#pragma once
class Log
{
public:
	Log();
	~Log();
	static void i(char * classname,char *msg, ...);
private:
	static void starGetTime(char *pszTime);

};

