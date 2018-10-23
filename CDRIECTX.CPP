#include "hDirectX.h"
#include "cMenu.h"
#include "hHack.h"

IDirect3D9Ex* p_Object = 0;
IDirect3DDevice9Ex* p_Device = 0;
D3DPRESENT_PARAMETERS p_Params;

ID3DXLine* p_Line;
ID3DXFont* pFontSmall = 0;

int DirectXInit(HWND hWnd)
{
	if(FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(1);

	ZeroMemory(&p_Params, sizeof(p_Params));
    p_Params.Windowed = TRUE;
    p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality   = D3DMULTISAMPLE_NONE;
    p_Params.BackBufferFormat = D3DFMT_A8R8G8B8 ;
    p_Params.BackBufferWidth = Width;
    p_Params.BackBufferHeight = Height;
    p_Params.EnableAutoDepthStencil = TRUE;
    p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if(FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		exit(1);

	if(!p_Line)
		D3DXCreateLine(p_Device, &p_Line);
		//p_Line->SetAntialias(1); *removed cuz crosshair was blurred*

	D3DXCreateFont(p_Device, 18, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Calibri", &pFontSmall);

	return 0;
}

int Render()
{
	p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	p_Device->BeginScene();

	if(tWnd == GetForegroundWindow())
	{
	    drawMenu();

		//text with shadow
		DrawShadowStringCentered("RATZ ROE ESP", Width / 2, 5, 145, 232, 188, 1, 1, 1, pFontSmall);

		drawCheat();

		//crosshair
		if(crosshair){
            FillRGB(Width / 2 - 4, Height / 2, 9, 1, 240, 240, 250, 255);
            FillRGB(Width / 2, Height / 2 - 4, 1, 8, 240, 240, 250, 255);
		}

	}

	p_Device->EndScene();
	p_Device->PresentEx(0, 0, 0, 0, 0);
	return 0;
}

