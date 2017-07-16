#include "Header.h"

#include "Hooks.h"


#pragma warning (disable : 4099)

/*Hack cHack;
Menu cMenu;
Aimbot cAim;
Visuals cVisuals;
EraseStuff cEraseStuff;*/


int loggedValue = 1500;
int step = 1;


struct ModelInfo_t
{
	float vOutX, vOutY;
	INT       iTeam;
	float CrosshairDistance;
};
std::vector<ModelInfo_t>ModelInfo;

float GetDistance(float Xx, float Yy, float xX, float yY)
{
	return sqrt((yY - Yy) * (yY - Yy) + (xX - Xx) * (xX - Xx));
}

bool IsAddressPresent(void* Address)
{
	for (auto it = g_Vector.begin(); it != g_Vector.end(); ++it)
	{
		if (*it == Address)
			return true;
	}
	return false;
}

void Aimbot::AddModel(LPDIRECT3DDEVICE9 Device, int iTeam)
{
	float xx, yy;
	D3DXMATRIX matrix, worldmatrix, m1;
	D3DXVECTOR4 position, input;
	D3DVIEWPORT9 dip_viewport;
	Device->GetViewport(&dip_viewport);
	Device->GetVertexShaderConstantF(36, m1, 4);
	Device->GetVertexShaderConstantF(0, worldmatrix, 3);

	input.x = worldmatrix._14;
	input.y = worldmatrix._24;
	input.z = worldmatrix._34 + (float)cAim.iAimheight;

	D3DXMatrixTranspose(&matrix, &m1);
	D3DXVec4Transform(&position, &input, &matrix);

	position.x = input.x * matrix._11 + input.y * matrix._21 + input.z * matrix._31 + matrix._41;
	position.y = input.x * matrix._12 + input.y * matrix._22 + input.z * matrix._32 + matrix._42;
	position.z = input.x * matrix._13 + input.y * matrix._23 + input.z * matrix._33 + matrix._43;
	position.w = input.x * matrix._14 + input.y * matrix._24 + input.z * matrix._34 + matrix._44;

	xx = ((position.x / position.w) * (dip_viewport.Width / 2.0f)) + dip_viewport.X + (dip_viewport.Width / 2.0f);
	yy = dip_viewport.Y + (dip_viewport.Height / 2.0f) - ((position.y / position.w) * (dip_viewport.Height / 2.0f));

	ModelInfo_t pModelInfo = { static_cast<float>(xx), static_cast<float>(yy), iTeam };

	ModelInfo.push_back(pModelInfo);
}

void HookThread()
{
	Original_Present = (Prototype_Present)DetourFunction((PBYTE)cHack.GetDeviceAddress(17), (PBYTE)Hooked_Present);
	Original_Reset = (Prototype_Reset)DetourFunction((PBYTE)cHack.GetDeviceAddress(16), (PBYTE)Hooked_Reset);
	Original_End = (Prototype_End)DetourFunction((PBYTE)cHack.GetDeviceAddress(42), (PBYTE)Hooked_End);
	Original_DrawIndexedPrimitive = (Prototype_DrawIndexedPrimitive)DetourFunction((PBYTE)cHack.GetDeviceAddress(82), (PBYTE)Hooked_DrawIndexedPrimitive);

	while (true)
	{
		Sleep(1000);
	}
}

