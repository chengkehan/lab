#ifndef __JCDI_H__
#define __JCDI_H__

#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <dinput.h>

#include "jccommon.h"
#include "jcwin32.h"

namespace jcdi
{
	extern HINSTANCE jcdi_hInstance;
	extern HWND jcdi_hwnd;
	extern IDirectInput8* jcdi_lpInputDevice;
	extern IDirectInputDevice8* jcdi_lpKeyboard;
	extern IDirectInputDevice8* jcdi_lpMouse;

	extern INT jcdi_mouseX;
	extern INT jcdi_mouseY;
	extern INT jcdi_mouseClientWidth;
	extern INT jcdi_mouseClientHeight;
	extern BOOL jcdi_mouseLockedOnWindow;
	extern RECT jcdi_mouseClipOriginal;
	extern FLOAT jcdi_mouseSpeed;
	extern DIMOUSESTATE jcdi_mouseState;
	extern BOOL jcdi_mouseLeftButtonDown;
	extern BOOL jcdi_mouseRightButtonDown;
	extern BOOL jcdi_mouseMiddleButtonDown;
	extern UCHAR jcdi_keyboardState[256];

	BOOL jcdi_initInput(HINSTANCE hInstance, HWND hwnd);
	BOOL jcdi_initInputDevice(HINSTANCE hInstance);
	BOOL jcdi_initKeyboard(HINSTANCE hInstance, HWND hwnd);
	BOOL jcdi_initMouse(HINSTANCE hInstance, HWND hwnd);
	VOID jcdi_releaseInput();
	VOID jcdi_releaseKeyboard();
	VOID jcdi_releaseMouse();
	BOOL jcdi_updateInput();
	BOOL jcdi_updateKeyboard();
	BOOL jcdi_updateMouse();
	BOOL jcdi_keyDown(INT diKeyCode);
	BOOL jcdi_mouseLockOnWindow();
	BOOL jcdi_mouseUnlockOnWindow();
}

#endif