#pragma once

#include "Base.h"

class Derived : public Base
{
public:
	void DoNow();

protected:
	virtual void func0();
};