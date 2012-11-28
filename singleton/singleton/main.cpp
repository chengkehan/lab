#include <cstdio>

#include "Singleton.h"
#include "MySingleton.h"

int main()
{
	Singleton::getInstance();
	MySingleton::getInstance();
	
	getchar();
	return 0;
}