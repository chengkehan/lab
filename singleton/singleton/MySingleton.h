#ifndef __MY_SINGLETON_H__
#define __MY_SINGLETON_H__

#include "jcSingleton.h"

class MySingleton
{
JC_DECLARE_SINGLETON(MySingleton)

private:
	MySingleton();
	~MySingleton();
};

#endif