#include <Windows.h>
#include <fstream>

#include "jcd3d.h"
#include "jcwin32.h"
#include "jcxcommon.h"

using namespace jcd3d;
using namespace jcwin32;
using namespace std;

UCHAR* lpTerrainData = NULL;
IDirect3DVertexBuffer9* lpTerrainVB = NULL;
IDirect3DIndexBuffer9* lpTerrainIB = NULL;
IDirect3DTexture9* lpTexture = NULL;
CONST FLOAT CELL_WIDTH = 1.2f;
CONST FLOAT CELL_DEPTH = 1.2f;
CONST INT ROWS = 500;
CONST INT COLS = 500;
CONST FLOAT HEIGHT_SCALE = 0.5f;
D3DXVECTOR3 lightDir;

struct Vertex_xyz_uv
{
	static DWORD FVF;
	FLOAT x, y, z;
	FLOAT u, v;
	Vertex_xyz_uv(FLOAT px, FLOAT py, FLOAT pz, FLOAT pu, FLOAT pv)
	{
		x = px; y = py; z = pz;
		u = pu; v = pv;
	}
};
DWORD Vertex_xyz_uv::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

BOOL readFile(LPCSTR file, CHAR* lpFileData, UINT* lpFileDataBytes)
{
	ifstream reader;
	reader.open(file, ios_base::in | ios_base::binary);
	BOOL r = FALSE;
	if(reader.good())
	{
		reader.seekg(0, ios_base::end);
		UINT bytes = (UINT)reader.tellg();
		reader.seekg(0, ios_base::beg);
		if(lpFileDataBytes != NULL)
		{
			*lpFileDataBytes = bytes;
		}
		if(lpFileData != NULL)
		{
			ZeroMemory(lpFileData, bytes);
			reader.read(lpFileData, bytes);
		}
		r = TRUE;
	}
	reader.close();

	return r;
}

FLOAT getTerrainY(INT row, INT col)
{
	if(row < 0 || row >= ROWS || col < 0 || col >= COLS)
	{
		return 0.0f;
	}
	else
	{
		FLOAT v = lpTerrainData[COLS * row + col] * HEIGHT_SCALE;
		return v;
	}
}

BOOL createTerrain()
{
	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexBuffer(ROWS * COLS * sizeof(Vertex_xyz_uv), D3DUSAGE_WRITEONLY, Vertex_xyz_uv::FVF, D3DPOOL_MANAGED, &lpTerrainVB, NULL))
		return FALSE;
	xVerifyFailedEndIf

	Vertex_xyz_uv* lpVB = NULL;
	xVerifyFailedIf(lpTerrainVB->Lock(0, ROWS * COLS * sizeof(Vertex_xyz_uv), (VOID**)&lpVB, 0))
		return FALSE;
	xVerifyFailedEndIf
	FLOAT du = 1.0f / (COLS - 1);
	FLOAT dv = 1.0f / (ROWS - 1);
	for (INT row = 0; row < ROWS; ++row)
	{
		for (INT col = 0; col < COLS; ++col)
		{
			INT index = COLS * row + col;
			lpVB[index].x = col * CELL_WIDTH;
			lpVB[index].z = row * CELL_DEPTH;
			lpVB[index].y = getTerrainY(row, col);
			lpVB[index].u = du * col;
			lpVB[index].v = dv * row;
		}
	}
	xVerifyFailedIf(lpTerrainVB->Unlock())
		return FALSE;
	xVerifyFailedEndIf

	xVerifyFailedIf(jcd3d_lpd3dd->CreateIndexBuffer(sizeof(DWORD) * (ROWS - 1) * (COLS - 1) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &lpTerrainIB, NULL))
		return FALSE;
	xVerifyFailedEndIf

	DWORD* lpIB = NULL;
	xVerifyFailedIf(lpTerrainIB->Lock(0, sizeof(DWORD) * (ROWS - 1) * (COLS - 1) * 6, (VOID**)&lpIB, 0))
		return FALSE;
	xVerifyFailedEndIf
	INT ibIndex = 0;
	for(INT row = 0; row < ROWS - 1; ++row)
	{
		for(INT col = 0; col < COLS - 1; ++col)
		{
			lpIB[ibIndex++] = COLS * row + col;
			lpIB[ibIndex++] = COLS * (row + 1) + col;
			lpIB[ibIndex++] = COLS * (row + 1) + col + 1;

			lpIB[ibIndex++] = COLS * row + col;
			lpIB[ibIndex++] = COLS * (row + 1) + col + 1;
			lpIB[ibIndex++] = COLS * row + col + 1;
		}
	}
	xVerifyFailedIf(lpTerrainIB->Unlock())
		return FALSE;
	xVerifyFailedEndIf

	return TRUE;
}

