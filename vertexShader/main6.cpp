#include "jcd3d.h"
#include "jcxcommon.h"

using namespace jcd3d;

ID3DXMesh* lpMesh = NULL;
D3DMATERIAL9* lpMaterials = NULL;
IDirect3DIndexBuffer9* lpIB = NULL;
IDirect3DVertexBuffer9* lpVB = NULL;

IDirect3DVertexShader9* lpShader = NULL;
IDirect3DVertexDeclaration9* lpShaderDecl = NULL;
D3DXMATRIX m_WorldMatrix;
D3DXMATRIX m_ViewMatrix;
D3DXMATRIX m_ProjectionMatrix;

BOOL jcd3d::jcd3d_setup()
{
	// Mesh
	ID3DXBuffer* lpMaterialBuffer = NULL;
	DWORD numMaterials = 0;
	
	xVerifyFailedIf(D3DXLoadMeshFromX(L"box.X", D3DXMESH_MANAGED, jcd3d_lpd3dd, NULL, &lpMaterialBuffer, NULL, &numMaterials, &lpMesh))xVerifyFailedEndIf

	D3DXMATERIAL* lpMaterialsData = (D3DXMATERIAL*)lpMaterialBuffer->GetBufferPointer();
	lpMaterials = new D3DMATERIAL9[numMaterials];
	for (DWORD indexMaterial = 0; indexMaterial < numMaterials; ++indexMaterial)
	{
		lpMaterials[indexMaterial] = lpMaterialsData[indexMaterial].MatD3D;
		lpMaterials[indexMaterial].Ambient = lpMaterialsData[indexMaterial].MatD3D.Diffuse;
	}
	jccommon_releaseComM(lpMaterialBuffer);

	DWORD fvf = lpMesh->GetFVF();
	jccommon_assertM(fvf & D3DFVF_XYZ);
	if(!(fvf & D3DFVF_DIFFUSE))
	{
		ID3DXMesh* lpNewMesh = NULL;
		xVerifyFailedIf(lpMesh->CloneMeshFVF(D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_DIFFUSE, jcd3d_lpd3dd, &lpNewMesh))xVerifyFailedEndIf
		jccommon_releaseComM(lpMesh);
		lpMesh = lpNewMesh;
	}

	DWORD numFaces = lpMesh->GetNumFaces();
	DWORD numVB = lpMesh->GetNumVertices();

	JCD3D_Vertex_xyz_diffuse* lpVBData = NULL;
	WORD* lpIBData = NULL;
	DWORD* lpAttr = NULL;
	xVerifyFailedIf(lpMesh->GetVertexBuffer(&lpVB))xVerifyFailedEndIf
	xVerifyFailedIf(lpMesh->GetIndexBuffer(&lpIB))xVerifyFailedEndIf
	xVerifyFailedIf(lpVB->Lock(0, lpMesh->GetNumVertices() * sizeof(JCD3D_Vertex_xyz_diffuse), (void**)&lpVBData, 0))xVerifyFailedEndIf
	xVerifyFailedIf(lpIB->Lock(0, lpMesh->GetNumFaces() * 3 * sizeof(WORD), (void**)&lpIBData, 0))xVerifyFailedEndIf
	xVerifyFailedIf(lpMesh->LockAttributeBuffer(0, &lpAttr))xVerifyFailedEndIf
	for (DWORD indexFace = 0; indexFace < lpMesh->GetNumFaces(); ++indexFace)
	{
		D3DXCOLOR diffuse = lpMaterials[lpAttr[indexFace]].Diffuse;
		lpVBData[lpIBData[indexFace * 3 + 0]].color = diffuse;
		lpVBData[lpIBData[indexFace * 3 + 1]].color = diffuse;
		lpVBData[lpIBData[indexFace * 3 + 2]].color = diffuse;
	}
	xVerifyFailedIf(lpVB->Unlock())xVerifyFailedEndIf
	xVerifyFailedIf(lpIB->Unlock())xVerifyFailedEndIf
	xVerifyFailedIf(lpMesh->UnlockAttributeBuffer())xVerifyFailedEndIf

	xVerifyFailedIf(jcd3d_lpd3dd->SetStreamSource(0, lpVB, 0, sizeof(JCD3D_Vertex_xyz_diffuse)))xVerifyFailedEndIf
	xVerifyFailedIf(jcd3d_lpd3dd->SetIndices(lpIB))xVerifyFailedEndIf

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
	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, D3DX_PI * 0.5f, 800.0f / 600.0f, 1.0f, 1000.0f);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32::jcwin32_exit();
		return;
	}

	static float radian = 0.0f;
	D3DXMATRIX matrix_rotation;
	D3DXMatrixRotationY(&matrix_rotation, radian);
	radian += 0.01f;
	D3DXMATRIX matrix_translation;
	D3DXMatrixTranslation(&matrix_translation, 0.0f, 0.0f, 20.0f);
	D3DXMATRIX ShaderMatrix = matrix_rotation * matrix_translation * m_WorldMatrix * m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixTranspose(&ShaderMatrix, &ShaderMatrix);
	xVerifyFailedIf(jcd3d_lpd3dd->SetVertexShaderConstantF(0, (float*)&ShaderMatrix, 4))xVerifyFailedEndIf
	xVerifyFailedIf(jcd3d_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, lpMesh->GetNumVertices(), 0, lpMesh->GetNumFaces()))xVerifyFailedEndIf
}

VOID jcd3d::jcd3d_release()
{
	jccommon_releaseComM(lpMesh);
	jccommon_deleteArrayM(lpMaterials);
	jccommon_releaseComM(lpVB);
	jccommon_releaseComM(lpIB);
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