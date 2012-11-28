#include "Singleton.h"

using namespace std;

auto_ptr<Singleton> Singleton::m_instance(NULL);

Singleton* Singleton::getInstance()
{
	if(m_instance.get() == NULL)
	{
		m_instance.reset(new Singleton());
	}
	return m_instance.get();
}

Singleton::Singleton()
{
	printf("Singleton construct\n");
}

Singleton::~Singleton()
{
	printf("Singleton deconstruct\n");
}