#include "MySingleton.h"

JC_IMPLEMENTS_SINGLETON(MySingleton)

MySingleton::MySingleton()
{
	printf("MySingleton Constructor\n");
}

MySingleton::~MySingleton()
{
	printf("MySingleton deconstructor\n");
}