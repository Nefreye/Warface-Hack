#pragma once
#include "Windows.h"
#include <detours.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


typedef HRESULT(WINAPI *Prototype_Present)(LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
typedef HRESULT(WINAPI *Prototype_Reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef HRESULT(WINAPI *Prototype_End)(LPDIRECT3DDEVICE9);
typedef HRESULT(WINAPI *Prototype_DrawIndexedPrimitive)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);

Prototype_Present Original_Present;
Prototype_Reset Original_Reset;
Prototype_End Original_End;
Prototype_DrawIndexedPrimitive Original_DrawIndexedPrimitive;

HRESULT WINAPI Hooked_Present(LPDIRECT3DDEVICE9 Device, CONST RECT *pSrcRect, CONST RECT *pDestRect, HWND hDestWindow, CONST RGNDATA *pDirtyRegion);
HRESULT WINAPI Hooked_Reset(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS *pp);
HRESULT WINAPI Hooked_End(LPDIRECT3DDEVICE9 Device);
HRESULT WINAPI Hooked_DrawIndexedPrimitive(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);