#include <Windows.h>

#include "Base.h"

void Base::DoNow()
{
	func0();
}

void Base::func0()
{
	OutputDebugString(L"Base func0\n");
}