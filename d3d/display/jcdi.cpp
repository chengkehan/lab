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
DIMOUSESTATE jcdi::jcdi_mouseState;
UCHAR jcdi::jcdi_keyboardState[256];

BOOL jcdi::jcdi_initInputDevice(HINSTANCE hInstance)
{
	jccommon_assertM(hInstance != NULL);

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

		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		jcdi_mouseClientWidth = clientRect.right;
		jcdi_mouseClientHeight = clientRect.bottom;
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
		jcdi_mouseX = max(min(jcdi_mouseX + jcdi_mouseState.lX, jcdi_mouseClientWidth), 0);
		jcdi_mouseY = max(min(jcdi_mouseY + jcdi_mouseState.lY, jcdi_mouseClientHeight), 0);
	}

	return TRUE;
}

BOOL jcdi::jcdi_keyDown(INT diKeyCode)
{
	return jcdi_keyboardState[diKeyCode] & 0x80;
}