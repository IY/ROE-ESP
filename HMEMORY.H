#include <Windows.h>

DWORD FindProcessId(const char* processName);
DWORD GetModuleBase(DWORD processId, TCHAR *szModuleName);
HANDLE GetProcessByName(char* name);
DWORD64 FindPatternEx(HANDLE hProcess, BYTE *bMask, char *szMask, DWORD64 dwAddress, DWORD64 dwLength);

DWORD readDword(DWORD address);
void writeDword(DWORD address, DWORD v);
float readFloat(DWORD address);
D3DMATRIX* readMatrix(DWORD address);
char* readArray(DWORD address, int n);
DWORD GetModuleSize(DWORD processID, char* module);
DWORD ScanPatternEx(HANDLE hProc, DWORD base, DWORD len, BYTE* sig, char* mask, int offset = 0);

extern HANDLE hProcess;
