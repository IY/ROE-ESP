#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <list>
#include "d3d9.h"
#include "d3dx9.h"
#include "hDirectX.h"
#include "cMenu.h"
#include "hMemory.h"


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int Width = 800;
int Height = 600;

//const MARGINS Margin = { 0, 0, Width, Height };

char lWindowName[256] = "Squarew00t ROE ESP";
HWND hWnd;

char tWindowName[256] = "Ring of Elysium"; /* tWindowName ? Target Window Name */
HWND tWnd;
RECT tSize;

DWORD QSO = 3;//0x2F4C9C0;
DWORD ELISTO = 4;//0x2CB3D64;

MSG Message;
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		Render();
		break;

	case WM_CREATE:
		//DwmExtendFrameIntoClientArea(hWnd, &Margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

HANDLE hProcess;
DWORD readDword(DWORD address){
    DWORD r;
    ReadProcessMemory(hProcess, (void*)address, &r, sizeof(DWORD), 0);
    return r;
}

WORD readWord(DWORD address){
    WORD r;
    ReadProcessMemory(hProcess, (void*)address, &r, sizeof(WORD), 0);
    return r;
}

D3DXVECTOR3 readVector(DWORD address){
    D3DXVECTOR3 r;
    ReadProcessMemory(hProcess, (void*)address, &r, sizeof(D3DXVECTOR3), 0);
    return r;
}

void writeDword(DWORD address, DWORD v){
    WriteProcessMemory(hProcess, (void*)address, (char*)&v, sizeof(DWORD), 0);
}

float readFloat(DWORD address){
    float r;
    ReadProcessMemory(hProcess, (void*)address, &r, sizeof(DWORD), 0);
    return r;
}

D3DMATRIX* readMatrix(DWORD address){
    D3DMATRIX* r = (D3DMATRIX*)calloc(1, sizeof(D3DMATRIX));
    ReadProcessMemory(hProcess, (void*)address, r, 8*8, 0);
    return r;
}

char* readArray(DWORD address, int n){
    char* r = (char*)calloc(1, n);
    ReadProcessMemory(hProcess, (void*)address, r, n, 0);
    return r;
}

bool WorldToScreen(D3DXVECTOR3 *posIn, D3DXVECTOR3 *posOut, D3DMATRIX *viewMatrix)
        {
            D3DXVECTOR3 translationVector = {viewMatrix->_41, viewMatrix->_42, viewMatrix->_43};
            D3DXVECTOR3 up = {viewMatrix->_21, viewMatrix->_22, viewMatrix->_23};
            D3DXVECTOR3 right = {viewMatrix->_11, viewMatrix->_12, viewMatrix->_13};

            float w = D3DXVec3Dot(&translationVector, posIn) + viewMatrix->_44;

            if (w < 0.098f)
                return false;

            float y = D3DXVec3Dot(&up, posIn) + viewMatrix->_24;
            float x = D3DXVec3Dot(&right, posIn) + viewMatrix->_14;

            posOut->x = (Width / 2) * (1.0f + x / w);
            posOut->y = (Height / 2) * (1.0f - y / w);

            return true;
        }

float localX = 0;
float localY = 0;
float localZ = 0;

float distance3d(float x1, float y1, float z1, float x2, float y2, float z2){
    float x_diff = x1 - x2;
    float y_diff = y1 - y2;
    float z_diff = z1 - z2;

    float difftotal = x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
    return sqrt(difftotal);
}