FLOAT computeShade(INT row, INT col)
{
	float height_a = getTerrainY(row, col);
	float height_b = getTerrainY(row, col + 1);
	float height_c = getTerrainY(row + 1, col);

	D3DXVECTOR3 u(CELL_WIDTH, height_b - height_a, 0.0f);
	D3DXVECTOR3 v(0.0f, height_c - height_a, -CELL_DEPTH);

	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	float cosine = D3DXVec3Dot(&n, &lightDir);

	if(cosine < 0.0f)
	{
		cosine = 0.0f;
	}

	return min(cosine + 0.6f, 1.0f);
}

BOOL createTexture()
{
	lightDir.x = 0.0f;
	lightDir.y = -1.0f;
	lightDir.z = 1.0f;

	xVerifyFailedIf(D3DXCreateTexture(jcd3d_lpd3dd, COLS, ROWS, 0, 0, D3DFMT_X8B8G8R8, D3DPOOL_MANAGED, &lpTexture))
		return FALSE;
	xVerifyFailedEndIf

	D3DLOCKED_RECT lockRect;
	xVerifyFailedIf(lpTexture->LockRect(0, &lockRect, NULL, 0))
		return FALSE;
	xVerifyFailedEndIf
	FLOAT step = HEIGHT_SCALE * 255 * 0.3f;
	DWORD* imgData = (DWORD*)lockRect.pBits;
	for(INT row = 0; row < ROWS; ++row)
	{
		for(INT col = 0; col < COLS; ++col)
		{
			D3DCOLOR color;
			FLOAT height = getTerrainY(row, col);
			if(height < step)
			{
				color = 0xFF914400;
			}
			else if(height >= step && height < step * 2)
			{
				color = 0xFFA2D100;
			}
			else
			{
				color = 0xFFEEEEEE;
			}
			FLOAT shade = computeShade(row, col);
			DWORD a = ((DWORD)(((color & 0xFF000000) >> 24) * shade)) << 24;
			DWORD r = ((DWORD)(((color & 0x00FF0000) >> 16) * shade)) << 16;
			DWORD g = ((DWORD)(((color & 0x0000FF00) >> 8) * shade)) << 8;
			DWORD b = ((DWORD)(((color & 0x000000FF) >> 0) * shade)) << 0;
			color = a + r + g + b;
			imgData[row * (int)(lockRect.Pitch * 0.25) + col] = color;
		}
	}
	xVerifyFailedIf(lpTexture->UnlockRect(0))
		return FALSE;
	xVerifyFailedEndIf

	xVerifyFailedIf(D3DXFilterTexture(lpTexture, NULL, 0, D3DX_DEFAULT))
		return FALSE;
	xVerifyFailedEndIf

	return TRUE;
}

BOOL jcd3d::jcd3d_setup()
{
	UINT fileBytes = 0;
	if(!readFile("terrain.raw", NULL, &fileBytes))
	{
		return FALSE;
	}
	jccommon_assertM(ROWS * COLS == fileBytes);
	lpTerrainData = new UCHAR[fileBytes];
	if(!readFile("terrain.raw", (CHAR*)lpTerrainData, NULL))
	{
		return FALSE;
	}

	if(!createTerrain())
	{
		return FALSE;
	}
	if(!createTexture())
	{
		return FALSE;
	}

	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, 800, 600);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 255 * HEIGHT_SCALE * 1.5f, -ROWS * 0.5f * CELL_DEPTH, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	static FLOAT angle = 0.0f;
	D3DXMATRIX mat1;
	D3DXMATRIX mat2;
	D3DXMATRIX mat3;
	D3DXMatrixIdentity(&mat1);
	D3DXMatrixIdentity(&mat2);
	D3DXMatrixIdentity(&mat3);
	D3DXMatrixTranslation(&mat1, -COLS * 0.5f * CELL_WIDTH, 0.0f, -ROWS * 0.5f * CELL_DEPTH);
	D3DXMatrixRotationY(&mat2, angle);
	mat3 = mat1 * mat2;
	angle += 0.01f;
	jcd3d_lpd3dd->SetTransform(D3DTS_WORLD, &mat3);

	jcd3d_lpd3dd->SetFVF(Vertex_xyz_uv::FVF);
	jcd3d_lpd3dd->SetTexture(0, lpTexture);
	jcd3d_lpd3dd->SetStreamSource(0, lpTerrainVB, 0, sizeof(Vertex_xyz_uv));
	jcd3d_lpd3dd->SetIndices(lpTerrainIB);
	jcd3d_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, ROWS * COLS, 0, (ROWS - 1) * (COLS - 1) * 2);
}

VOID jcd3d::jcd3d_release()
{
	jccommon_deleteArrayM(lpTerrainData);
	jccommon_releaseComM(lpTerrainVB);
	jccommon_releaseComM(lpTerrainIB);
	jccommon_releaseComM(lpTexture);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, 0, 0, 800, 600))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}