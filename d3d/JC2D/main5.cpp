#include "jc2dGame.h"

using namespace jc2dGame;

CONST INT numBoxes = 500;
JCDisplayObject* lpBoxes[numBoxes];

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

	for (INT i = 0; i < numBoxes; ++i)
	{
		FLOAT x = (FLOAT)(rand() % theGame_stageWidth());
		FLOAT y = (FLOAT)(rand() % theGame_stageHeight());
		lpBoxes[i] = createBox(x, y);
	}

	theGame_setFPS(60);
	theGame_mouseVisible(TRUE);
	theGame_mouseLockOnWindow(TRUE);

	return TRUE;
}

VOID gameRelease()
{
	for (INT i = 0; i < numBoxes; ++i)
	{
		theGame_releaseDisplayObject(lpBoxes[i]);
	}
}

VOID gameFrame(DWORD timeDelta)
{
	FLOAT speed;
	INT half = (INT)((FLOAT)numBoxes * 0.5f);
	for (INT i = 0; i < numBoxes; ++i)
	{
		speed = i < half ? 0.05f : -0.05f;
		lpBoxes[i]->setRotation((FLOAT)lpBoxes[i]->getRotation() + speed);
	}

	//char buffer[256];
	//sprintf(buffer, "%d\n", timeDelta);
	//sprintf(buffer, "%f, %f, %f, %f\n", theGame_stage()->getBounds()->x, theGame_stage()->getBounds()->y, theGame_stage()->getBounds()->width, theGame_stage()->getBounds()->height);
	//OutputDebugStringA(buffer);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!theGame_init(hInstance, gameSetup, gameRelease, gameFrame, 0, 0, 1024, 768, TRUE))
	{
		return 0;
	}
	theGame_run();

	return 0;
}