#include "jc2dGame.h"

using namespace jc2dGame;

JCDisplayObject* lpBox1 = NULL;
JCDisplayObject* lpBox2 = NULL;

JCDisplayObject* createBox(FLOAT x, FLOAT y)
{
	JCDisplayObject* box = theGame_newDisplayObject();
	box->setTexture(theGame_getTexture(0));
	box->setX(x);
	box->setY(y);
	box->setRefX(box->getWidthOriginal() * 0.5f);
	box->setRefY(box->getHeightOriginal() * 0.5f);
	theGame_stage()->addChild(box);

	return box;
}

BOOL gameSetup()
{
	if(!theGame_loadFileTexture(0, L"rect.png"))
	{
		return FALSE;
	}

	lpBox1 = createBox(200.0f, 200.0f);
	lpBox2 = createBox(400.0f, 400.0f);

	theGame_setFPS(60);
	theGame_mouseVisible(TRUE);
	theGame_mouseLockOnWindow(TRUE);

	return TRUE;
}

VOID gameRelease()
{
	theGame_releaseDisplayObject(lpBox1);
	theGame_releaseDisplayObject(lpBox2);
}

VOID gameFrame(DWORD timeDelta)
{
	lpBox1->setRotation((FLOAT)lpBox1->getRotation() + 0.01f);
	lpBox2->setRotation((FLOAT)lpBox2->getRotation() - 0.01f);
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