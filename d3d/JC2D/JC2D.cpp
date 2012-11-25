#include "JC2D.h"

using namespace jcd3d;
using namespace jcdi;

JC2D* JC2D::lpJC2D = NULL;

JC2D* JC2D::getInstance()
{
	return JC2D::lpJC2D;
}

VOID JC2D::mouseLockOnWindow()
{
	if(JC2D::getInstance() != NULL)
	{
		//jcdi_mouseLockedOnWindow();
	}
}

JC2D::JC2D():m_init(FALSE), m_running(FALSE), setup(NULL), release(NULL)
{

}

JC2D::~JC2D()
{
	
}

BOOL JC2D::init(HINSTANCE hInstance, INT windowX, INT windowY, INT windowWidth, INT windowHeight, BOOL windowed)
{
	if(m_init)
	{
		return FALSE;
	}

	m_init = TRUE;
	JC2D::lpJC2D = this;
	if(!jcd3d_init(hInstance, windowX, windowY, windowWidth, windowHeight, windowed, D3DDEVTYPE_HAL, 1))
	{
		return FALSE;
	}

	return TRUE;
}

VOID JC2D::run()
{
	if(m_running)
	{
		return;
	}

	m_running = TRUE;
	jcd3d_loop();
}

BOOL jcd3d::jcd3d_setup()
{
	if(JC2D::getInstance()->setup != NULL && !JC2D::getInstance()->setup())
	{
		return FALSE;
	}
	jcd3d_windowMoveCallback = JC2D::mouseLockOnWindow;
	
	if(!jcdi_initInput(jcd3d_hInstance, jcd3d_hwnd))
	{
		return FALSE;
	}

	return TRUE;
}

VOID jcd3d::jcd3d_release()
{
	if(JC2D::getInstance()->release != NULL)
	{
		JC2D::getInstance()->release();
	}
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(JC2D::getInstance()->frame != NULL)
	{
		JC2D::getInstance()->frame(timeDelta);
	}
}