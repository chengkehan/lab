#include "jcd3d.h"
#include "jcxcommon.h"

using namespace jcd3d;

const int NUM_POINTS = 20;

IDirect3DVertexBuffer9* lpVB = NULL;
IDirect3DVertexShader9* lpShader = NULL;
IDirect3DVertexDeclaration9* lpShaderDecl = NULL;
D3DXMATRIX m_WorldMatrix;
D3DXMATRIX m_ViewMatrix;
D3DXMATRIX m_ProjectionMatrix;

BOOL jcd3d::jcd3d_setup()
{
	// vertex buffer
	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexBuffer(sizeof(JCD3D_Vertex_xyz_diffuse) * NUM_POINTS, D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse::FVF, D3DPOOL_DEFAULT, &lpVB, NULL))xVerifyFailedEndIf
	JCD3D_Vertex_xyz_diffuse* lpVBData = NULL;
	xVerifyFailedIf(lpVB->Lock(0, sizeof(JCD3D_Vertex_xyz_diffuse) * NUM_POINTS, (void**)&lpVBData, 0))xVerifyFailedEndIf
	for (int pointIndex = 0; pointIndex < NUM_POINTS; ++pointIndex)
	{
		float angle = (float)pointIndex / NUM_POINTS * 2.0f * D3DX_PI;
		lpVBData[pointIndex].x = cosf(angle);
		lpVBData[pointIndex].y = sinf(angle);
		lpVBData[pointIndex].z = 4.0f;
		lpVBData[pointIndex].color = 0xFFFFFFFF;
	}
	xVerifyFailedIf(lpVB->Unlock())xVerifyFailedEndIf
	xVerifyFailedIf(jcd3d_lpd3dd->SetStreamSource(0, lpVB, 0, sizeof(JCD3D_Vertex_xyz_diffuse)))xVerifyFailedEndIf

	// shader
	const char BasicShader[] =
	"vs.1.1                \n"
	"dcl_position0 v0      \n"
	"dcl_color0 v6         \n"
	"dp4 oPos.x, v0, c0    \n"
	"dp4 oPos.y, v0, c1    \n"
	"dp4 oPos.z, v0, c2    \n"
	"dp4 oPos.w, v0, c3    \n"
	"mov oD0,  v6          \n";

	D3DVERTEXELEMENT9 shaderDecl[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
		{ 0, sizeof(float) * 3, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0}, 
		D3DDECL_END()
	};

	ID3DXBuffer* lpShaderBuffer = NULL;
	ID3DXBuffer* lpErrorBuffer = NULL;
	xVerifyFailedIf(D3DXAssembleShader(BasicShader, strlen(BasicShader), NULL, NULL, 0, &lpShaderBuffer, &lpErrorBuffer))xVerifyFailedEndIf
	assert(lpErrorBuffer == NULL);
	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexShader((DWORD*)lpShaderBuffer->GetBufferPointer(), &lpShader))xVerifyFailedEndIf
	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexDeclaration(shaderDecl, &lpShaderDecl))xVerifyFailedEndIf
	jccommon_releaseComM(lpShaderBuffer);

	xVerifyFailedIf(jcd3d_lpd3dd->SetVertexDeclaration(lpShaderDecl))xVerifyFailedEndIf
	xVerifyFailedIf(jcd3d_lpd3dd->SetVertexShader(lpShader))xVerifyFailedEndIf

	// matrix
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, D3DX_PI * 0.5f, 800.0f / 600.0f, 1.0f, 100.0f);

	return true;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32::jcwin32_exit();
		return;
	}

	D3DXMATRIX RotationMatrix1;
	D3DXMATRIX RotationMatrix2;
	D3DXMATRIX TranslationMatrix;
	D3DXMATRIX ScalingMatrix;

	D3DXMatrixRotationZ(&RotationMatrix1,  (float)GetTickCount() / 1000.0f);
	D3DXMatrixRotationZ(&RotationMatrix2, -(float)GetTickCount() / 1000.0f);
	D3DXMatrixTranslation(&TranslationMatrix, 3.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&ScalingMatrix, 1.0f, 0.5f, 1.0f);

	//Create the world matrix
	m_WorldMatrix = ScalingMatrix * RotationMatrix2 * TranslationMatrix * RotationMatrix1;

	//Create the complete matrix
	D3DXMATRIX ShaderMatrix = m_WorldMatrix * m_ViewMatrix * m_ProjectionMatrix;

	//Get the transpose
	D3DXMatrixTranspose(&ShaderMatrix, &ShaderMatrix);

	//Pass the transposed matrix to the shader
	xVerifyFailedIf(jcd3d_lpd3dd->SetVertexShaderConstantF(0, (float*)&ShaderMatrix, 4))xVerifyFailedEndIf

	//Draw...
	jcd3d_lpd3dd->DrawPrimitive(D3DPT_POINTLIST, 0, NUM_POINTS);
}

VOID jcd3d::jcd3d_release()
{
	jccommon_releaseComM(lpVB);
	jccommon_releaseComM(lpShader);
	jccommon_releaseComM(lpShaderDecl);
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