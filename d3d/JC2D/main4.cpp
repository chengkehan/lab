#include "jc2dGame.h"

using namespace jc2dGame;

JCDisplayObject* lpCursor;

BOOL gameSetup()
{
	if(!theGame_loadFileTexture(0, L"cursor.png"))
	{
		return FALSE;
	}

	lpCursor = theGame_newDisplayObject();
	lpCursor->setTexture(theGame_getTexture(0));
	theGame_stage()->addChild(lpCursor);

	theGame_mouseVisible(FALSE);
	theGame_mouseLockOnWindow(TRUE);


	return TRUE;
}

VOID gameFrame(DWORD timeDelta)
{
	lpCursor->setX((FLOAT)theGame_mouseX());
	lpCursor->setY((FLOAT)theGame_mouseY());

	char buffer[256];
	sprintf(buffer, "%d\n", timeDelta);
	OutputDebugStringA(buffer);
}

VOID gameRelease()
{
	jccommon_deleteM(lpCursor);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!theGame_init(hInstance, gameSetup, gameRelease, gameFrame))
	{
		return 0;
	}
	theGame_run();

	return 0;
}