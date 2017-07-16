#include <vector>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#pragma warning (disable : 4244)
#pragma warning (disable : 4099)

#define PI 3.14159265

D3DCOLOR cBlack = D3DCOLOR_ARGB(255, 0, 0, 0);
D3DCOLOR cWhite = D3DCOLOR_ARGB(255, 255, 255, 255);
D3DCOLOR cGrey = D3DCOLOR_ARGB(255, 128, 128, 128);
D3DCOLOR cLightGrey = D3DCOLOR_ARGB(255, 192, 192, 192);
D3DCOLOR cRed = D3DCOLOR_ARGB(255, 255, 0, 0);
D3DCOLOR cGreen = D3DCOLOR_ARGB(255, 0, 255, 0);
D3DCOLOR cDarkGreen2 = D3DCOLOR_ARGB(255, 51, 102, 0);
D3DCOLOR cDarkGreen1 = D3DCOLOR_ARGB(255, 0, 82, 0);
D3DCOLOR cGreenOlive = D3DCOLOR_ARGB(255, 76, 153, 0);
D3DCOLOR cBlue = D3DCOLOR_ARGB(255, 0, 0, 255);
D3DCOLOR cBlue2 = D3DCOLOR_ARGB(255, 0, 128, 255);
D3DCOLOR cYellow = D3DCOLOR_ARGB(255, 255, 255, 0);
D3DCOLOR cOrange = D3DCOLOR_ARGB(255, 255, 128, 0);
D3DCOLOR cLightBlue = D3DCOLOR_ARGB(255, 0, 255, 255);
D3DCOLOR cPink = D3DCOLOR_ARGB(255, 255, 0, 255);

ID3DXFont* fMenuTitle = NULL;
ID3DXFont* fMenuText = NULL;
ID3DXFont* fTitle = NULL;
ID3DXFont* fInfo = NULL;
ID3DXFont* fCursor = NULL;

ID3DXLine *pLine;

int cursorX, cursorY;
bool clicking = false;
bool holdclicking = false;

namespace Drawing {

	void Snapline(LPDIRECT3DDEVICE9 Device, int x, int y, int w, int h, D3DCOLOR Color)
	{
		struct D3DVERTEX
		{
			float x, y, z, rhw;
			D3DCOLOR color;
		};
		D3DVERTEX vertices[] =
		{
			{ x, y, 0.5f, 1.0f, Color },//problem 
										//{ x + w, y + h, 0.5f, 1.0f, Color, }//problem 
			{ w, h, 0.5f, 1.0f, Color }    // test 
		};

		Device->SetTexture(0, NULL);
		Device->SetPixelShader(NULL);
		Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Device->SetRenderState(D3DRS_ZENABLE, FALSE);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(D3DVERTEX));
	}

	void Line(LPDIRECT3DDEVICE9 pDevice, float X, float Y, float Width, float Height, D3DCOLOR Color)
	{
		struct Vertex2D
		{
			float m_X, m_Y, m_Z, m_T;
			DWORD m_Color;
		};
		Vertex2D Vertex[4];
		Vertex[0].m_Color = Vertex[1].m_Color = Vertex[2].m_Color = Vertex[3].m_Color = Color;
		Vertex[0].m_Z = Vertex[1].m_Z = Vertex[2].m_Z = Vertex[3].m_Z = 0;
		Vertex[0].m_T = Vertex[1].m_T = Vertex[2].m_T = Vertex[3].m_T = 0;
		Vertex[0].m_X = Vertex[2].m_X = X;
		Vertex[0].m_Y = Vertex[1].m_Y = Y;
		Vertex[1].m_X = Vertex[3].m_X = X + Width;
		Vertex[2].m_Y = Vertex[3].m_Y = Y + Height;
		pDevice->SetTexture(0, NULL);
		pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(Vertex2D));
	}

	void Box(LPDIRECT3DDEVICE9 pDevice, float X, float Y, float Width, float Height, float Thickness, D3DCOLOR Color)
	{
		Line(pDevice, X + Thickness, Y + Height - Thickness, Width - (Thickness * 2), Thickness, Color);
		Line(pDevice, X, Y, Thickness, Height, Color);
		Line(pDevice, X + Thickness, Y, Width - (Thickness * 2), Thickness, Color);
		Line(pDevice, X + Width - Thickness, Y, Thickness, Height, Color);
	}

	void DrawString(ID3DXFont *Font, float PosX, float PosY, DWORD Color, char *Text)
	{
		if (Font == NULL)
			return;
		static RECT FontRect;
		SetRect(&FontRect, 0, 0, 0, 0);
		Font->DrawTextA(0, Text, -1, &FontRect, DT_CALCRECT, Color);
		int Width = FontRect.right - FontRect.left;
		int Height = FontRect.bottom - FontRect.top;
		FontRect.right = FontRect.left + Width;
		FontRect.bottom = FontRect.top + Height;
		FontRect.left = (LONG)PosX;
		FontRect.top = (LONG)PosY;
		Font->DrawTextA(0, Text, -1, &FontRect, DT_NOCLIP, Color);
	}
}