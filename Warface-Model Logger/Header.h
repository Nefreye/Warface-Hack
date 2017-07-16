#include "Drawing.h"
#include <vector>
#include <windows.h>
#include <detours.h>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include <ctime>
#include <string>
#include <stdio.h>
#include <sstream>
#include <fstream>

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

#pragma intrinsic(_ReturnAddress)

#pragma warning (disable : 4099)
#pragma warning (disable : 4244)

using namespace std;

#define WFPlayer 0x5F8F6A
//#define WFPlayer 0x12F15BA
#define WFSmoke 0x5F4E39
//#define WFSmoke 0x12ED489
#define WFWorld 0x5F8F18
//#define WFWorld 0x12F1568
#define WFFullbright 0x10AC1EB
//#define WFFullbright 0x8F4F4B

#define WFBots (NumVertices == 32233 && primCount == 4560 && Stride == 16) || (NumVertices == 4145 && primCount == 5149 && Stride == 16) || (NumVertices == 2800 && primCount == 4324 && Stride == 16) || (NumVertices == 2900 && primCount == 4324 && Stride == 16)
#define Mines ((NumVertices == 637 && primCount == 524 && Stride == 16)|| (NumVertices == 1541 && primCount == 1220 && Stride == 16))
#define Grenade ((NumVertices == 128 && primCount == 120 && Stride == 16 ))
#define TestTargets ((NumVertices == 64 && primCount == 38 && Stride == 16) || (NumVertices == 88 || primCount == 56 && Stride == 16) || (NumVertices == 148 && primCount == 108 && Stride == 16))
#define Warface ((NumVertices == 503 && primCount == 674 && Stride == 16) && (NumVertices == 1700 && primCount == 2400 && Stride == 16))
#define Blackwood ((NumVertices == 503 && primCount == 674 && Stride == 16) && (NumVertices == 1244 && primCount == 2014 && Stride == 16))

class Hack;
class Menu;
class Aimbot;
class Visuals;
class EraseStuff;


char g_Text[128] = { " \0 " };
void* g_SelectedAddress = NULL;
int g_Index = -1;
std::vector<void*> g_Vector;


class Menu {
public:
	int spaceX = 10;
	int t = 5;
	int spaceY = 20;
	int y = 100;
	int spaceXX = 125;
	int x = 100;
	int h = 285;
	int w = 205;
	bool bMenu = false;
	bool bInput = false;
	int index = 1;

	D3DCOLOR cAimbot = cRed;
	D3DCOLOR cESP = cRed;
	D3DCOLOR cSnaplines = cRed;
	D3DCOLOR cWallhack = cRed;
	D3DCOLOR cChams = cRed;
	D3DCOLOR cExplosiveChams = cRed;
	D3DCOLOR cWeaponChams = cRed;
	D3DCOLOR cNoSmoke = cRed;
	D3DCOLOR cFullbright = cRed;
	D3DCOLOR cDarkMode = cRed;
	D3DCOLOR cMatrixMode = cRed;
	D3DCOLOR cFPS = cRed;
	D3DCOLOR cCrosshair = cRed;
	D3DCOLOR cSave = cYellow;
	D3DCOLOR cLoad = cYellow;

	char* sAimbot = "Disabled";
	char* sESP = "Disabled";
	char* sSnaplines = "Disabled";
	char* sWallhack = "Disabled";
	char* sChams = "Disabled";
	char* sExplosiveChams = "Disabled";
	char* sWeaponChams = "Disabled";
	char* sNoSmoke = "Disabled";
	char* sFullbright = "Disabled";
	char* sDarkMode = "Disabled";
	char* sMatrixMode = "Disabled";
	char* sFPS = "Disabled";
	char* sCrosshair = "Disabled";

	void CalculateMenu();
	void DrawMenu(LPDIRECT3DDEVICE9 Device);
};



class Aimbot {
public:
	DWORD frametime = timeGetTime();
	bool bAimbot = false;
	int iTeam = 3;
	int iAimbotFOV = 10;
	int iAimsens = 2;
	int iAimheight = 6;
	bool bAutoshoot = false;
	DWORD Aimkey = VK_RBUTTON;
	bool bPressed = false;
	void AddModel(LPDIRECT3DDEVICE9 Device, int iTeam);
	/*float GetDistance(float Xx, float Yy, float xX, float yY)
	{
	return sqrt((yY - Yy) * (yY - Yy) + (xX - Xx) * (xX - Xx));
	}
	struct AimInfo_t
	{
	INT       iTeam;

	//float RealDistance
	float CrosshairDistance;

	float vOutX, vOutY;
	};
	std::vector<AimInfo_t>AimInfo;
	float RealDistance;*/

	void Run();
};



class Visuals {
public:
	bool bESP = false;
	double dESPAimheight = 0.5;
	bool bSnaplines = false;
	bool bWallhack = false;
	bool bChams = false;
	bool bExplosiveChams = false;
	bool bWeaponChams = false;
	bool bNoSmoke = false;
	bool bFullbright = false;
	bool bDarkMode = false;
	bool bMatrixMode = false;
	bool bFPS = false;
	int iCrosshair = 0;

