#include <Windows.h>

#include "Derived.h"

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	Derived d;
	d.DoNow();

	return 0;
}