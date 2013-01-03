#include <Windows.h>

#include "jcd3d.h"
#include "jcwin32.h"
#include "jcxcommon.h"

using namespace jcd3d;
using namespace jcwin32;

IDirect3DVertexBuffer9* lpvb1 = NULL;
IDirect3DVertexBuffer9* lpvb2 = NULL;
IDirect3DIndexBuffer9* lpib = NULL;
IDirect3DVertexDeclaration9* lpVertexDeclaration = NULL;
IDirect3DVertexShader9* lpVertexShader = NULL;
ID3DXConstantTable* lpConstantTable = NULL;
D3DXHANDLE hMatrixWVP;
D3DXHANDLE hScalar;
FLOAT scalar = 0.0f;

BOOL jcd3d::jcd3d_setup()
{
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, 800, 600);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 2.0f, 2.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	D3DXMATRIX worldMatrix;
	D3DXMatrixIdentity(&worldMatrix);
	jcd3d_lpd3dd->SetTransform(D3DTS_WORLD, &worldMatrix);

	xVerifyFailedIf(jcd3d_lpd3dd->CreateIndexBuffer(3 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lpib, NULL))
	xVerifyFailedEndIf
	WORD* lpibData = NULL;
	xVerifyFailedIf(lpib->Lock(0, 3 * sizeof(WORD), (VOID**)&lpibData, NULL))
		return FALSE;
	xVerifyFailedEndIf
		lpibData[0] = 0; lpibData[1] = 1; lpibData[2] = 2;
	xVerifyFailedIf(lpib->Unlock())
	xVerifyFailedEndIf

	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexBuffer(3 * sizeof(JCD3D_Vertex_xyz_diffuse), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse::FVF, D3DPOOL_MANAGED, &lpvb1, NULL))
		return FALSE;
	xVerifyFailedEndIf
	JCD3D_Vertex_xyz_diffuse* lpvb1Data = NULL;
	xVerifyFailedIf(lpvb1->Lock(0, 3 * sizeof(JCD3D_Vertex_xyz_diffuse), (VOID**)&lpvb1Data, 0))
		return FALSE;
	xVerifyFailedEndIf
		lpvb1Data[0].x = -1.0f; lpvb1Data[0].y = -1.0f, lpvb1Data[0].z = 0.0f; lpvb1Data[0].color = 0xFF0000;
		lpvb1Data[1].x = -1.0f; lpvb1Data[1].y = 1.0f, lpvb1Data[1].z = 0.0f; lpvb1Data[1].color = 0x00FF00;
		lpvb1Data[2].x = 1.0f; lpvb1Data[2].y = -1.0f, lpvb1Data[2].z = 0.0f; lpvb1Data[2].color = 0x0000FF;
	xVerifyFailedIf(lpvb1->Unlock())
		return FALSE;
	xVerifyFailedEndIf

	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexBuffer(3 * sizeof(JCD3D_Vertex_xyz_diffuse), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse::FVF, D3DPOOL_MANAGED, &lpvb2, NULL))
		return FALSE;
	xVerifyFailedEndIf
		JCD3D_Vertex_xyz_diffuse* lpvb2Data = NULL;
	xVerifyFailedIf(lpvb2->Lock(0, 3 * sizeof(JCD3D_Vertex_xyz_diffuse), (VOID**)&lpvb2Data, 0))
		return FALSE;
	xVerifyFailedEndIf
		lpvb2Data[0].x = -1.0f; lpvb2Data[0].y = -1.0f, lpvb2Data[0].z = -1.0f; lpvb2Data[0].color = 0xFF0000;
		lpvb2Data[1].x = -1.0f; lpvb2Data[1].y = 1.0f, lpvb2Data[1].z = -1.0f; lpvb2Data[1].color = 0x00FF00;
		lpvb2Data[2].x = 1.0f; lpvb2Data[2].y = -1.0f, lpvb2Data[2].z = -1.0f; lpvb2Data[2].color = 0x0000FF;
	xVerifyFailedIf(lpvb2->Unlock())
		return FALSE;
	xVerifyFailedEndIf

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
	hScalar = lpConstantTable->GetConstantByName(NULL, "gScalar");
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

	D3DVERTEXELEMENT9 vertexDeclaration[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, 
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0}, 
		{1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1},
		D3DDECL_END()
	};
	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexDeclaration(vertexDeclaration, &lpVertexDeclaration))
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

	jcd3d_lpd3dd->SetStreamSource(0, lpvb1, 0, sizeof(JCD3D_Vertex_xyz_diffuse));
	jcd3d_lpd3dd->SetFVF(JCD3D_Vertex_xyz_diffuse::FVF);
	jcd3d_lpd3dd->SetIndices(lpib);
	jcd3d_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);
	jcd3d_lpd3dd->SetStreamSource(0, lpvb2, 0, sizeof(JCD3D_Vertex_xyz_diffuse));
	jcd3d_lpd3dd->SetFVF(JCD3D_Vertex_xyz_diffuse::FVF);
	jcd3d_lpd3dd->SetIndices(lpib);
	jcd3d_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);

	static FLOAT scalarV = 0.02f;
	lpConstantTable->SetFloat(jcd3d_lpd3dd, hScalar, scalar);
	scalar += scalarV;
	if(scalar < 0.0f)
	{
		scalar = 0.0f;
		scalarV *= -1.0f;
	}
	else if(scalar > 1.0f)
	{
		scalar = 1.0f;
		scalarV *= -1.0f;
	}

	jcd3d_lpd3dd->SetVertexDeclaration(lpVertexDeclaration);
	jcd3d_lpd3dd->SetVertexShader(lpVertexShader);
	jcd3d_lpd3dd->SetStreamSource(0, lpvb1, 0, sizeof(JCD3D_Vertex_xyz_diffuse));
	jcd3d_lpd3dd->SetStreamSource(1, lpvb2, 0, sizeof(JCD3D_Vertex_xyz_diffuse));
	jcd3d_lpd3dd->SetFVF(0);
	jcd3d_lpd3dd->SetIndices(lpib);
	jcd3d_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);
	jcd3d_lpd3dd->SetVertexDeclaration(NULL);
	jcd3d_lpd3dd->SetVertexShader(NULL);
	jcd3d_lpd3dd->SetIndices(NULL);
	jcd3d_lpd3dd->SetStreamSource(0, NULL, 0, 0);
	jcd3d_lpd3dd->SetStreamSource(0, NULL, 0, 0);
}

VOID jcd3d::jcd3d_release()
{
	jccommon_releaseComM(lpib);
	jccommon_releaseComM(lpvb1);
	jccommon_releaseComM(lpvb2);
	jccommon_releaseComM(lpVertexDeclaration);
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