#include <Windows.h>

#include "Derived.h"

void Derived::func0()
{
	Base::func0();
	OutputDebugString(L"Derived func0\n");
}