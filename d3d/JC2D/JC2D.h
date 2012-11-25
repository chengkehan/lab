#ifndef __JC_2D_H__
#define __JC_2D_H__

#include <Windows.h>

#include "jcd3d.h"
#include "jcdi.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class JC2D
{
public:
	typedef BOOL(*JC2DSETUPCALLBACK)(VOID);
	typedef VOID(*JC2DRELEASECALLBACK)(VOID);
	typedef VOID(*JC2DFRAMECALLBACK)(DWORD timeDelta);

public:
	JC2D();
	~JC2D();

public:
	BOOL init(HINSTANCE hInstance, INT windowX = 0, INT windowY = 0, INT windowWidth = 800, INT windowHeight = 600, BOOL windowed = TRUE);
	VOID run();

public:
	JC2DSETUPCALLBACK setup;
	JC2DRELEASECALLBACK release;
	JC2DFRAMECALLBACK frame;

public:
	static JC2D* getInstance();
	static VOID mouseLockOnWindow();

private:
	JC2D(CONST JC2D& jc2d);

private:
	BOOL m_init;
	BOOL m_running;

private:
	static JC2D* lpJC2D;
};

#endif