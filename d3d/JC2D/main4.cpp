#include "jc2dGame.h"

using namespace jc2dGame;

JCDisplayObject* lpCursor;

BOOL gameSetup()
{
	if(!theGame_loadFileTexture(0, L"rect.png"))
	{
		return FALSE;
	}

	lpCursor = theGame_newDisplayObject();
	lpCursor->setTexture(theGame_getTexture(0));
	lpCursor->setAlpha(0.6f);
	//lpCursor->setRefX(lpCursor->getWidthOriginal() * 0.5f);
	//lpCursor->setRefY(lpCursor->getHeightOriginal() * 0.5f);
	theGame_stage()->addChild(lpCursor);

	theGame_mouseVisible(FALSE);
	theGame_mouseLockOnWindow(TRUE);
	theGame_setFPS(60);
	theGame_stage()->setAlpha(0.5f);

	return TRUE;
}

VOID gameFrame(DWORD timeDelta)
{
	lpCursor->setX((FLOAT)theGame_mouseX());
	lpCursor->setY((FLOAT)theGame_mouseY());
	//lpCursor->setRotation(lpCursor->getRotation() + 0.005f);

	char buffer[256];
	//sprintf(buffer, "%d\n", timeDelta);
	sprintf(buffer, "x=%f, y=%f, w=%f, h=%f\n", lpCursor->getBounds()->x, lpCursor->getBounds()->y, lpCursor->getBounds()->width, lpCursor->getBounds()->height);
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