void KeyThread() {
	while (true) {
		if (cHack.hwnd != GetForegroundWindow()) {
			cMenu.bInput = false;
			Sleep(25);
		}
		else if (cHack.hwnd == GetForegroundWindow() && cMenu.bMenu) {
			cMenu.bInput = true;
			Sleep(25);
		}
		else if (cHack.hwnd == GetForegroundWindow() && !cMenu.bMenu){
			cMenu.bInput = false;
			Sleep(25);
		}
		else {
			cMenu.bInput = false;
			Sleep(25);
		}

		/*if (GetAsyncKeyState(VK_RIGHT) & 1)
		{
			if (g_Index != g_Vector.size() - 1)
			{
				g_Index++;
				g_SelectedAddress = g_Vector[g_Index];
			}
		}
		else if (GetAsyncKeyState(VK_LEFT) & 1)
		{
			if (g_Index >= 0)
			{
				g_Index--;
				g_SelectedAddress = g_Vector[g_Index];
				if (g_Index == -1)
					g_SelectedAddress = NULL;
			}
		}*/

		
		if (GetAsyncKeyState(VK_INSERT) & 1)
			cMenu.bMenu = !cMenu.bMenu;


		if (cMenu.bMenu) {

			if (cMenu.index == 1 && !cAim.bAimbot && GetAsyncKeyState(VK_RIGHT) & 1) {
				cAim.bAimbot = true;
				Sleep(25);
			}
			if (cMenu.index == 1 && cAim.bAimbot && GetAsyncKeyState(VK_LEFT) & 1) {
				cAim.bAimbot = false;
				Sleep(25);
			}

			if (cMenu.index == 3 && !cVisuals.bWallhack && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bWallhack = true;
				cVisuals.bChams = false;
				cVisuals.bMatrixMode = false;
				Sleep(25);
			}
			if (cMenu.index == 3 && cVisuals.bWallhack && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bWallhack = false;
				Sleep(25);
			}

			if (cMenu.index == 4 && !cVisuals.bChams && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bChams = true;
				cVisuals.bWallhack = false;
				cVisuals.bMatrixMode = false;
				Sleep(25);
			}
			if (cMenu.index == 4 && cVisuals.bChams && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bChams = false;
				Sleep(25);
			}

			if (cMenu.index == 5 && !cVisuals.bExplosiveChams && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bExplosiveChams = true;
				cVisuals.bMatrixMode = false;
				Sleep(25);
			}
			if (cMenu.index == 5 && cVisuals.bExplosiveChams && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bExplosiveChams = false;
				Sleep(25);
			}

			if (cMenu.index == 6 && !cVisuals.bWeaponChams && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bWeaponChams = true;
				cVisuals.bMatrixMode = false;
				Sleep(25);
			}
			if (cMenu.index == 6 && cVisuals.bWeaponChams && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bWeaponChams = false;
				Sleep(25);
			}

			if (cMenu.index == 7 && !cVisuals.bNoSmoke && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bNoSmoke = true;
				Sleep(25);
			}
			if (cMenu.index == 7 && cVisuals.bNoSmoke && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bNoSmoke = false;
				Sleep(25);
			}

			if (cMenu.index == 8 && !cVisuals.bFullbright && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bFullbright = true;
				Sleep(25);
			}
			if (cMenu.index == 8 && cVisuals.bFullbright && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bFullbright = false;
				Sleep(25);
			}

			if (cMenu.index == 9 && !cVisuals.bDarkMode && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bDarkMode = true;
				cVisuals.bMatrixMode = false;
				Sleep(25);
			}
			if (cMenu.index == 9 && cVisuals.bDarkMode && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bDarkMode = false;
				Sleep(25);
			}

			if (cMenu.index == 10 && !cVisuals.bMatrixMode  && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bMatrixMode = true;
				cVisuals.bDarkMode = false;
				cVisuals.bChams = false;
				cVisuals.bWallhack = false;
				Sleep(25);
			}
			if (cMenu.index == 10 && cVisuals.bMatrixMode && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bMatrixMode = false;
				Sleep(25);
			}

			if (cMenu.index == 11 && !cVisuals.bFPS && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.bFPS = true;
				Sleep(25);
			}
			if (cMenu.index == 11 && cVisuals.bFPS && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.bFPS = false;
				Sleep(25);
			}

			if (cMenu.index == 12 && cVisuals.iCrosshair < 17 && GetAsyncKeyState(VK_RIGHT) & 1) {
				cVisuals.iCrosshair++;
				Sleep(25);
			}
			if (cMenu.index == 12 && cVisuals.iCrosshair > 0 && GetAsyncKeyState(VK_LEFT) & 1) {
				cVisuals.iCrosshair--;
				Sleep(25);
			}

			if (GetAsyncKeyState(VK_HOME) & 1)
				step++;
			else if (GetAsyncKeyState(VK_END) & 1)
				step--;

			if (GetAsyncKeyState(VK_PRIOR) & 1)
				loggedValue += step;
			else if (GetAsyncKeyState(VK_NEXT) & 1)
				loggedValue -= step;


			if (cMenu.index > 1 && GetAsyncKeyState(VK_UP) & 1)
				cMenu.index--;
			if (cMenu.index < 12 && GetAsyncKeyState(VK_DOWN) & 1)
				cMenu.index++;
		}
		Sleep(50);
	}
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		cHack.g_HModule = hInstance;

		cEraseStuff.HideModule(cHack.g_HModule);
		cEraseStuff.EraseHeaders(cHack.g_HModule);
		cEraseStuff.ErasePortableHeader(cHack.g_HModule);

		cHack.hwnd = FindWindow(0, "Warface");

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HookThread, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)KeyThread, 0, 0, 0);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		if (fMenuTitle != NULL)
		{
			fMenuTitle->OnLostDevice();
			fMenuTitle->OnResetDevice();
			fMenuTitle->Release();
			fMenuTitle = NULL;
		}
		if (fMenuText != NULL)
		{
			fMenuText->OnLostDevice();
			fMenuText->OnResetDevice();
			fMenuText->Release();
			fMenuText = NULL;
		}
		if (fTitle != NULL)
		{
			fTitle->OnLostDevice();
			fTitle->OnResetDevice();
			fTitle->Release();
			fTitle = NULL;
		}
		if (fInfo != NULL)
		{
			fInfo->OnLostDevice();
			fInfo->OnResetDevice();
			fInfo->Release();
			fInfo = NULL;
		}
		if (fCursor != NULL)
		{
			fCursor->OnLostDevice();
			fCursor->OnResetDevice();
			fCursor->Release();
			fCursor = NULL;
		}
		DetourRemove((PBYTE)Original_Present, (PBYTE)Hooked_Present);
		DetourRemove((PBYTE)Original_Reset, (PBYTE)Hooked_Reset);
		DetourRemove((PBYTE)Original_End, (PBYTE)Hooked_End);
		DetourRemove((PBYTE)Original_DrawIndexedPrimitive, (PBYTE)Hooked_DrawIndexedPrimitive);
	}
	return TRUE;
}

