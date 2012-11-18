#include "jcdi.h"

HINSTANCE jcdi::jcdi_hInstance = NULL;
HWND jcdi::jcdi_hwnd = NULL;
IDirectInput8* jcdi::jcdi_lpInputDevice = NULL;
IDirectInputDevice8* jcdi::jcdi_lpKeyboard = NULL;
IDirectInputDevice8* jcdi::jcdi_lpMouse = NULL;

INT jcdi::jcdi_mouseX = 0;
INT jcdi::jcdi_mouseY = 0;
INT jcdi::jcdi_mouseClientWidth = 0;
INT jcdi::jcdi_mouseClientHeight = 0;
RECT jcdi::jcdi_mouseClipOriginal;
BOOL jcdi::jcdi_mouseLockedOnWindow = FALSE;
FLOAT jcdi::jcdi_mouseSpeed = 1.0f;
DIMOUSESTATE jcdi::jcdi_mouseState;
UCHAR jcdi::jcdi_keyboardState[256];

using namespace jcwin32;

BOOL jcdi::jcdi_initInputDevice(HINSTANCE hInstance)
{
	jccommon_assertM(hInstance != NULL);
	if(hInstance == NULL)
	{
		return FALSE;
	}

	if(jcdi_lpInputDevice == NULL)
	{
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&jcdi_lpInputDevice, NULL)))
		{
			jcwin32_messageBoxErrorM("DirectInput8Create Failed");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL jcdi::jcdi_initKeyboard(HINSTANCE hInstance, HWND hwnd)
{
	jccommon_assertM(hInstance != NULL);
	jccommon_assertM(hwnd != NULL);
	if(hInstance == NULL || hwnd == NULL)
	{
		return FALSE;
	}

	if(jcdi_lpKeyboard == NULL)
	{
		if(!jcdi_initInputDevice(hInstance))
		{
			return FALSE;
		}

		if(FAILED(jcdi_lpInputDevice->CreateDevice(GUID_SysKeyboard, &jcdi_lpKeyboard, NULL)))
		{
			jcwin32_messageBoxErrorM("Keyboard CreateDevice Failed");
			return FALSE;
		}

		if(FAILED(jcdi_lpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			jcwin32_messageBoxErrorM("Keyboard SetDataFormat Failed");
			return FALSE;
		}

		if(FAILED(jcdi_lpKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			jcwin32_messageBoxErrorM("Keyboard SetCooperativeLevel Failed");
			return FALSE;
		}

		if(FAILED(jcdi_lpKeyboard->Acquire()))
		{
			jcwin32_messageBoxErrorM("Keyboard Acquire Failed");
			return FALSE;
		}
		jcdi_hwnd = hwnd;
		jcdi_hInstance = hInstance;
	}

	return TRUE;
}

BOOL jcdi::jcdi_initMouse(HINSTANCE hInstance, HWND hwnd)
{
	jccommon_assertM(hInstance != NULL);
	jccommon_assertM(hwnd != NULL);
	if(hInstance == NULL || hwnd == NULL)
	{
		return FALSE;
	}

	if(jcdi_lpMouse == NULL)
	{
		if(!jcdi_initInputDevice(hInstance))
		{
			return FALSE;
		}

		if(FAILED(jcdi_lpInputDevice->CreateDevice(GUID_SysMouse, &jcdi_lpMouse, NULL)))
		{
			jcwin32_messageBoxErrorM("Mouse CreateDevice Failed");
			return FALSE;
		}

		if(FAILED(jcdi_lpMouse->SetDataFormat(&c_dfDIMouse)))
		{
			jcwin32_messageBoxErrorM("Mouse SetDataFormat Failed");
			return FALSE;
		}

		if(FAILED(jcdi_lpMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			jcwin32_messageBoxErrorM("Mouse SetCooperativeLevel Failed");
			return FALSE;
		}

		if(FAILED(jcdi_lpMouse->Acquire()))
		{
			jcwin32_messageBoxErrorM("Mouse Acquire Failed");
			return FALSE;
		}

		RECT windowRect;
		jcwin32_getWindowRect(hwnd, &windowRect);
		jcdi_mouseClientWidth = windowRect.right - windowRect.left;
		jcdi_mouseClientHeight = windowRect.bottom - windowRect.top;
		jcdi_hwnd = hwnd;
		jcdi_hInstance = hInstance;
	}

	return TRUE;
}

BOOL jcdi::jcdi_initInput(HINSTANCE hInstance, HWND hwnd)
{
	BOOL result = jcdi_initKeyboard(hInstance, hwnd) && jcdi_initMouse(hInstance, hwnd);
	if(result)
	{
		return TRUE;
	}
	else
	{
		jcdi_releaseInput();
		jcwin32_messageBoxErrorM("Init Input Failed");
		return FALSE;
	}
}

VOID jcdi::jcdi_releaseInput()
{
	jcdi_releaseKeyboard();
	jcdi_releaseMouse();
}

VOID jcdi::jcdi_releaseKeyboard()
{
	if(jcdi_lpKeyboard != NULL)
	{
		jcdi_lpKeyboard->Unacquire();
	}
	jccommon_releaseComM(jcdi_lpKeyboard);
	if(jcdi_lpMouse == NULL)
	{
		jccommon_releaseComM(jcdi_lpInputDevice);
	}
}

VOID jcdi::jcdi_releaseMouse()
{
	if(jcdi_lpMouse != NULL)
	{
		jcdi_lpMouse->Unacquire();
	}
	jccommon_releaseComM(jcdi_lpMouse);
	if(jcdi_lpKeyboard)
	{
		jccommon_releaseComM(jcdi_lpInputDevice);
	}
	jcdi_mouseUnlockOnWindow();
}

BOOL jcdi::jcdi_updateInput()
{
	BOOL keyboardResult = jcdi_lpKeyboard == NULL ? TRUE : jcdi_updateKeyboard();
	BOOL mouseResult = jcdi_lpMouse == NULL ? TRUE : jcdi_updateMouse();

	return keyboardResult && mouseResult;
}

BOOL jcdi::jcdi_updateKeyboard()
{
	if(jcdi_lpKeyboard == NULL)
	{
		return FALSE;
	}
	
	HRESULT result = jcdi_lpKeyboard->GetDeviceState(sizeof(jcdi_keyboardState), (LPVOID)&jcdi_keyboardState);
	if(FAILED(result))
	{
		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			jcdi_lpKeyboard->Acquire();
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL jcdi::jcdi_updateMouse()
{
	if(jcdi_lpMouse == NULL)
	{
		return FALSE;
	}

	HRESULT result = jcdi_lpMouse->GetDeviceState(sizeof(jcdi_mouseState), (LPVOID)&jcdi_mouseState);
	if(FAILED(result))
	{
		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			if(FAILED(jcdi_lpMouse->Acquire()))
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
		jcdi_mouseX = max(min(jcdi_mouseX + (INT)((FLOAT)jcdi_mouseState.lX * jcdi_mouseSpeed), jcdi_mouseClientWidth), 0);
		jcdi_mouseY = max(min(jcdi_mouseY + (INT)((FLOAT)jcdi_mouseState.lY * jcdi_mouseSpeed), jcdi_mouseClientHeight), 0);
	}

	return TRUE;
}

BOOL jcdi::jcdi_keyDown(INT diKeyCode)
{
	return jcdi_keyboardState[diKeyCode] & 0x80;
}

BOOL jcdi::jcdi_mouseLockOnWindow()
{
	if(jcdi_hwnd == NULL)
	{
		return FALSE;
	}

	if(!jcwin32_getClipCursor(&jcdi_mouseClipOriginal))
	{
		return FALSE;
	}

	RECT lockRect;

	POINT screenPoint = { 0, 0 };
	if(!jcwin32_clientToScreen(jcdi_hwnd, &screenPoint))
	{
		return FALSE;
	}
	lockRect.left = screenPoint.x;
	lockRect.top = screenPoint.y;
	
	RECT clientRect;
	if(!jcwin32_getClientRect(jcdi_hwnd, &clientRect))
	{
		return FALSE;
	}
	screenPoint.x = clientRect.right;
	screenPoint.y = clientRect.bottom;
	if(!jcwin32_clientToScreen(jcdi_hwnd, &screenPoint))
	{
		return FALSE;
	}
	lockRect.right = screenPoint.x;
	lockRect.bottom = screenPoint.y;

	if(!jcwin32_clipCursor(&lockRect))
	{
		return FALSE;
	}

	jcdi_mouseLockedOnWindow = TRUE;

	return TRUE;
}

BOOL jcdi::jcdi_mouseUnlockOnWindow()
{
	if(jcdi_hwnd == NULL)
	{
		return FALSE;
	}
	if(!jcdi_mouseLockedOnWindow)
	{
		return FALSE;
	}

	if(!jcwin32_clipCursor(&jcdi_mouseClipOriginal))
	{
		return FALSE;
	}

	jcdi_mouseLockedOnWindow = FALSE;

	return TRUE;
}