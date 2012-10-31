#include "jcd3d.h"

using namespace jcd3d;

const INT WINDOW_WIDTH = 800;
const INT WINDOW_HEIGHT = 600;

ID3DXMesh* g_mesh = NULL;
D3DMATERIAL9* g_materialList = NULL;
IDirect3DTexture9** g_textureList = NULL;
DWORD g_numMaterials = 0;

BOOL jcd3d::jcd3d_setup()
{
	ID3DXBuffer* materialBuffer = NULL;
	if(FAILED(D3DXLoadMeshFromX(L"role.x", D3DXMESH_MANAGED, jcd3d_lpd3dd, NULL, &materialBuffer, NULL, &g_numMaterials, &g_mesh)))
	{
		MessageBox(NULL, L"Load x file failed", L"Error", MB_OK);
		return FALSE;
	}

	D3DXMATERIAL* materials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
	g_materialList = new D3DMATERIAL9[g_numMaterials];
	g_textureList = new IDirect3DTexture9*[g_numMaterials];
	for (DWORD i = 0; i < g_numMaterials; i++)
	{
		g_materialList[i] = materials[i].MatD3D;
		g_materialList[i].Ambient = g_materialList[i].Diffuse;
		g_textureList[i] = NULL;
		if(materials[i].pTextureFilename != NULL && strlen(materials[i].pTextureFilename) > 0)
		{
			D3DXCreateTextureFromFileA(jcd3d_lpd3dd, materials[i].pTextureFilename, &g_textureList[i]);
		}
	}

	jcd3d_releaseHandle(&materialBuffer);


	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, WINDOW_WIDTH, WINDOW_HEIGHT);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	static FLOAT radius = 60.0f;
	static FLOAT eyeY = 0.0f;
	static FLOAT angle = 0.0f;
	static FLOAT speed = 0.05f;
	static FLOAT speed2 = 1.0f;
	if(jcd3d_keyDown('W'))
	{
		eyeY += speed2;
	}
	else if(jcd3d_keyDown('S'))
	{
		eyeY -= speed2;
	}
	if(jcd3d_keyDown('A'))
	{
		angle += speed;
	}
	else if(jcd3d_keyDown('D'))
	{
		angle -= speed;
	}
	jcd3d_setViewTransform(jcd3d_lpd3dd, cosf(angle) * radius, eyeY, sinf(angle) * radius, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	for (DWORD i = 0; i < g_numMaterials; i++)
	{
		jcd3d_lpd3dd->SetMaterial(&g_materialList[i]);
		jcd3d_lpd3dd->SetTexture(0, g_textureList[i]);
		g_mesh->DrawSubset(i);
	}
}

VOID jcd3d::jcd3d_release()
{
	if(g_materialList != NULL)
	{
		delete[] g_materialList;
		g_materialList = NULL;
	}

	if(g_textureList != NULL)
	{
		for (DWORD i = 0; i < g_numMaterials; i++)
		{
			jcd3d_releaseHandle(&g_textureList[i]);
		}
		delete[] g_textureList;
		g_textureList = NULL;
	}

	jcd3d_releaseHandle(&g_mesh);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE, D3DDEVTYPE_HAL, 1))
	{
		return 0;
	}

	jcd3d_loop();

	return 0;
}