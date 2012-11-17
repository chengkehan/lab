#include <Windows.h>
#include <fstream>
#include "jcd3d.h"
#include "jcdi.h"
#include "jcwin32.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std;
using namespace jcd3d;
using namespace jcdi;
using namespace jcwin32;

IDirect3DTexture9* lpTexture = NULL;
IDirect3DVertexBuffer9* lpvb = NULL;
FLOAT g_imgWidth = 0.0f;
FLOAT g_imgHeight = 0.0f;

VOID windowMoveCallback()
{
	jcdi_mouseLockOnWindow();
}

BOOL jcd3d::jcd3d_setup()
{
	ifstream reader;
	reader.open(L"wall.bmp", ios::ios_base::in | ios::ios_base::binary);
	if(reader.good())
	{
		reader.seekg(0, ios::end);
		int bytesAvailable = (INT)reader.tellg();
		reader.seekg(0, ios::beg);

		char* imgBuffer = new char[bytesAvailable];
		reader.read(imgBuffer, bytesAvailable);
		if(FAILED(D3DXCreateTextureFromFileInMemory(jcd3d_lpd3dd, imgBuffer, bytesAvailable, &lpTexture)))
		{
			jcwin32_messageBoxErrorM("CreateTexture Failed");
		}
		else
		{
			D3DSURFACE_DESC imgDesc;
			lpTexture->GetLevelDesc(0, &imgDesc);
			g_imgWidth = (FLOAT)imgDesc.Width;
			g_imgHeight = (FLOAT)imgDesc.Height;
		}
		delete[] imgBuffer;
	}
	reader.close();

	jcd3d_lpd3dd->CreateVertexBuffer(4 * sizeof(JCD3D_Vertex_xyzrhw_texture), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyzrhw_texture::FVF, D3DPOOL_MANAGED, &lpvb, NULL);
	JCD3D_Vertex_xyzrhw_texture* vb = NULL;
	lpvb->Lock(0, 4 * sizeof(JCD3D_Vertex_xyzrhw_texture), (VOID**)&vb, 0);
	vb[0] = JCD3D_Vertex_xyzrhw_texture(0.0f, g_imgHeight, 0.0f, 1.0f, 0.0f, 1.0f);
	vb[1] = JCD3D_Vertex_xyzrhw_texture(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vb[2] = JCD3D_Vertex_xyzrhw_texture(g_imgWidth, g_imgHeight, 0.0f, 1.0f, 1.0f, 1.0f);
	vb[3] = JCD3D_Vertex_xyzrhw_texture(g_imgWidth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	lpvb->Unlock();

	if(!jcdi_initInput(jcd3d_hInstance, jcd3d_hwnd))
	{
		return FALSE;
	}
	jcdi_mouseSpeed = 1.5f;
	jcwin32_cursorHide();

	jcd3d_windowMoveCallback = windowMoveCallback;
	windowMoveCallback();

	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, jcd3d_windowWidth, jcd3d_windowHeight);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	jcdi_updateInput();
	if(jcdi_keyDown(DIK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	JCD3D_Vertex_xyzrhw_texture* vb = NULL;
	lpvb->Lock(0, 4 * sizeof(JCD3D_Vertex_xyzrhw_texture), (VOID**)&vb, 0);
	vb[0].x = (FLOAT)jcdi_mouseX;//JCD3D_Vertex_xyzrhw_texture(0.0f, g_imgHeight, 0.0f, 1.0f, 0.0f, 1.0f);
	vb[0].y = (FLOAT)jcdi_mouseY + g_imgHeight;
	vb[1].x = (FLOAT)jcdi_mouseX;//JCD3D_Vertex_xyzrhw_texture(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vb[1].y = (FLOAT)jcdi_mouseY;
	vb[2].x = (FLOAT)jcdi_mouseX + g_imgWidth;//JCD3D_Vertex_xyzrhw_texture(g_imgWidth, g_imgHeight, 0.0f, 1.0f, 1.0f, 1.0f);
	vb[2].y = (FLOAT)jcdi_mouseY + g_imgHeight;
	vb[3].x = (FLOAT)jcdi_mouseX + g_imgWidth;//JCD3D_Vertex_xyzrhw_texture(g_imgWidth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vb[3].y = (FLOAT)jcdi_mouseY;
	lpvb->Unlock();

	jcd3d_lpd3dd->SetTexture(0, lpTexture);
	jcd3d_lpd3dd->SetStreamSource(0, lpvb, 0, sizeof(JCD3D_Vertex_xyzrhw_texture));
	jcd3d_lpd3dd->SetFVF(JCD3D_Vertex_xyzrhw_texture::FVF);
	jcd3d_lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

VOID jcd3d::jcd3d_release()
{
	jccommon_releaseComM(lpvb);
	jccommon_releaseComM(lpTexture);
	jcdi_releaseInput();
	jcwin32_cursorShow();
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, 100, 200))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}