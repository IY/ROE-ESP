#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <WinBase.h>

DWORD FindProcessId(const char* processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!strcmp(processName, processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while ( Process32Next(processesSnapshot, &processInfo) )
	{
	    if( !strcmp(processName, processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

#define TH32CS_SNAPMODULE 0x8
#define TH32CS_SNAPMODULE32 0x10
DWORD GetModuleBase(DWORD processId, char *szModuleName)
{
    DWORD moduleBase = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if(hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32);
        if(Module32First(hSnapshot, &moduleEntry)) {
            do {
            printf(moduleEntry.szModule);
            if(strcmp(moduleEntry.szModule, szModuleName) == 0) {
                moduleBase = (DWORD)moduleEntry.modBaseAddr;
                break;
            }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }
        CloseHandle(hSnapshot);
    }
    return moduleBase;
}

BOOL DataCompare(BYTE* pData, BYTE* bMask, char * szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
	if (*szMask == 'x' && *pData != *bMask)
		return FALSE;

	return (*szMask == NULL);
}

DWORD64 FindPatternEx(HANDLE hProcess, BYTE *bMask, char *szMask, DWORD64 dwAddress, DWORD64 dwLength)
{
	DWORD64 dwReturn = 0;
	DWORD64 dwDataLength = strlen(szMask);
	BYTE *pData = new BYTE[dwDataLength + 1];
	SIZE_T dwRead;

	for (DWORD64 i = 0; i < dwLength; i++)
	{
		DWORD64 dwCurAddr = dwAddress + i;
		bool bSuccess;
		bSuccess  = ReadProcessMemory(hProcess, (LPCVOID)dwCurAddr, pData, dwDataLength, &dwRead);

		if (!bSuccess || dwRead == 0)
		{
			continue;
		}

		if (DataCompare(pData, bMask, szMask))
		{
			dwReturn = dwAddress + i;
			break;
		}
	}

	delete[] pData;
	return dwReturn;
}

HANDLE GetProcessByName(char* name)
{
    DWORD pid = 0;

    // Create toolhelp snapshot.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    // Walkthrough all processes.
    if (Process32First(snapshot, &process))
    {
        do
        {
            // Compare process.szExeFile based on format of name, i.e., trim file path
            // trim .exe if necessary, etc.
            if (strcmp(process.szExeFile, name) == 0)
            {
               pid = process.th32ProcessID;
               break;
            }
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    if (pid != 0)
    {
         return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

    // Not found


       return NULL;
}

DWORD GetModuleSize(DWORD processID, char* module)
{
    HANDLE hSnap;
    MODULEENTRY32 xModule;
    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
    xModule.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnap, &xModule)) {
        while (Module32Next(hSnap, &xModule)) {
            if (!strncmp((char*)xModule.szModule, module, 8)) {
                CloseHandle(hSnap);
                return (DWORD)xModule.modBaseSize;
            }
        }
    }
    CloseHandle(hSnap);
    return 0;
}

DWORD ScanPatternEx(HANDLE hProc, DWORD base, DWORD len, BYTE* sig, char* mask, int offset = 0)
{
	BYTE* buf = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (ReadProcessMemory(hProc, (LPCVOID)base, buf, len, NULL) == false)
	{
		char buf2[64];	//memoria
		return NULL;
	}
	for (int i = 0; i <= (len - strlen(mask))+1; i++)
	{
		if ((buf[i] == sig[0] && mask[0] == 'x') || (mask[0] == '?'))
		{
			for (int x = 0;; x++)
			{
				if (mask[x] == 'x')
				{
					if (buf[i + x] == sig[x])
						continue;
					else
						break;
				}
				else if (mask[x] == 0x00)
				{
					return (DWORD)(base + i + offset);
				}
			}
		}
	}
	return NULL;
}
