#include <Windows.h>

#include "jcd3d.h"
#include "jcwin32.h"
#include "jcxcommon.h"

using namespace jcd3d;
using namespace jcwin32;

ID3DXMesh* lpMesh = NULL;
IDirect3DVertexShader9* lpVertexShader = NULL;
ID3DXConstantTable* lpConstantTable = NULL;
D3DXHANDLE hMatrixWVP;

BOOL jcd3d::jcd3d_setup()
{
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, 800, 600);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 2.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE);

	xVerifyFailedIf(D3DXCreateTeapot(jcd3d_lpd3dd, &lpMesh, NULL))
		return FALSE;
	xVerifyFailedEndIf

	D3DXMATRIX worldMatrix;
	D3DXMatrixIdentity(&worldMatrix);
	jcd3d_lpd3dd->SetTransform(D3DTS_WORLD, &worldMatrix);

	ID3DXBuffer* lpShaderBuffer = NULL;
	ID3DXBuffer* lpErrorBuffer = NULL;
	xVerifyFailedIf(D3DXCompileShaderFromFileA("testVS.vs", NULL, NULL, "Main", "vs_1_1", D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &lpShaderBuffer, &lpErrorBuffer, &lpConstantTable))
		return FALSE;
	xVerifyFailedEndIf
	if(lpErrorBuffer != NULL)
	{
		CHAR* errMsg = (CHAR*)lpErrorBuffer->GetBufferPointer();
		MessageBoxA(jcd3d_hwnd, errMsg, "Error", MB_OK);
		jccommon_releaseComM(lpErrorBuffer);
		return FALSE;
	}
	jccommon_releaseComM(lpErrorBuffer);
	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexShader((DWORD*)lpShaderBuffer->GetBufferPointer(), &lpVertexShader))
		jccommon_releaseComM(lpShaderBuffer);
		return FALSE;
	xVerifyFailedEndIf
	jccommon_releaseComM(lpShaderBuffer);
	hMatrixWVP = lpConstantTable->GetConstantByName(NULL, "gWVP");
	D3DXMATRIX viewMatrix;
	xVerifyFailedIf(jcd3d_lpd3dd->GetTransform(D3DTS_VIEW, &viewMatrix))
		return FALSE;
	xVerifyFailedEndIf
	D3DXMATRIX projectionMatrix;
	xVerifyFailedIf(jcd3d_lpd3dd->GetTransform(D3DTS_PROJECTION, &projectionMatrix))
		return FALSE;
	xVerifyFailedEndIf
	D3DXMATRIX vwpMatrix = worldMatrix * viewMatrix * projectionMatrix;
	xVerifyFailedIf(lpConstantTable->SetMatrix(jcd3d_lpd3dd, hMatrixWVP, &vwpMatrix))
		return FALSE;
	xVerifyFailedEndIf
	xVerifyFailedIf(lpConstantTable->SetDefaults(jcd3d_lpd3dd))
		return FALSE;
	xVerifyFailedEndIf

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	jcd3d_lpd3dd->SetVertexShader(lpVertexShader);
	lpMesh->DrawSubset(0);
}

VOID jcd3d::jcd3d_release()
{
	jccommon_releaseComM(lpMesh);
	jccommon_releaseComM(lpVertexShader);
	jccommon_releaseComM(lpConstantTable);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, 0, 0, 800, 600, TRUE))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}