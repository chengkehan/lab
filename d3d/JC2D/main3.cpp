#include "JC2D.h"

using namespace std;

auto_ptr<JCDisplayObject> lpCursor;

BOOL gameSetup()
{
	lpCursor.reset(new JCDisplayObject(JC2D::getInstance()->getJCD3D()->getDirect3DDevice()));
	
	if(!JC2D::getInstance()->getTextureManager()->loadFileTexture(0, L"cursor.png", JC2D::getInstance()->getJCD3D()->getDirect3DDevice()))
	{
		return FALSE;
	}

	lpCursor->setTexture(JC2D::getInstance()->getTextureManager()->getTexture(0));
	JC2D::getInstance()->getStage()->addChild(lpCursor.get());

	JC2D::getInstance()->setMouseVisible(FALSE);
	JC2D::getInstance()->setMouseLockOnWindow(TRUE);

	return TRUE;
}

VOID gameFrame(DWORD timeDelta)
{
	lpCursor->setX((FLOAT)JC2D::getInstance()->getJCDI()->getMouseX());
	lpCursor->setY((FLOAT)JC2D::getInstance()->getJCDI()->getMouseY());
}

VOID gameRelease()
{

}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!JC2D::getInstance()->init(hInstance, gameSetup, gameRelease, gameFrame))
	{
		return 0;
	}
	JC2D::getInstance()->run();

	return 0;
}