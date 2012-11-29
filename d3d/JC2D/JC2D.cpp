#include "JC2D.h"

using namespace jcwin32;

JC_SINGLETON_IMPLEMENTS(JC2D)

JC2D::JC2D():m_stage(NULL), m_exitWhileEscapeDown(TRUE), m_mouseVisible(TRUE), m_frameCallback(NULL), m_setupCallback(NULL), m_releaseCallback(NULL)
{

}

JC2D::~JC2D()
{
	if(m_releaseCallback != NULL)
	{
		m_releaseCallback();
	}
}

BOOL JC2D::init(HINSTANCE hInstance, JCD3D::SETUPCALLBACK setupCallback, JCD3D::RELEASECALLBACK releaseCallback, JCD3D::FRAMECALLBACK frameCallback, 
	INT windowX, INT windowY, INT windowWidth, INT windowHeight, BOOL windowd)
{
	m_jc3d.setup = NULL;
	m_jc3d.release = NULL;
	m_jc3d.frame = jc2dFrameCallback;
	m_frameCallback = frameCallback;
	m_setupCallback = setupCallback;
	m_releaseCallback = releaseCallback;

	if(!m_jc3d.init(hInstance, windowX, windowY, windowWidth, windowHeight, windowd))
	{
		return FALSE;
	}
	
	if(!m_jcdi.initInput(hInstance, m_jc3d.getHWnd()))
	{
		return FALSE;
	}

	m_stage = new JCDisplayObjectContainer(m_jc3d.getDirect3DDevice());

	if(m_setupCallback != NULL)
	{
		if(!m_setupCallback())
		{
			return FALSE;
		}
	}

	return TRUE;
}

JCD3D* JC2D::getJCD3D()
{
	return &m_jc3d;
}

JCDI* JC2D::getJCDI()
{
	return &m_jcdi;
}

JCTextureManager* JC2D::getTextureManager()
{
	return &m_textureManager;
}

JCDisplayObjectContainer* JC2D::getStage()
{
	return m_stage;
}

VOID JC2D::run()
{
	m_jc3d.run();
}

VOID JC2D::setExitWhileEscapeDown(BOOL value)
{
	m_exitWhileEscapeDown = value;
}

BOOL JC2D::getExitWhileEscapeDown()
{
	return m_exitWhileEscapeDown;
}

VOID JC2D::setMouseVisible(BOOL value)
{
	m_mouseVisible = value;
	if(m_mouseVisible)
	{
		jcwin32_cursorShow();
	}
	else
	{
		jcwin32_cursorHide();
	}
}

VOID JC2D::setMouseLockOnWindow(BOOL value)
{
	if(value)
	{
		m_jc3d.setMessageCallback(WM_MOVE, jc2dMouseLockOnWindowProc);
		jc2dMouseLockOnWindowProc(NULL, 0, 0, 0);
	}
	else
	{
		m_jc3d.clearMessageCallback(WM_MOVE);
		m_jcdi.mouseUnlockOnWindow();
	}
}

BOOL JC2D::getMouseLockOnWindow()
{
	return m_jcdi.getMouseLockedOnWindow();
}

BOOL JC2D::getMouseVisible()
{
	return m_mouseVisible;
}

VOID JC2D::jc2dFrameCallback(DWORD timeDelta)
{
	JC2D::getInstance()->getJCDI()->updateInput();
	if(JC2D::getInstance()->m_exitWhileEscapeDown && JC2D::getInstance()->getJCDI()->keyDown(DIK_ESCAPE))
	{
		jcwin32_postQuitMessage(0);
		return;
	}

	//jc2dUpdateMouseEvent(JC2D::getInstance()->getStage());

	if(JC2D::getInstance()->m_frameCallback != NULL)
	{
		JC2D::getInstance()->m_frameCallback(timeDelta);
	}
	JC2D::getInstance()->getStage()->render();
}

VOID JC2D::jc2dMouseLockOnWindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	JC2D::getInstance()->getJCDI()->mouseLockOnWindow();
}

VOID JC2D::jc2dUpdateMouseEvent(JCDisplayObjectContainer* lpContainer)
{
	std::list<JCDisplayObject*> children = lpContainer->m_childrenList;
	jccommon_stdIterForEachM(std::list<JCDisplayObject*>, lpContainer->m_childrenList, iter)
	{
		JCDisplayObject* child = *iter;
		if(child->isContainer())
		{
			jc2dUpdateMouseEvent((JCDisplayObjectContainer*)child);
		}
		else
		{

		}
	}
}