HRESULT WINAPI Hooked_Present(LPDIRECT3DDEVICE9 Device, CONST RECT *pSrcRect, CONST RECT *pDestRect, HWND hDestWindow, CONST RGNDATA *pDirtyRegion)
{
	if (!cHack.g_Init)
	{
		if (fMenuTitle == NULL)
			D3DXCreateFontA(Device, 20, 0, true, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &fMenuTitle);
		if (fMenuText == NULL)
			D3DXCreateFontA(Device, 15, 0, true, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &fMenuText);
		if (fTitle == NULL)
			D3DXCreateFontA(Device, 20, 0, true, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Impact", &fTitle);
		if (fInfo == NULL)
			D3DXCreateFontA(Device, 20, 0, true, 1, true, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &fInfo);
		if (fCursor == NULL)
			D3DXCreateFontA(Device, 20, 0, true, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &fCursor);
		cHack.GenerateTexture(Device, &cHack.g_Blue, cBlue);
		cHack.GenerateTexture(Device, &cHack.g_Green, cGreen);
		cHack.GenerateTexture(Device, &cHack.g_Null, D3DCOLOR_ARGB(0, 0, 0, 0));
		cHack.GenerateTexture(Device, &cHack.g_White, cWhite);
		cHack.GenerateTexture(Device, &cHack.g_Pink, cPink);
		cHack.GenerateTexture(Device, &cHack.g_Red, cRed);
		cHack.GenerateTexture(Device, &cHack.g_Yellow, cYellow);
		cHack.GenerateTexture(Device, &cHack.g_Orange, cOrange);
		cHack.GenerateTexture(Device, &cHack.g_DarkGreen2, cDarkGreen2);
		cHack.GenerateTexture(Device, &cHack.g_DarkGreen1, cDarkGreen1);
		Device->GetViewport(&cHack.Viewport);
		cHack.ScreenX = (float)cHack.Viewport.Width;
		cHack.ScreenY = (float)cHack.Viewport.Height;
		cHack.ScreenCenterX = (float)cHack.Viewport.Width / 2;
		cHack.ScreenCenterY = (float)cHack.Viewport.Height / 2;
		cHack.g_Init = true;
	}

	return Original_Present(Device, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

HRESULT WINAPI Hooked_Reset(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS *pp)
{
	if (fMenuTitle != NULL)
	{
		fMenuTitle->OnLostDevice();
		fMenuTitle->OnResetDevice();
	}
	if (fMenuText != NULL)
	{
		fMenuText->OnLostDevice();
		fMenuText->OnResetDevice();
	}
	if (fTitle != NULL)
	{
		fTitle->OnLostDevice();
		fTitle->OnResetDevice();
	}
	if (fInfo != NULL)
	{
		fInfo->OnLostDevice();
		fInfo->OnResetDevice();
	}
	if (fCursor != NULL)
	{
		fCursor->OnLostDevice();
		fCursor->OnResetDevice();
	}
	return Original_Reset(Device, pp);
}

HRESULT WINAPI Hooked_DrawIndexedPrimitive(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	IDirect3DVertexBuffer9* pStreamData = NULL;
	UINT xOffset, Stride;

	if (Device->GetStreamSource(0, &pStreamData, &xOffset, &Stride) == D3D_OK)
		pStreamData->Release();
	if (SUCCEEDED(Device->GetPixelShader(&cHack.pShader)))
		if (cHack.pShader != NULL)
			if (SUCCEEDED(cHack.pShader->GetFunction(NULL, &cHack.pSizeOfData)))
				if (cHack.pShader != NULL) { cHack.pShader->Release(); cHack.pShader = NULL; }
	
	void* ReturnAddress = _ReturnAddress();

	if (!IsAddressPresent(ReturnAddress))
		g_Vector.push_back(ReturnAddress);


	/*if (ReturnAddress != NULL && g_SelectedAddress != NULL && ReturnAddress == g_SelectedAddress)
	{
		Device->SetRenderState(D3DRS_ZENABLE, FALSE);
		Device->SetTexture(0, cHack.g_Blue);
		Original_DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		Device->SetRenderState(D3DRS_ZENABLE, TRUE);
		Device->SetTexture(0, cHack.g_Green);
	}*/


	if (ReturnAddress != NULL && ReturnAddress == (void *)WFPlayer /*&& cHack.pSizeOfData == 2268*/) {

		if (cVisuals.bChams) {
			Device->SetRenderState(D3DRS_ZENABLE, FALSE);
			Device->SetTexture(0, cHack.g_Orange);
			Original_DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			Device->SetRenderState(D3DRS_ZENABLE, TRUE);
			Device->SetTexture(0, cHack.g_Yellow);
		}

		//cVisuals.Chams(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

		if (cVisuals.bWallhack) {
			Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			Device->SetRenderState(D3DRS_ZENABLE, FALSE);
			Device->DrawIndexedPrimitive(PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			Device->SetRenderState(D3DRS_ZENABLE, TRUE);
			Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

		if (cAim.bAimbot && cHack.pSizeOfData == 2268) {
			cAim.AddModel(Device, 3);
		}

		//cVisuals.Wallhack(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	}

	/*if (ReturnAddress == (void *)WFPlayer && (Stride == 16 && cHack.pSizeOfData == 640)) {
		AddModel(Device, 1);
	}*/


	if (cVisuals.bMatrixMode && !cVisuals.bDarkMode && ReturnAddress != NULL && ReturnAddress == (void*)WFPlayer/* && cHack.pSizeOfData == 2268*/) {
		cVisuals.MatrixMode(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}


	if (cVisuals.bMatrixMode && !cVisuals.bDarkMode && ReturnAddress != NULL && ReturnAddress == (void*)WFWorld) {
		cVisuals.MatrixMode(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}


	if (cVisuals.bNoSmoke && ReturnAddress != NULL && ReturnAddress == (void*)WFSmoke) {
		return 0;
	}

	if (cVisuals.bDarkMode && ReturnAddress != NULL && ReturnAddress == (void*)WFWorld) {
		cVisuals.DarkMode(Device);
	}


	if (cVisuals.bFullbright && ReturnAddress != NULL && ReturnAddress == (void*)WFFullbright) {
		return 0;
	}


	if (cVisuals.bWeaponChams) {
		if (cHack.pSizeOfData == 1984) {
			cVisuals.WeaponChams(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		}
	}


	if (cVisuals.bExplosiveChams) {
		if (Grenade || Mines) {
			cVisuals.ExplosivesChams(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		}
	}


	if (cHack.pSizeOfData == loggedValue) {
		Device->SetTexture(0, cHack.g_Pink);
		Original_DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		Device->SetTexture(0, cHack.g_Pink);
	}

	return Original_DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT WINAPI Hooked_End(LPDIRECT3DDEVICE9 Device)
{

	if (ModelInfo.size() != NULL)
	{
		for (unsigned int i = 0; i < ModelInfo.size(); i++)
		{
			if (cVisuals.bESP || cAim.bAimbot)
			{
				ModelInfo[i].CrosshairDistance = GetDistance(ModelInfo[i].vOutX, ModelInfo[i].vOutY, cHack.ScreenCenterX, cHack.ScreenCenterY);

				if (ModelInfo[i].vOutX > 0 && ModelInfo[i].vOutY > 0)
					Drawing::Box(Device, (int)ModelInfo[i].vOutX - 11, (int)ModelInfo[i].vOutY - 11, 20, 30, 3, cRed);
			}
		}
	}

	/*
	if (cAim.bAimbot && ModelInfo.size() != NULL)
	{
		UINT BestTarget = -1;
		DOUBLE fClosestPos = 99999;

		for (unsigned int i = 0; i < ModelInfo.size(); i++)
		{
			//aimfov
			float radiusx = (cAim.iAimbotFOV*10.0f) * (cHack.ScreenCenterX / 100);
			float radiusy = (cAim.iAimbotFOV*10.0f) * (cHack.ScreenCenterY / 100);

			if (cAim.iAimbotFOV == 0)
			{
				radiusx = 5.0f * (cHack.ScreenCenterX / 100);
				radiusy = 5.0f * (cHack.ScreenCenterY / 100);
			}

			//get crosshairdistance
			ModelInfo[i].CrosshairDistance = GetDistance(ModelInfo[i].vOutX, ModelInfo[i].vOutY, cHack.ScreenCenterX, cHack.ScreenCenterY);

			//aim at team 1, 2 or 3
			if (cAim.iTeam == ModelInfo[i].iTeam)

				//if in fov
				if (ModelInfo[i].vOutX >= cHack.ScreenCenterX - radiusx && ModelInfo[i].vOutX <= cHack.ScreenCenterX + radiusx && ModelInfo[i].vOutY >= cHack.ScreenCenterY - radiusy && ModelInfo[i].vOutY <= cHack.ScreenCenterY + radiusy)

					//get closest/nearest target to crosshair
					if (ModelInfo[i].CrosshairDistance < fClosestPos)
					{
						fClosestPos = ModelInfo[i].CrosshairDistance;
						BestTarget = i;
					}
		}


		//if nearest target to crosshair
		if (BestTarget != -1)
		{
			double DistX = ModelInfo[BestTarget].vOutX - cHack.ScreenCenterX;
			double DistY = (ModelInfo[BestTarget].vOutY - cHack.ScreenCenterY);

			DistX /= (1 + cAim.iAimsens);
			DistY /= (1 + cAim.iAimsens);

			//aim
			if (GetAsyncKeyState(cAim.Aimkey) & 0x8000)
				mouse_event(MOUSEEVENTF_MOVE, (int)DistX, (int)DistY, 0, NULL);
		}
	}*/

	ModelInfo.clear();

	/*for (int i = 0; i < r - p; i++) {
		if (*(p + i) <= 42) {
			iless++;
			if (iless != i) {
				int tmp = *(p + iless);
				*(p + iless) = *(p + i);
				*(p + i) = tmp;
			}
		}
	}*/


	//ModelInfo.clear();


	
	/*for (int i = 0; i < r - p; i++) {
		if (*(p + i) <= 42) {
			iless++;
			if (iless != i) {
				int tmp = *(p + iless);
				*(p + iless) = *(p + i);
				*(p + i) = tmp;
			}
		}
	}*/

	//FoV
	cVisuals.FoV();

	//Crosshair
	cVisuals.Crosshair(Device);

	//ESP
	//cVisuals.ESP(Device);

	//Aimbot
	//cAim.Run();

	//ModelInfo.clear();

	//Menu
	cMenu.CalculateMenu();

	cMenu.DrawMenu(Device);

	char cLoggedValue[128] = { "\0" };
	sprintf_s(cLoggedValue, sizeof(cLoggedValue), "Value: %d", loggedValue);
	Drawing::DrawString(fTitle, 50, 400, cYellow, cLoggedValue);
	
	char cStep[128] = { "\0" };
	sprintf_s(cStep, sizeof(cStep), "Value: %d", step);
	Drawing::DrawString(fTitle, 50, 450, cYellow, cStep);

	//Watermark
	///Drawing::DrawString(fTitle, cHack.ScreenCenterX - (sizeof("Warface Hack by Dark1027") / 1.5), cHack.ScreenY - 22, cWhite, "Warface Hack by Dark1027");

	/*memset(g_Text, 0, sizeof(g_Text));
	sprintf_s(g_Text, sizeof(g_Text), "Return - Address: [ 0x%X ]", g_SelectedAddress);
	Drawing::DrawString(fTitle, 500, 500, cRed, g_Text);*/

	return Original_End(Device);
}

void Menu::CalculateMenu() {
	if (cMenu.bMenu) {

		if (cAim.bAimbot) {
			cMenu.sAimbot = "Enabled";
			cMenu.cAimbot = cGreen;
		}
		else {
			cMenu.sAimbot = "Disabled";
			cMenu.cAimbot = cRed;
		}

		/*if (cVisuals.bESP) {
			cMenu.sESP = "Enabled";
			cMenu.cESP = cGreen;
		}
		else {
			cMenu.sESP = "Disabled";
			cMenu.cESP = cRed;
		}*/

		/*if (cVisuals.bSnaplines) {
			cMenu.sSnaplines = "Enabled";
			cMenu.cSnaplines = cGreen;
		}
		else {
			cMenu.sSnaplines = "Disabled";
			cMenu.cSnaplines = cRed;
		}*/

		if (cVisuals.bWallhack) {
			cMenu.sWallhack = "Enabled";
			cMenu.cWallhack = cGreen;
		}
		else {
			cMenu.sWallhack = "Disabled";
			cMenu.cWallhack = cRed;
		}

		if (cVisuals.bChams) {
			cMenu.sChams = "Enabled";
			cMenu.cChams = cGreen;
		}
		else {
			cMenu.sChams = "Disabled";
			cMenu.cChams = cRed;
		}

		if (cVisuals.bExplosiveChams) {
			cMenu.sExplosiveChams = "Enabled";
			cMenu.cExplosiveChams = cGreen;
		}
		else {
			cMenu.sExplosiveChams = "Disabled";
			cMenu.cExplosiveChams = cRed;
		}

		if (cVisuals.bWeaponChams) {
			cMenu.sWeaponChams = "Enabled";
			cMenu.cWeaponChams = cGreen;
		}
		else {
			cMenu.sWeaponChams = "Disabled";
			cMenu.cWeaponChams = cRed;
		}

		if (cVisuals.bNoSmoke) {
			cMenu.sNoSmoke = "Enabled";
			cMenu.cNoSmoke = cGreen;
		}
		else {
			cMenu.sNoSmoke = "Disabled";
			cMenu.cNoSmoke = cRed;
		}

		if (cVisuals.bFullbright) {
			cMenu.sFullbright = "Enabled";
			cMenu.cFullbright = cGreen;
		}
		else {
			cMenu.sFullbright = "Disabled";
			cMenu.cFullbright = cRed;
		}

		if (!cVisuals.bMatrixMode && cVisuals.bDarkMode) {
			cMenu.sDarkMode = "Enabled";
			cMenu.cDarkMode = cGreen;
		}
		else {
			cMenu.sDarkMode = "Disabled";
			cMenu.cDarkMode = cRed;
		}

		if (!cVisuals.bDarkMode && cVisuals.bMatrixMode) {
			cMenu.sMatrixMode = "Enabled";
			cMenu.cMatrixMode = cGreen;
		}
		else {
			cMenu.sMatrixMode = "Disabled";
			cMenu.cMatrixMode = cRed;
		}

		if (cVisuals.bFPS) {
			cMenu.sFPS = "Enabled";
			cMenu.cFPS = cGreen;
		}
		else {
			cMenu.sFPS = "Disabled";
			cMenu.cFPS = cRed;
		}

		if (cVisuals.iCrosshair > 0) {
			cMenu.cCrosshair = cGreen;
		}
		else {
			cMenu.sCrosshair = "Disabled";
			cMenu.cCrosshair = cRed;
		}

	}
}

void Menu::DrawMenu(LPDIRECT3DDEVICE9 Device) {
	if (bMenu) {

		Drawing::Box(Device, cMenu.x, cMenu.y, cMenu.w, cMenu.h, cMenu.t, cLightGrey);
		Drawing::DrawString(fMenuTitle, cMenu.x + cMenu.t + 3, cMenu.y + cMenu.t, cGrey, "Warface Hack by Dark1027");
		Drawing::Line(Device, cMenu.x + cMenu.t, cMenu.y + cMenu.spaceY + (cMenu.spaceY * 0.5), cMenu.w - (2 * cMenu.t), 3, cLightGrey);
		//Aimbot
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 2), cYellow, "Aimbot:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 2), cMenu.cAimbot, cMenu.sAimbot);
		/*//ESP
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 3), cYellow, "ESP:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 3), cMenu.cESP, cMenu.sESP);
		//Snaplines
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 4), cYellow, "Snaplines:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 4), cMenu.cSnaplines, cMenu.sSnaplines);*/
		//Wallhack
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 4), cYellow, "Wallhack:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 4), cMenu.cWallhack, cMenu.sWallhack);
		//Chams
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 5), cYellow, "Chams:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 5), cMenu.cChams, cMenu.sChams);
		//ExplosiveChams
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 6), cYellow, "Explosives Chams:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 6), cMenu.cExplosiveChams, cMenu.sExplosiveChams);
		//WeaponChams
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 7), cYellow, "Weapon Chams:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 7), cMenu.cWeaponChams, cMenu.sWeaponChams);
		//NoSmoke
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 8), cYellow, "No Smoke:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 8), cMenu.cNoSmoke, cMenu.sNoSmoke);
		//Fullbright
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 9), cYellow, "Fullbright:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 9), cMenu.cFullbright, cMenu.sFullbright);
		//DarkMode
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 10), cYellow, "Scary Mode:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 10), cMenu.cDarkMode, cMenu.sDarkMode);
		//MatrixMode
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 11), cYellow, "Matrix Mode:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 11), cMenu.cMatrixMode, cMenu.sMatrixMode);
		//FPS
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 12), cYellow, "Show FPS:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 12), cMenu.cFPS, cMenu.sFPS);
		//Crosshair
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceX, cMenu.y + (cMenu.spaceY * 13), cYellow, "Crosshair:");
		Drawing::DrawString(fMenuText, cMenu.x + cMenu.t + cMenu.spaceXX, cMenu.y + (cMenu.spaceY * 13), cMenu.cCrosshair, cMenu.sCrosshair);
		//Selection
		Drawing::Box(Device, cMenu.x + cMenu.spaceX, (cMenu.y + (cMenu.spaceY * (cMenu.index + 1))) - 3, cMenu.w - (cMenu.spaceX * 2), 20, 2, cLightBlue);
	}
}