int playersDrawn = 0;
void drawCheat(){
    DWORD QSSceneCameraH = readDword(QSO);
    DWORD QSSceneCamera = readDword(QSSceneCameraH + 0x24);
    DWORD pViewMatrix = readDword(QSSceneCamera + 0xF0);
    D3DMATRIX* viewMatrix = readMatrix(pViewMatrix + 0x08);

    DWORD EntityList = readDword(ELISTO);
        int maxEntities = readDword(ELISTO+4);
        int nEntities = readDword(ELISTO+8);
        playersDrawn = 0;

        for(int i = 0; i < maxEntities; i++){
            DWORD entity = readDword(EntityList + i*sizeof(DWORD));
            DWORD pEntity = readDword(entity + 0x8);
            DWORD pName = readDword(pEntity - 0xC);
            char *entityName = readArray(pName, 64);
            if(caresp && !strcmp(entityName, "VehicleUnit")){
                DWORD player = readDword(pEntity + 0x20);
                char *playerName = readArray(readDword(player + 0xEC), 64);
                float x = (float)readFloat(player + 0x3C0);
                float y = (float)readFloat(player + 0x3C4);
                float z = (float)readFloat(player + 0x3C8);

                D3DXVECTOR3 pos;
                D3DXVECTOR3 posIn = {x, y, z};
                bool w = WorldToScreen(&posIn, &pos, viewMatrix );

                if(w){
                    float dist = distance3d(localX, localY, localZ, x, y ,z);
                    dist /= 100;
                    if(dist < 350){
                        char print[512];
                        sprintf(print, "car[%dm]", distesp ? (int)dist : 0);
                        DrawShadowStringCentered(print, pos.x, pos.y, 232, 188, 122, 1, 1, 1, pFontSmall);
                    }

                }
            }
            if(dropesp && !strcmp(entityName, "AirDropEntity")){
                DWORD player = readDword(pEntity + 0x20);
                char *playerName = readArray(readDword(player + 0xEC), 64);
                float x = (float)readFloat(player + 0x3C0);
                float y = (float)readFloat(player + 0x3C4);
                float z = (float)readFloat(player + 0x3C8);

                D3DXVECTOR3 pos;
                D3DXVECTOR3 posIn = {x, y, z};
                bool w = WorldToScreen(&posIn, &pos, viewMatrix );

                if(w){
                    float dist = distance3d(localX/100, localY/100, localZ/100, x/100, y/100 ,z/100);
                    char print[512];
                    sprintf(print, "DROP[%dm]", distesp ? (int)dist : 0);
                    DrawShadowStringCentered(print, pos.x, pos.y, 255, 12, 12, 255, 255, 2525, pFontSmall);

                }
            }

            if((nameesp || boxesp) && (!strcmp(entityName, "ManOther") || !strcmp(entityName, "ManPlayer"))){
                DWORD player = readDword(pEntity + 0x20);
                char *playerName = readArray(readDword(player + 0xEC), 64);
                float x = (float)readFloat(player + 0x3C0);
                float y = (float)readFloat(player + 0x3C4);
                float z = (float)readFloat(player + 0x3C8);

                D3DXVECTOR3 pos;
                D3DXVECTOR3 posIn = {x, y, z};
                bool w = WorldToScreen(&posIn, &pos, viewMatrix );

                playersDrawn++;
                if(w){
                    D3DXVECTOR3 posTop;
                    D3DXVECTOR3 posInTop = {x, y, z + 180};
                    bool w = WorldToScreen(&posInTop, &posTop, viewMatrix );

                    float screenHeight = fabs(pos.y - posTop.y);
                    float screenWidth = 0.45 * screenHeight;//fabs(pos.x - posLeft.x);

                    float dist = distance3d(localX, localY, localZ, x, y ,z);
                    dist /= 100;

                    char print[512];
                    sprintf(print, "%s[%dm]", playerName, distesp ? (int)dist : 0);
                    DrawShadowStringCentered(print, pos.x, pos.y, 145, 232, 188, 1, 1, 1, pFontSmall);
                    DrawBox(pos.x - screenWidth / 2, posTop.y, screenWidth, screenHeight ,1,255, 25, 25, 255);
                }

                free(playerName);
            }

            free(entityName);
            }
    char dbg[256];
        sprintf(dbg, "mEntities: %d pDrawn %d", maxEntities, playersDrawn);
        DrawShadowStringCentered(dbg, Width / 2, 25, 145, 232, 188, 1, 1, 1, pFontSmall);
}

void updateCheat(){
    while(true){
        DWORD QSSceneCameraH = readDword(QSO);
        DWORD QSSceneCamera = readDword(QSSceneCameraH + 0x24);
        D3DXVECTOR3 localPos = readVector(QSSceneCamera + 0x114);
        localX = localPos.x;
        localY = localPos.y;
        localZ = localPos.z;

        Sleep(100);
    }
}

DWORD getMainProcedure(){
    DWORD mpEnrypted = readDword(0x2F23648);
    DWORD shifted = mpEnrypted >> 8;
    unsigned char al = static_cast<unsigned char>(mpEnrypted);
    unsigned char cl = static_cast<unsigned char>(shifted);
    DWORD ecx = readDword(cl*4+0x02F230B0);
    DWORD edx = readDword(al*4+0x02F22CB0);
    DWORD eax = ecx+edx+0x1BCB;
    edx = edx ^ edx;
    edx = eax % 0x66;
    eax = readWord(0x02F2364C);
    eax = eax ^ readDword(0x2F234B0+edx*4);
    return eax;
}

