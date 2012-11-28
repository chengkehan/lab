#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <cstdio>
#include <memory>

class Singleton
{
friend class std::auto_ptr<Singleton>;

public:
	static Singleton* getInstance();

private:
	Singleton();
	~Singleton();
	Singleton(const Singleton& object);

	static std::auto_ptr<Singleton> m_instance;
};

#endif