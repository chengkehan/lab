#ifndef __JC_DI_H__
#define __JC_DI_H__

#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <dinput.h>

#include "jccommon.h"
#include "jcwin32.h"

class JCDI
{
public:
	JCDI();
	~JCDI();

	BOOL initInput(HINSTANCE hInstance, HWND hWnd);
	BOOL initKeyboard(HINSTANCE hInstance, HWND hWnd);
	BOOL initMouse(HINSTANCE hInstance, HWND hWnd);
	BOOL updateInput();
	BOOL updateKeyboard();
	BOOL updateMouse();
	BOOL keyDown(INT diKeyCode);
	BOOL mouseLockOnWindow();
	BOOL mouseUnlockOnWindow();
	HWND getHWnd();
	HINSTANCE getHInstance();
	INT getMouseX();
	INT getMouseY();
	BOOL getMouseLockedOnWindow();
	VOID setMouseSpeed(FLOAT speed);
	FLOAT getMouseSpeed();
	BOOL getMouseLeftButtonDown();
	BOOL getMouseRightButtonDown();
	BOOL getMouseMiddleButtonDown();

private:
	JCDI(CONST JCDI& di);

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	IDirectInput8* m_lpInput;
	IDirectInputDevice8* m_lpKeyboard;
	IDirectInputDevice8* m_lpMouse;
	INT m_mouseX;
	INT m_mouseY;
	INT m_mouseClientWidth;
	INT m_mouseClientHeight;
	BOOL m_mouseLockedOnWindow;
	RECT m_mouseClipOriginal;
	FLOAT m_mouseSpeed;
	DIMOUSESTATE m_mouseState;
	BOOL m_mouseLeftButtonDown;
	BOOL m_mouseRightButtonDown;
	BOOL m_mouseMiddleButtonDown;
	UCHAR m_keyboardState[256];

	BOOL initInputDevice(HINSTANCE hInstance);
};

#endif