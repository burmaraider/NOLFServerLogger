#pragma once
Logger myLog;
SignatureScanner sig;

intptr_t FindObjectAddress(char* szModName)
{
	module mod = sig.GetModule(szModName);
	return mod.dwBase;
}

//Hold addresses for the loaded modules
intptr_t p_addrServerApp = 0x0;
intptr_t p_addrObjectLto = 0x0;

typedef int(__cdecl* tSub_404CC0)(int, char*, char);
tSub_404CC0 oAddMessageToBox;

int __cdecl StuffMessageIntoLog(int eax, char* Format, char argList)
{
	myLog.LogMessageToFile(Format);
	return oAddMessageToBox(eax, Format, argList);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////


typedef int(__fastcall* tSub_D6DC0)(char*, char*, int, int, int);
tSub_D6DC0 oOnClientEnterWorld;

int __fastcall OnClientEnterWorld(char* ecx, char* edx, int a2, int playerNameAddress, int a4)
{
	std::string szTemp = sig.ReadStringAtMemory(playerNameAddress, a4);
	szTemp.append(" joined the game");
	myLog.LogMessageToFile(szTemp);
	return oOnClientEnterWorld(ecx, edx, a2, playerNameAddress, a4);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
typedef int(__fastcall* tSub_D72A0)(char*, char*, int);
tSub_D72A0 oOnClientExitWorld;

int __fastcall OnClientExitWorld(char* eax, char* ecx, int a1)
{
	DWORD addr = a1 + 272; //add our offset to the name or we could just add 0x1810 to get there... maybe this changes?
	int tempAddr = sig.ReadMemory<int>(addr); //read the address to this offset
	std::string szTemp = sig.ReadStringAtMemory(tempAddr, 20); //read a string at this address
	szTemp.append(" left the game");
	myLog.LogMessageToFile(szTemp);
	return oOnClientExitWorld(eax, ecx, a1);
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
typedef LPCSTR(__fastcall* tSub_06D80)(char* ecx, char* edx, char* szKey, char* szValue);
tSub_06D80 oTest;

LPCSTR __fastcall Test(char* ecx, char* edx, char* szKey, char* szValue)
{

	if (!strncmp(szKey, "NetSessionName", strlen(szKey)))
	{
		myLog.SetServerName(szValue);
		std::string szMessage = "Server Name: ";
		szMessage.append(szValue);
		myLog.LogMessageToFile(szMessage);
	}
	return oTest(ecx, edx, szKey, szValue);
}