void Visuals::FoV() {

	if (cVisuals.bFPS)
		Drawing::DrawString(fInfo, 5, 5, cOrange, cVisuals.cFrameRate);

	cVisuals.fCurrentTickCount = clock() * 0.001f;
	cVisuals.fFps++;
	if ((cVisuals.fCurrentTickCount - cVisuals.fLastTickCount) > 1.0f)
	{

		cVisuals.fLastTickCount = cVisuals.fCurrentTickCount;
		memset(cVisuals.cFrameRate, 0, sizeof(cVisuals.cFrameRate));
		sprintf_s(cVisuals.cFrameRate, sizeof(cVisuals.cFrameRate), "FPS: %d", cVisuals.fFps);
		cVisuals.fFps = 0;
	}
}

/*void Visuals::ESP(LPDIRECT3DDEVICE9 Device) {
	if (cVisuals.bESP && ModelInfo.size() != NULL) {
		for (unsigned int i = 0; i < ModelInfo.size(); i++) {
			if (ModelInfo[i].iTeam == 1 || ModelInfo[i].iTeam == 2 || ModelInfo[i].iTeam == 3) {
				if (ModelInfo[i].vOutX <= cHack.ScreenX && ModelInfo[i].vOutY <= cHack.ScreenY) {
					Drawing::Box(Device, ModelInfo[i].vOutX - 11, ModelInfo[i].vOutY - 11, 30, 45, 2, cRed);
				}
			}
		}
	}
}*/

