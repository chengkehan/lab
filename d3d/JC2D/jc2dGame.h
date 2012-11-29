#ifndef __JC_2D_GAME_H__
#define __JC_2D_GAME_H__

#include "JC2D.h"

namespace jc2dGame
{
	extern JC2D* theGame;

	inline BOOL theGame_init(HINSTANCE hInstance, JCD3D::SETUPCALLBACK setupCallback = NULL, JCD3D::RELEASECALLBACK releaseCallback = NULL, JCD3D::FRAMECALLBACK frameCallback = NULL, 
		INT windowX = 0, INT windowY = 0, INT windowWidth = 800, INT windowHeight = 600, BOOL windowd = TRUE)
	{
		theGame = JC2D::getInstance();
		return theGame->init(hInstance, setupCallback, releaseCallback, frameCallback, windowX, windowY, windowWidth, windowHeight, windowd);
	}

	inline VOID theGame_run()
	{
		theGame->run();
	}

	inline IDirect3DTexture9* theGame_loadFileTexture(INT id, LPCWSTR path)
	{
		return theGame->getTextureManager()->loadFileTexture(id, path, theGame->getJCD3D()->getDirect3DDevice());
	}

	inline IDirect3DTexture9* theGame_getTexture(INT id)
	{
		return theGame->getTextureManager()->getTexture(id);
	}

	inline JCDisplayObjectContainer* theGame_stage()
	{
		return theGame->getStage();
	}

	inline INT theGame_mouseX()
	{
		return theGame->getJCDI()->getMouseX();
	}

	inline INT theGame_mouseY()
	{
		return theGame->getJCDI()->getMouseY();
	}

	inline BOOL theGame_keyDown(INT diKeyCode)
	{
		return theGame->getJCDI()->keyDown(diKeyCode);
	}

	inline VOID theGame_mouseSpeed(FLOAT value)
	{
		theGame->getJCDI()->setMouseSpeed(value);
	}

	inline BOOL theGame_mouseLeftButtonDown()
	{
		return theGame->getJCDI()->getMouseLeftButtonDown();
	}

	inline BOOL theGame_mouseRightButtonDown()
	{
		return theGame->getJCDI()->getMouseRightButtonDown();
	}

	inline BOOL theGame_mouseMiddleButtonDown()
	{
		return theGame->getJCDI()->getMouseMiddleButtonDown();
	}

	inline VOID theGame_mouseVisible(BOOL value)
	{
		theGame->setMouseVisible(value);
	}

	inline VOID theGame_mouseLockOnWindow(BOOL value)
	{
		theGame->setMouseLockOnWindow(value);
	}

	inline JCDisplayObject* theGame_newDisplayObject()
	{
		return new JCDisplayObject(theGame->getJCD3D()->getDirect3DDevice());
	}

	inline VOID theGame_releaseDisplayObject(JCDisplayObject* value)
	{
		jccommon_deleteM(value);
	}

	inline JCDisplayObjectContainer* theGame_newDisplayObjectContainer()
	{
		return new JCDisplayObjectContainer(theGame->getJCD3D()->getDirect3DDevice());
	}

	inline VOID theGame_releaseDisplayObjectContainer(JCDisplayObjectContainer* value)
	{
		jccommon_deleteM(value);
	}
}

#endif