void initHack()
{
    DWORD pid = FindProcessId("Europa_Client.exe");
    if(pid == 0)
        printf("process Europa_Client.exe not found", pid);
    DWORD base = GetModuleBase(pid, "Europa_Client.exe");
    hProcess = GetProcessByName("Europa_Client.exe");

    DWORD modulesize = GetModuleSize(pid, "Europa_Client.exe");
    DWORD elist = ScanPatternEx(hProcess, base, modulesize, (BYTE*)"\x8B\x15\x00\x00\x00\x00\x3B\x04\x8A\x74\x00\x8B\x48\x08\x85\xC9", "xx????xxxx?xxxxx", 0);
    ELISTO = readDword(elist + 0x2);


    DWORD camera = ScanPatternEx(hProcess, base, modulesize, (BYTE*)"\x8B\x0D\x00\x00\x00\x00\x8B\x11\xF3\x0F\x00\x00\x00\x00\x8D\xB3\x00\x00\x00\x00", "xx????xxxx????xx??xx", 0);
    QSO = readDword(camera + 0x2);

    //DWORD mainProcedure = getMainProcedure();
    //printf("MainProcedure: %x\n", mainProcedure);

    DWORD QSSceneCameraH = readDword(QSO);
    DWORD QSSceneCamera = readDword(QSSceneCameraH + 0x24);
    DWORD pViewMatrix = readDword(QSSceneCamera + 0xF0);
    D3DMATRIX* viewMatrix = readMatrix(pViewMatrix + 0x08);
    printf("pViewMatrix: %x\n", viewMatrix);
    printf("viewMatrix: \n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n", viewMatrix->_11, viewMatrix->_12, viewMatrix->_13, viewMatrix->_14
           , viewMatrix->_21, viewMatrix->_22, viewMatrix->_23, viewMatrix->_24
           , viewMatrix->_31, viewMatrix->_32, viewMatrix->_33, viewMatrix->_34
           , viewMatrix->_41, viewMatrix->_42, viewMatrix->_43, viewMatrix->_44);

           /*
    DWORD ManMng = readDword(mainProcedure + 0x13A4);
    DWORD ManArray = readDword(ManMng + 0x10C);
    printf("%x \n", ManArray);
    DWORD entity;
    int i = 0;
    do{
        entity = readDword(ManArray + i * 4);
        printf("Entity: %x\n", entity);
        i++;
    }
    while(entity != NULL);*/
    /*
    printf("ManMng: %x\n", ManMng);
    int ManMax = readDword(ManMng + 0x124);
    printf("ManMax: %d\n", ManMax);
    DWORD ManArray = readDword(ManMng + 0x120);
    for(int i = 0; i < ManMax; i++){
        DWORD entity = readDword(ManArray + i * 4);
        if(entity){
            DWORD pEntity = readDword(entity + 0x8);
            DWORD player = readDword(pEntity + 0x30);

            char *playerName = readArray(readDword(player + 0xEC), 64);
            printf("player: %s\n", playerName);
            free(playerName);
        }
    }
    */
    /*printf("%d entities  %d entitiesMax %x EntityList",nEntities, maxEntities, EntityList);
    for(int i = 0; i < maxEntities; i++){

    }*/
    //DWORD64 add1 = FindPatternEx(hProcess, (BYTE*)"\x48\x8B\x0D\x00", "xxxx", base, 0x0F000000);
    //printf("maxEntities: %d, nEntities: %d", maxEntities, nEntities);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hSecInstance, LPSTR nCmdLine, INT nCmdShow)
{
    initMenu();
    initHack();

    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UpdateThread, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)updateCheat, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)updateMenu, 0, 0, 0);

    WNDCLASSEX wClass;
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = WinProc;
	wClass.lpszClassName = lWindowName;
	wClass.lpszMenuName = lWindowName;
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	if(!RegisterClassEx(&wClass))
		exit(1);

	tWnd = FindWindow(0, tWindowName);
	if (tWnd)
	{
		GetWindowRect(tWnd, &tSize);
		Width = tSize.right - tSize.left;
		Height = tSize.bottom - tSize.top;
		hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, lWindowName, lWindowName,  WS_POPUP, 1, 1, Width, Height, 0, 0, 0, 0);
		SetLayeredWindowAttributes(hWnd, 0, 1.0f, LWA_ALPHA);
		SetLayeredWindowAttributes(hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
		ShowWindow( hWnd, SW_SHOW);
	}

	DirectXInit(hWnd);

	for (;;)
	{
		if(PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message);
			TranslateMessage(&Message);
		}
		Sleep(1);
	}
    return 0;
}

void UpdateThread()
{
	while(true)
	{
		tWnd = FindWindow(0, tWindowName);
		if (tWnd)
		{
			GetWindowRect(tWnd, &tSize);
			Width = tSize.right - tSize.left;
			Height = tSize.bottom - tSize.top;
			DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
			if(dwStyle & WS_BORDER)
			{
				tSize.top += 23;
				Height -= 23;
			}
			MoveWindow(hWnd, tSize.left, tSize.top, Width, Height, true);
		}
		else
		{
			char ErrorMsg[125];
			sprintf(ErrorMsg, "Make sure %s is running!", tWindowName);
			MessageBox(0, ErrorMsg, "Error - Cannot find the game!", MB_OK | MB_ICONERROR);
			exit(1);
		}
		Sleep(100);
	}
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
