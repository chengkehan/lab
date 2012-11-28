#ifndef __JC_SINGLETON_H__
#define __JC_SINGLETON_H__

#include <cstdio>
#include <memory>

#define JC_DECLARE_SINGLETON(clazz) \
	friend class std::auto_ptr<##clazz##>; \
	public: \
		static clazz##* getInstance(); \
	private: \
		clazz##(const clazz##& object); \
		static std::auto_ptr<##clazz##> m_instance;

#define JC_IMPLEMENTS_SINGLETON(clazz) \
	std::auto_ptr<##clazz##> clazz##::m_instance(NULL); \
	clazz##* clazz##::getInstance() \
	{ \
		if(m_instance.get() == NULL) \
		{ m_instance.reset(new clazz##()); } \
		return m_instance.get(); \
	}

#endif