void Visuals::Crosshair(LPDIRECT3DDEVICE9 Device) {


	if (cVisuals.iCrosshair == 1) {//very big
		Drawing::Line(Device, cHack.ScreenCenterX - 50, cHack.ScreenCenterY - 10, 100, 20, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 10, cHack.ScreenCenterY - 50, 20, 100, cRed);
		cMenu.sCrosshair = "1";
	}
	else if (cVisuals.iCrosshair == 2) {
		Drawing::Line(Device, cHack.ScreenCenterX - 50, cHack.ScreenCenterY - 5, 100, 10, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 5, cHack.ScreenCenterY - 50, 10, 100, cRed);
		cMenu.sCrosshair = "2";
	}
	else if (cVisuals.iCrosshair == 3) {
		Drawing::Line(Device, cHack.ScreenCenterX - 50, cHack.ScreenCenterY - 2.5, 100, 5, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 2.5, cHack.ScreenCenterY - 50, 5, 100, cRed);
		cMenu.sCrosshair = "3";
	}

	/*else */if (cVisuals.iCrosshair == 4) {
		Drawing::Line(Device, cHack.ScreenCenterX - 50, cHack.ScreenCenterY - 1, 100, 2, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 1, cHack.ScreenCenterY - 50, 2, 100, cRed);
		cMenu.sCrosshair = "4";
	}
	else if (cVisuals.iCrosshair == 5) {//big
		Drawing::Line(Device, cHack.ScreenCenterX - 35, cHack.ScreenCenterY - 5, 70, 10, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 5, cHack.ScreenCenterY - 35, 10, 70, cRed);
		cMenu.sCrosshair = "5";
	}
	else if (cVisuals.iCrosshair == 6) {
		Drawing::Line(Device, cHack.ScreenCenterX - 35, cHack.ScreenCenterY - 2.5, 70, 5, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 2.5, cHack.ScreenCenterY - 35, 5, 70, cRed);
		cMenu.sCrosshair = "6";
	}
	else if (cVisuals.iCrosshair == 7) {
		Drawing::Line(Device, cHack.ScreenCenterX - 35, cHack.ScreenCenterY - 1, 70, 2, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 1, cHack.ScreenCenterY - 35, 2, 70, cRed);
		cMenu.sCrosshair = "7";
	}
	else if (cVisuals.iCrosshair == 8) {//medium
		Drawing::Line(Device, cHack.ScreenCenterX - 25, cHack.ScreenCenterY - 5, 50, 10, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 5, cHack.ScreenCenterY - 25, 10, 50, cRed);
		cMenu.sCrosshair = "8";
	}

	/*else */if (cVisuals.iCrosshair == 9) {
		Drawing::Line(Device, cHack.ScreenCenterX - 25, cHack.ScreenCenterY - 2.5, 50, 5, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 2.5, cHack.ScreenCenterY - 25, 5, 50, cRed);
		cMenu.sCrosshair = "9";
	}
	else if (cVisuals.iCrosshair == 10) {
		Drawing::Line(Device, cHack.ScreenCenterX - 25, cHack.ScreenCenterY - 1, 50, 2, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 1, cHack.ScreenCenterY - 25, 2, 50, cRed);
		cMenu.sCrosshair = "10";
	}
	else if (cVisuals.iCrosshair == 11) {//small
		Drawing::Line(Device, cHack.ScreenCenterX - 10, cHack.ScreenCenterY - 5, 20, 10, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 5, cHack.ScreenCenterY - 10, 10, 20, cRed);
		cMenu.sCrosshair = "11";
	}

	/*else */if (cVisuals.iCrosshair == 12) {
		Drawing::Line(Device, cHack.ScreenCenterX - 10, cHack.ScreenCenterY - 2.5, 20, 5, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 2.5, cHack.ScreenCenterY - 10, 5, 20, cRed);
		cMenu.sCrosshair = "12";
	}
	else if (cVisuals.iCrosshair == 13) {
		Drawing::Line(Device, cHack.ScreenCenterX - 10, cHack.ScreenCenterY - 1, 20, 2, cRed);
		Drawing::Line(Device, cHack.ScreenCenterX - 1, cHack.ScreenCenterY - 10, 2, 20, cRed);
		cMenu.sCrosshair = "13";
	}
	else if (cVisuals.iCrosshair == 14) {//Dot
		Drawing::Line(Device, cHack.ScreenCenterX - 5, cHack.ScreenCenterY - 5, 10, 10, cRed);
		cMenu.sCrosshair = "14";
	}

	/*else */if (cVisuals.iCrosshair == 15) {
		Drawing::Line(Device, cHack.ScreenCenterX - 2.5, cHack.ScreenCenterY - 2.5, 5, 5, cRed);
		cMenu.sCrosshair = "15";
	}
	else if (cVisuals.iCrosshair == 16) {
		Drawing::Line(Device, cHack.ScreenCenterX - 1, cHack.ScreenCenterY - 1, 2, 2, cRed);
		cMenu.sCrosshair = "16";
	}
	else if (cVisuals.iCrosshair == 17) {
		Drawing::Line(Device, cHack.ScreenCenterX - 0.5, cHack.ScreenCenterY - 0.5, 1, 1, cRed);
		cMenu.sCrosshair = "17";
	}
}

