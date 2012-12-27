#include "jcd3d.h"
#include "jcwin32.h"
#include "JCXModel.h"
#include "JCXFile.h"
#include "jccommon.h"

using namespace jcd3d;
using namespace jcwin32;

JCXModel* lpModel = NULL;

BOOL jcd3d::jcd3d_setup()
{
	UINT fileDataBytes = 0;
	if(!JCXFile::readXFile("Warrior.x", NULL, &fileDataBytes))
	{
		return FALSE;
	}
	CHAR* lpFileData = new CHAR[fileDataBytes];
	if(!JCXFile::readXFile("Warrior.x", lpFileData, NULL))
	{
		jccommon_deleteM(lpFileData);
		return FALSE;
	}

	lpModel = new JCXModel(jcd3d_lpd3dd);
	if(!lpModel->parse(lpFileData))
	{
		return FALSE;
	}

	jccommon_deleteM(lpFileData);

	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, 800, 600);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	lpModel->getBone()->updateHierarchy(lpModel->getBone()->getRootBone(), &mat);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	lpModel->draw();

	static FLOAT radius = 50.0f;
	static FLOAT angle = 0.0f;
	jcd3d_setViewTransform(jcd3d_lpd3dd, cosf(angle) * radius, 0.0f, sinf(angle) * radius, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	angle += 0.05;
}

VOID jcd3d::jcd3d_release()
{
	jccommon_deleteM(lpModel);
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