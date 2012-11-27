#include "JCDI.h"

using namespace jcwin32;

JCDI::JCDI():
m_hWnd(NULL), m_hInstance(NULL), m_lpInput(NULL), m_lpKeyboard(NULL), m_lpMouse(NULL), 
m_mouseX(0), m_mouseY(0), m_mouseClientWidth(0), m_mouseClientHeight(0), m_mouseSpeed(1.0f), 
m_mouseLeftButtonDown(FALSE), m_mouseRightButtonDown(FALSE), m_mouseMiddleButtonDown(FALSE)
{

}

JCDI::~JCDI()
{
	if(m_lpKeyboard != NULL)
	{
		m_lpKeyboard->Unacquire();
	}
	jccommon_releaseComM(m_lpKeyboard);

	if(m_lpMouse != NULL)
	{
		m_lpMouse->Unacquire();
	}
	jccommon_releaseComM(m_lpMouse);
	mouseUnlockOnWindow();

	jccommon_releaseComM(m_lpInput);
}

BOOL JCDI::initInput(HINSTANCE hInstance, HWND hWnd)
{
	BOOL result = initKeyboard(hInstance, hWnd) && initMouse(hInstance, hWnd);
	if(result)
	{
		return TRUE;
	}
	else
	{
		jcwin32_messageBoxErrorM("Init Input Failed");
		return FALSE;
	}
}

BOOL JCDI::initKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	if(hInstance == NULL || hWnd == NULL)
	{
		return FALSE;
	}

	if(m_lpKeyboard == NULL)
	{
		if(!initInputDevice(hInstance))
		{
			return FALSE;
		}

		if(FAILED(m_lpInput->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, NULL)))
		{
			jcwin32_messageBoxErrorM("Keyboard CreateDevice Failed");
			return FALSE;
		}

		if(FAILED(m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			jcwin32_messageBoxErrorM("Keyboard SetDataFormat Failed");
			return FALSE;
		}

		if(FAILED(m_lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			jcwin32_messageBoxErrorM("Keyboard SetCooperativeLevel Failed");
			return FALSE;
		}

		if(FAILED(m_lpKeyboard->Acquire()))
		{
			jcwin32_messageBoxErrorM("Keyboard Acquire Failed");
			return FALSE;
		}
		m_hWnd = hWnd;
		m_hInstance = hInstance;
	}

	return TRUE;
}

