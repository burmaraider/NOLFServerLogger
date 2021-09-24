#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>

class Logger
{
public:
	Logger();
	~Logger();
	void LogMessageToFile(std::string szMessage);
	void SetLogFile(std::string szFileName);
	void SetLogFile();
	std::time_t CurrentTime();
	std::string GetExeFileNameNoPath();
	void SetServerName(char* szInput);
	std::string GetServerName();
private:
	std::string _szFileName;
	std::string _szServerName;
	std::ofstream _fileHandle;
	bool LogFileExists(std::string& szFileName);
	std::string GetExeFileName();
	std::string GetExePath();
	std::string GetCurrentRealTime();
	std::string _szServerInitialize;
	bool bServerNamePresent = false;
	bool bServerInitLogged = false;
};
#endif