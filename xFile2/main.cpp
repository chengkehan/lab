#include <Windows.h>

#include "jcd3d.h"
#include "JCXFileBone.h"
#include "JCXFileBoneTransform.h"
#include "JCXFileSkinMesh.h"
#include "jccommon.h"
#include "jcwin32.h"

using namespace jcd3d;
using namespace jcwin32;

JCXFileBone* lpBone = NULL;
JCXFileBoneTransform* lpBoneTransform = NULL;
JCXFileSkinMesh* lpSkinMesh = NULL;

BOOL jcd3d::jcd3d_setup()
{
	lpBone = new JCXFileBone();
	UINT xFileNumChars = 0;
	CHAR* lpXFileData = NULL;
	if(!JCXFile::readXFile("Warrior.x", NULL, &xFileNumChars))
	{
		jccommon_deleteM(lpBone);
		return 0;
	}
	lpXFileData = new CHAR[xFileNumChars];
	if(!JCXFile::readXFile("Warrior.x", lpXFileData, &xFileNumChars))
	{
		jccommon_deleteM(lpBone);
		jccommon_deleteArrayM(lpXFileData);
		return 0;
	}

	lpBone->parse(lpXFileData);
	jcxcommon_printBones(lpBone->getRootBone(), 0);

	lpBoneTransform = new JCXFileBoneTransform(lpBone);
	lpBoneTransform->parse(lpXFileData);

	lpSkinMesh = new JCXFileSkinMesh(jcd3d_lpd3dd, lpBone);
	lpSkinMesh->parse(lpXFileData);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}
}

VOID jcd3d::jcd3d_release()
{
	jccommon_deleteM(lpBone);
	jccommon_deleteM(lpBoneTransform);
	jccommon_deleteM(lpSkinMesh);
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