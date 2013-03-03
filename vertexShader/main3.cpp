#include "jcd3d.h"
#include "jcwin32.h"
#include "jcxcommon.h"

using namespace jcd3d;
using namespace jcwin32;

IDirect3DVertexBuffer9* lpvb = NULL;
IDirect3DIndexBuffer9* lpib = NULL;
ID3DXConstantTable* lpShaderConstantTable = NULL;
IDirect3DVertexShader9* lpVertexShader = NULL;
IDirect3DVertexDeclaration9* lpVertexDecl = NULL;

BOOL jcd3d::jcd3d_setup()
{
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, 800, 600);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 2.0f, 2.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	xVerifyFailedIf(jcd3d_lpd3dd->CreateIndexBuffer(3 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lpib, NULL))xVerifyFailedEndIf
	WORD* lpibData = NULL;
	xVerifyFailedIf(lpib->Lock(0, 3 * sizeof(WORD), (VOID**)&lpibData, NULL))xVerifyFailedEndIf
	lpibData[0] = 0; lpibData[1] = 1; lpibData[2] = 2;
	xVerifyFailedIf(lpib->Unlock())xVerifyFailedEndIf

	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexBuffer(3 * sizeof(JCD3D_Vertex_xyz_diffuse), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse::FVF, D3DPOOL_MANAGED, &lpvb, NULL))	xVerifyFailedEndIf
	JCD3D_Vertex_xyz_diffuse* lpvbData = NULL;
	xVerifyFailedIf(lpvb->Lock(0, 3 * sizeof(JCD3D_Vertex_xyz_diffuse), (VOID**)&lpvbData, 0))xVerifyFailedEndIf
		lpvbData[0].x = -1.0f; lpvbData[0].y = -1.0f, lpvbData[0].z = 0.0f; lpvbData[0].color = 0xFF0000;
		lpvbData[1].x = -1.0f; lpvbData[1].y = 1.0f, lpvbData[1].z = 0.0f; lpvbData[1].color = 0x00FF00;
		lpvbData[2].x = 1.0f; lpvbData[2].y = -1.0f, lpvbData[2].z = 0.0f; lpvbData[2].color = 0x0000FF;
	xVerifyFailedIf(lpvb->Unlock())xVerifyFailedEndIf

	ID3DXBuffer* lpShader = NULL;
	ID3DXBuffer* lpError = NULL;
	xVerifyFailedIf(D3DXCompileShaderFromFileA("testVS2.vs", NULL, NULL, "Main", "vs_1_1", D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &lpShader, &lpError, &lpShaderConstantTable))xVerifyFailedEndIf
	if(lpError != NULL)
	{
		MessageBoxA(NULL, (CHAR*)lpError->GetBufferPointer(), "Error", MB_OK);
		jccommon_releaseComM(lpError);
		return FALSE;
	}
	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexShader((DWORD*)lpShader->GetBufferPointer(), &lpVertexShader))xVerifyFailedEndIf
	jccommon_releaseComM(lpShader);

	D3DXHANDLE wvpMatrixHandle = lpShaderConstantTable->GetConstantByName(NULL, "wvpMatrix");

	D3DXMATRIX worldMatrix;
	D3DXMatrixIdentity(&worldMatrix);
	jcd3d_lpd3dd->SetTransform(D3DTS_WORLD, &worldMatrix);
	D3DXMATRIX viewMatrix;
	xVerifyFailedIf(jcd3d_lpd3dd->GetTransform(D3DTS_VIEW, &viewMatrix))	xVerifyFailedEndIf
	D3DXMATRIX projectionMatrix;
	xVerifyFailedIf(jcd3d_lpd3dd->GetTransform(D3DTS_PROJECTION, &projectionMatrix))	xVerifyFailedEndIf
	D3DXMATRIX wvpMatrix = worldMatrix * viewMatrix * projectionMatrix;

	xVerifyFailedIf(lpShaderConstantTable->SetMatrix(jcd3d_lpd3dd, wvpMatrixHandle, &wvpMatrix))xVerifyFailedEndIf
	xVerifyFailedIf(lpShaderConstantTable->SetDefaults(jcd3d_lpd3dd))xVerifyFailedEndIf

	D3DVERTEXELEMENT9 vertexDecl[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
		{ 0, sizeof(float) * 3, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0}, 
		D3DDECL_END()
	};
	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexDeclaration(vertexDecl, &lpVertexDecl))xVerifyFailedEndIf

	return TRUE;
}

VOID jcd3d::jcd3d_release()
{
	jccommon_releaseComM(lpvb);
	jccommon_releaseComM(lpib);
	jccommon_releaseComM(lpShaderConstantTable);
	jccommon_releaseComM(lpVertexShader);
	jccommon_releaseComM(lpVertexDecl);
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	jcd3d_lpd3dd->SetVertexDeclaration(lpVertexDecl);
	jcd3d_lpd3dd->SetVertexShader(lpVertexShader);
	jcd3d_lpd3dd->SetStreamSource(0, lpvb, 0, sizeof(JCD3D_Vertex_xyz_diffuse));
	jcd3d_lpd3dd->SetIndices(lpib);
	jcd3d_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}