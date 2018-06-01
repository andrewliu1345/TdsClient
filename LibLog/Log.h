#pragma once
class Log
{
public:
	static void i(char * classname, char *msg, ...);
private:
	static void starGetTime(char *pszTime);

};