BOOL JCDI::initMouse(HINSTANCE hInstance, HWND hWnd)
{
	if(hInstance == NULL || hWnd == NULL)
	{
		return FALSE;
	}

	if(m_lpMouse == NULL)
	{
		if(!initInputDevice(hInstance))
		{
			return FALSE;
		}

		if(FAILED(m_lpInput->CreateDevice(GUID_SysMouse, &m_lpMouse, NULL)))
		{
			jcwin32_messageBoxErrorM("Mouse CreateDevice Failed");
			return FALSE;
		}

		if(FAILED(m_lpMouse->SetDataFormat(&c_dfDIMouse)))
		{
			jcwin32_messageBoxErrorM("Mouse SetDataFormat Failed");
			return FALSE;
		}

		if(FAILED(m_lpMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			jcwin32_messageBoxErrorM("Mouse SetCooperativeLevel Failed");
			return FALSE;
		}

		if(FAILED(m_lpMouse->Acquire()))
		{
			jcwin32_messageBoxErrorM("Mouse Acquire Failed");
			return FALSE;
		}

		RECT windowRect;
		jcwin32_getWindowRect(hWnd, &windowRect);
		m_mouseClientWidth = windowRect.right - windowRect.left;
		m_mouseClientHeight = windowRect.bottom - windowRect.top;
		m_hWnd = hWnd;
		m_hInstance = hInstance;
	}

	return TRUE;
}

BOOL JCDI::updateInput()
{
	BOOL keyboardResult = m_lpKeyboard == NULL ? TRUE : updateKeyboard();
	BOOL mouseResult = m_lpMouse == NULL ? TRUE : updateMouse();

	return keyboardResult && mouseResult;
}

BOOL JCDI::updateKeyboard()
{
	if(m_lpKeyboard == NULL)
	{
		return FALSE;
	}

	HRESULT result = m_lpKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_lpKeyboard->Acquire();
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL JCDI::updateMouse()
{
	if(m_lpMouse == NULL)
	{
		return FALSE;
	}

	HRESULT result = m_lpMouse->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			if(FAILED(m_lpMouse->Acquire()))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		m_mouseX = max(min(m_mouseX + (INT)((FLOAT)m_mouseState.lX * m_mouseSpeed), m_mouseClientWidth), 0);
		m_mouseY = max(min(m_mouseY + (INT)((FLOAT)m_mouseState.lY * m_mouseSpeed), m_mouseClientHeight), 0);
		m_mouseLeftButtonDown = m_mouseState.rgbButtons[0];
		m_mouseRightButtonDown = m_mouseState.rgbButtons[1];
		m_mouseMiddleButtonDown = m_mouseState.rgbButtons[2];
	}

	return TRUE;
}

BOOL JCDI::keyDown(INT diKeyCode)
{
	if(diKeyCode < 0 || diKeyCode >= 256)
	{
		return FALSE;
	}
	else
	{
		return m_keyboardState[diKeyCode] & 0x80;
	}	
}

BOOL JCDI::mouseLockOnWindow()
{
	if(m_hWnd == NULL)
	{
		return FALSE;
	}

	if(!jcwin32_getClipCursor(&m_mouseClipOriginal))
	{
		return FALSE;
	}

	RECT lockRect;

	POINT screenPoint = { 0, 0 };
	if(!jcwin32_clientToScreen(m_hWnd, &screenPoint))
	{
		return FALSE;
	}
	lockRect.left = screenPoint.x;
	lockRect.top = screenPoint.y;

	RECT clientRect;
	if(!jcwin32_getClientRect(m_hWnd, &clientRect))
	{
		return FALSE;
	}
	screenPoint.x = clientRect.right;
	screenPoint.y = clientRect.bottom;
	if(!jcwin32_clientToScreen(m_hWnd, &screenPoint))
	{
		return FALSE;
	}
	lockRect.right = screenPoint.x;
	lockRect.bottom = screenPoint.y;

	if(!jcwin32_clipCursor(&lockRect))
	{
		return FALSE;
	}

	m_mouseLockedOnWindow = TRUE;

	return TRUE;
}

BOOL JCDI::mouseUnlockOnWindow()
{
	if(m_hWnd == NULL)
	{
		return FALSE;
	}
	if(!m_mouseLockedOnWindow)
	{
		return FALSE;
	}

	if(!jcwin32_clipCursor(&m_mouseClipOriginal))
	{
		return FALSE;
	}

	m_mouseLockedOnWindow = FALSE;

	return TRUE;
}

BOOL JCDI::initInputDevice(HINSTANCE hInstance)
{
	if(hInstance == NULL)
	{
		return FALSE;
	}

	if(m_lpInput == NULL)
	{
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_lpInput, NULL)))
		{
			jcwin32_messageBoxErrorM("DirectInput8Create Failed");
			return FALSE;
		}
	}
	return TRUE;
}

HWND JCDI::getHWnd()
{
	return m_hWnd;
}

HINSTANCE JCDI::getHInstance()
{
	return m_hInstance;
}

INT JCDI::getMouseX()
{
	return m_mouseX;
}

INT JCDI::getMouseY()
{
	return m_mouseY;
}

BOOL JCDI::getMouseLockedOnWindow()
{
	return m_mouseLockedOnWindow;
}

VOID JCDI::setMouseSpeed(FLOAT speed)
{
	m_mouseSpeed = speed;
}

FLOAT JCDI::getMouseSpeed()
{
	return m_mouseSpeed;
}

BOOL JCDI::getMouseLeftButtonDown()
{
	return m_mouseLeftButtonDown;
}

BOOL JCDI::getMouseRightButtonDown()
{
	return m_mouseRightButtonDown;
}

BOOL JCDI::getMouseMiddleButtonDown()
{
	return m_mouseMiddleButtonDown;
}