	int fFps = 0;
	float fLastTickCount = 0.0f;
	float fCurrentTickCount;
	char cFrameRate[50] = { '\0' };

	void FoV();
	void ESP(LPDIRECT3DDEVICE9 Device);
	void Crosshair(LPDIRECT3DDEVICE9 Device);
	void Chams(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
	void Wallhack(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
	void MatrixMode(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
	void DarkMode(LPDIRECT3DDEVICE9 Device);
	void WeaponChams(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
	void ExplosivesChams(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
};



class EraseStuff {
public:
	void EraseHeaders(HINSTANCE hModule);
	void ErasePortableHeader(HINSTANCE hModule);
	void HideModule(HMODULE Module);
private:
	typedef struct _CodeSpeed {
		LARGE_INTEGER s, e, f, o;
	} CodeSpeed;

	typedef struct _LDR_MODULE {
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY InInitializationOrderModuleList;
		PVOID BaseAddress;
		PVOID EntryPoint;
		ULONG SizeOfImage;
	}LDR_MODULE, *PLDR_MODULE;

	typedef struct _PEB_LDR_DATA {
		ULONG Length;
		BOOLEAN Initialized;
		PVOID SsHandle;
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY InInitializationOrderModuleList;
	}PEB_LDR_DATA, *PPEB_LDR_DATA;

	typedef struct _PEB {
		BYTE Reserved1[2];
		BYTE BeingDebugged;
		BYTE Reserved2[1];
		PVOID Reserved3[2];
		PPEB_LDR_DATA Ldr;
	}PEB, *PPEB;
};

void EraseStuff::EraseHeaders(HINSTANCE hModule)
{
	PIMAGE_DOS_HEADER pDoH;
	PIMAGE_NT_HEADERS pNtH;
	DWORD i, ersize, protect;

	if (!hModule) return;

	pDoH = (PIMAGE_DOS_HEADER)(hModule);
	pNtH = (PIMAGE_NT_HEADERS)((LONG)hModule + ((PIMAGE_DOS_HEADER)hModule)->e_lfanew);

	ersize = sizeof(IMAGE_DOS_HEADER);

	if (VirtualProtect(pDoH, ersize, PAGE_READWRITE, &protect))
	{
		for (i = 0; i < ersize; i++)
			*(BYTE*)((BYTE*)pDoH + i) = 0;
	}

	ersize = sizeof(IMAGE_NT_HEADERS);

	if (pNtH && VirtualProtect(pNtH, ersize, PAGE_READWRITE, &protect))
	{
		for (i = 0; i < ersize; i++)
			*(BYTE*)((BYTE*)pNtH + i) = 0;
	}

	return;
}

void EraseStuff::ErasePortableHeader(HINSTANCE hModule)
{
	MEMORY_BASIC_INFORMATION MemoryInfo;

	VirtualQuery((LPCVOID)hModule, &MemoryInfo, sizeof(MemoryInfo));

	VirtualProtect(MemoryInfo.BaseAddress, MemoryInfo.RegionSize, PAGE_EXECUTE_READWRITE, &MemoryInfo.Protect);
	ZeroMemory((PVOID)hModule, 4096);

	VirtualProtect(MemoryInfo.BaseAddress, MemoryInfo.RegionSize, MemoryInfo.Protect, NULL);
	FlushInstructionCache(GetCurrentProcess(), (LPCVOID)MemoryInfo.BaseAddress, MemoryInfo.RegionSize);
}

void EraseStuff::HideModule(HMODULE Module)
{
	PEB* peb;
	LDR_MODULE* ldr;

	peb = (PEB*)__readfsdword(0x30);
	ldr = (LDR_MODULE*)peb->Ldr->InLoadOrderModuleList.Flink;

	while (ldr->BaseAddress != 0)
	{
		if (ldr->BaseAddress == Module)
		{
			if (ldr->InLoadOrderModuleList.Blink != 0)
				(ldr->InLoadOrderModuleList.Blink)->Flink = ldr->InLoadOrderModuleList.Flink;
			if (ldr->InLoadOrderModuleList.Flink != 0)
				(ldr->InLoadOrderModuleList.Flink)->Blink = ldr->InLoadOrderModuleList.Blink;

			if (ldr->InInitializationOrderModuleList.Blink != 0)
				(ldr->InInitializationOrderModuleList.Blink)->Flink = ldr->InInitializationOrderModuleList.Flink;
			if (ldr->InInitializationOrderModuleList.Flink != 0)
				(ldr->InInitializationOrderModuleList.Flink)->Blink = ldr->InInitializationOrderModuleList.Blink;

			if (ldr->InMemoryOrderModuleList.Flink != 0)
				(ldr->InMemoryOrderModuleList.Blink)->Flink = ldr->InMemoryOrderModuleList.Flink;
			if (ldr->InMemoryOrderModuleList.Blink != 0)
				(ldr->InMemoryOrderModuleList.Flink)->Blink = ldr->InMemoryOrderModuleList.Blink;
		}

		ldr = (LDR_MODULE*)ldr->InLoadOrderModuleList.Flink;
	}
}





Menu cMenu;
Aimbot cAim;
Visuals cVisuals;
EraseStuff cEraseStuff;




class Hack {
public:
	UINT pSizeOfData;
	IDirect3DPixelShader9* pShader;
	HWND hwnd;
	LPDIRECT3DTEXTURE9 g_Blue = NULL, g_Green = NULL, g_Null = NULL, g_White = NULL, g_Pink = NULL, g_Red = NULL, g_Orange = NULL, g_Yellow = NULL, g_DarkGreen2 = NULL, g_DarkGreen1 = NULL;
	D3DVIEWPORT9 Viewport;
	HMODULE g_HModule = 0;
	bool g_Init = false;
	float ScreenCenterX = 0;
	float ScreenCenterY = 0;
	float ScreenX = 0;
	float ScreenY = 0;
	void SaveSettings();
	void LoadSettings();
	HRESULT GenerateTexture(LPDIRECT3DDEVICE9 pDevice, IDirect3DTexture9 **ppD3Dtex, DWORD colour32);
	DWORD FindDevice(DWORD Len)
	{
		DWORD dwObjBase = 0;

		dwObjBase = (DWORD)LoadLibrary("d3d9.dll");
		while (dwObjBase++ < dwObjBase + Len)
		{
			if ((*(WORD*)(dwObjBase + 0x00)) == 0x06C7
				&& (*(WORD*)(dwObjBase + 0x06)) == 0x8689
				&& (*(WORD*)(dwObjBase + 0x0C)) == 0x8689
				) {
				dwObjBase += 2; break;
			}
		}
		return(dwObjBase);
	}
	DWORD QuickChecksum(DWORD *pData, int size)
	{
		if (!pData) { return 0x0; }

		DWORD sum;
		DWORD tmp;
		sum = *pData;

		for (int i = 1; i < (size / 4); i++)
		{
			tmp = pData[i];
			tmp = (DWORD)(sum >> 29) + tmp;
			tmp = (DWORD)(sum >> 17) + tmp;
			sum = (DWORD)(sum << 3) ^ tmp;
		}

		return sum;
	}
	DWORD GetDeviceAddress(int VTableIndex)
	{
		PDWORD VTable;
		*(DWORD*)&VTable = *(DWORD*)FindDevice(0x128000);
		return VTable[VTableIndex];
	}
private:
	char dlldir[320];
	char* GetDirectoryFile(char *filename)
	{
		static char path[320];
		strcpy_s(path, dlldir);
		strcat_s(path, filename);
		return path;
	}
};

HRESULT Hack::GenerateTexture(LPDIRECT3DDEVICE9 pDevice, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if (FAILED(pDevice->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
		return E_FAIL;

	WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
		| (WORD)(((colour32 >> 20) & 0xF) << 8)
		| (WORD)(((colour32 >> 12) & 0xF) << 4)
		| (WORD)(((colour32 >> 4) & 0xF) << 0);

	D3DLOCKED_RECT d3dlr;
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;

	for (int xy = 0; xy < 8 * 8; xy++)
		*pDst16++ = colour16;

	(*ppD3Dtex)->UnlockRect(0);

	return S_OK;
}

Hack cHack;

void Hack::SaveSettings()
{
	ofstream fout;
	fout.open(GetDirectoryFile("settings.ini"), ios::trunc);
	fout << "Aimbot = " << cAim.bAimbot << endl;
	fout << "ESP = " << cVisuals.bESP << endl;
	fout << "Wallhack = " << cVisuals.bWallhack << endl;
	fout << "Chams = " << cVisuals.bChams << endl;
	fout << "Explosives Chams = " << cVisuals.bExplosiveChams << endl;
	fout << "Weapon Chams = " << cVisuals.bWeaponChams << endl;
	fout << "NoSmoke = " << cVisuals.bNoSmoke << endl;
	fout << "Fullbright = " << cVisuals.bFullbright << endl;
	fout << "DarkMode = " << cVisuals.bDarkMode << endl;
	fout << "MatrixMode = " << cVisuals.bMatrixMode << endl;
	fout << "ShowFPS = " << cVisuals.bFPS << endl;
	fout << "Crosshair = " << cVisuals.iCrosshair << endl;
	fout.close();
}

void Hack::LoadSettings()
{
	ifstream fin;
	string Word = "";
	fin.open(GetDirectoryFile("settings.ini"), ifstream::in);
	fin >> Word >> cAim.bAimbot;
	fin >> Word >> cVisuals.bESP;
	fin >> Word >> cVisuals.bWallhack;
	fin >> Word >> cVisuals.bChams;
	fin >> Word >> cVisuals.bExplosiveChams;
	fin >> Word >> cVisuals.bWeaponChams;
	fin >> Word >> cVisuals.bNoSmoke;
	fin >> Word >> cVisuals.bFullbright;
	fin >> Word >> cVisuals.bDarkMode;
	fin >> Word >> cVisuals.bMatrixMode;
	fin >> Word >> cVisuals.bFPS;
	fin >> Word >> cVisuals.iCrosshair;
	fin.close();
}