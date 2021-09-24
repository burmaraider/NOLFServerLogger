#include "stdafx.h"
#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
	_fileHandle.close();
}

void Logger::LogMessageToFile(std::string szMessage)
{
	if (bServerNamePresent)
	{
		this->_fileHandle.open(_szFileName, std::ios::app);
		if (!bServerInitLogged)
		{
			bServerInitLogged = true;
			this->_fileHandle << _szServerInitialize;
		}
		std::string szTime;
		szTime.append(GetCurrentRealTime());
		this->_fileHandle << szTime << szMessage << "\n";
		this->_fileHandle.close();
		return;
	}

	//servername not present yet, lets just store all this crap
	_szServerInitialize.append(GetCurrentRealTime());
	_szServerInitialize.append(szMessage + "\n");
}

void Logger::SetLogFile(std::string szFileName)
{
	_szFileName = szFileName;

	std::ifstream f(_szFileName.c_str());
	bool bExists = f.good();

	if(!bExists)
		this->_fileHandle.open(szFileName);
	else
		this->_fileHandle.open(szFileName, std::ios::app);
	
	this->_fileHandle.close();
}

void Logger::SetLogFile()
{
	std::string szFileName;
	std::string szFilePath = this->GetExePath();
	szFileName.append(szFilePath);
	szFileName.append("\\ServerLog\\");

	//Create directory or not, it doesnt care, if it exists it wont try to create again, no need to catch it.
	CreateDirectory(szFileName.c_str(), NULL);

	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%d-%m-%Y", timeinfo);
	std::string str(buffer);

	szFileName.append(str);
	szFileName.append(" - ");
	szFileName.append(GetServerName());
	szFileName.append(" - Server Log.txt");

	this->SetLogFile(szFileName);
}

std::time_t Logger::CurrentTime()
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

bool Logger::LogFileExists(std::string& szFileName)
{
	std::ifstream f(szFileName.c_str());
	return f.good();
}

std::string Logger::GetExeFileName()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::string(buffer);
}

std::string Logger::GetExeFileNameNoPath()
{
	std::string f = GetExeFileName();
	int marker = f.find_last_of("\\/");
	return f.substr(marker+1, f.length() - marker);
}

void Logger::SetServerName(char* szInput)
{
	std::string::iterator it;

	std::string s = szInput;

	std::string illegalChars = "\\/:?\"<>!|";
		for (it = s.begin(); it < s.end(); ++it) {
			bool found = illegalChars.find(*it) != std::string::npos;
			if (found) {
				*it = ' ';
			}
		}
	_szServerName = s;

	bServerNamePresent = true;
	SetLogFile();
}

std::string Logger::GetServerName()
{
	return _szServerName;
}

std::string Logger::GetExePath()
{
	std::string f = GetExeFileName();
	return f.substr(0, f.find_last_of("\\/"));
}

std::string Logger::GetCurrentRealTime()
{
	std::string szTime;
	time_t currentTime = CurrentTime();
	szTime.append(std::ctime(&currentTime));
	szTime = szTime.erase(szTime.find('\n'));
	szTime.append(" - ");
	return szTime;
}