void Visuals::Chams(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
	
}

void Visuals::Wallhack(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
	
}

void Visuals::MatrixMode(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	Device->SetTexture(0, cHack.g_Green);
	Original_DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Device->SetTexture(0, cHack.g_DarkGreen1);
}

void Visuals::DarkMode(LPDIRECT3DDEVICE9 Device) {
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
}

void Visuals::WeaponChams(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	Device->SetTexture(0, cHack.g_Pink);
	Original_DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Device->SetTexture(0, cHack.g_Red);
}

void Visuals::ExplosivesChams(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	Device->SetTexture(0, cHack.g_Pink);
	Original_DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Device->SetTexture(0, cHack.g_Red);
}

/*void Aimbot::Run() {
	
	if (cAim.bAimbot && ModelInfo.size() != NULL)
	{
		UINT BestTarget = -1;
		DOUBLE fClosestPos = 99999;

		

		for (unsigned int i = 0; i < ModelInfo.size(); i++)
		{
			//aimfov
			float radiusx = (cAim.iAimbotFOV*10.0f) * (cHack.ScreenCenterX / 100);
			float radiusy = (cAim.iAimbotFOV*10.0f) * (cHack.ScreenCenterY / 100);

			

			if (cAim.iAimbotFOV == 0)
			{
				radiusx = 5.0f * (cHack.ScreenCenterX / 100);
				radiusy = 5.0f * (cHack.ScreenCenterY / 100);
			}

			//get crosshairdistance
			ModelInfo[i].CrosshairDistance = GetDistance(ModelInfo[i].vOutX, ModelInfo[i].vOutY, cHack.ScreenCenterX, cHack.ScreenCenterY);

			

			//aim at team 1, 2 or 3
			if (cAim.iTeam == ModelInfo[i].iTeam)

				//if in fov
				if (ModelInfo[i].vOutX >= cHack.ScreenCenterX - radiusx && ModelInfo[i].vOutX <= cHack.ScreenCenterX + radiusx && ModelInfo[i].vOutY >= cHack.ScreenCenterY - radiusy && ModelInfo[i].vOutY <= cHack.ScreenCenterY + radiusy)

					//get closest/nearest target to crosshair
					if (ModelInfo[i].CrosshairDistance < fClosestPos)
					{
						

						fClosestPos = ModelInfo[i].CrosshairDistance;
						BestTarget = i;
					}
		}


		//if nearest target to crosshair
		if (BestTarget != -1)
		{
			

			double DistX = ModelInfo[BestTarget].vOutX - cHack.ScreenCenterX;
			double DistY = (ModelInfo[BestTarget].vOutY - cHack.ScreenCenterY);

			DistX /= (1 + cAim.iAimsens);
			DistY /= (1 + cAim.iAimsens);

			

			//aim
			if (GetAsyncKeyState(cAim.Aimkey) & 0x8000)
				mouse_event(MOUSEEVENTF_MOVE, (int)DistX, (int)DistY, 0, NULL);

			

			//autoshoot on
			//if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000))) //
			/*if (((!GetAsyncKeyState(VK_LBUTTON) && (cAim.bAutoshoot) && (GetAsyncKeyState(cAim.Aimkey) & 0x8000))) || ((!GetAsyncKeyState(VK_LBUTTON) && (cAim.bAutoshoot))))
			{
				if (!cAim.bPressed)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					cAim.bPressed = true;
				}
			}*//*
		}
	}*/

	/*if (cAim.bAutoshoot && cAim.bPressed)
	{
		if ((timeGetTime() - cAim.frametime >= 99) || (timeGetTime() >= (cAim.frametime + CLOCKS_PER_SEC / 2)))
		{
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			cAim.bPressed = false;
			cAim.frametime = timeGetTime();
		}
	}*//*
}*/