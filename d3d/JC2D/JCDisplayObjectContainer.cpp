#include "JCDisplayObjectContainer.h"

using namespace std;
using namespace jcstring;

JCDisplayObjectContainer::JCDisplayObjectContainer(IDirect3DDevice9* lpd3dd):JCDisplayObject(lpd3dd)
{

}

JCDisplayObjectContainer::~JCDisplayObjectContainer()
{

}

INT JCDisplayObjectContainer::getNumChildren() CONST
{
	return m_childrenList.size();
}

JCDisplayObject* JCDisplayObjectContainer::addChild(JCDisplayObject* child)
{
	if(child == NULL)
	{
		return NULL;
	}
	removeChild(child);
	m_childrenList.push_back(child);
	child->setParent(this);

	return child;
}

JCDisplayObject* JCDisplayObjectContainer::addChildAt(JCDisplayObject* child, INT index)
{
	if(child == NULL)
	{
		return NULL;
	}
	if(index < 0 || index > getNumChildren())
	{
		return NULL;
	}

	if(index == getNumChildren())
	{
		addChild(child);
		return child;
	}
	else
	{
		INT indexCount = 0;
		JCDisplayObject* childAdded = NULL;
		for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++indexCount)
		{
			if(indexCount == index)
			{
				childAdded = child;
				m_childrenList.insert(iter, child);
				break;
			}
		}
		jccommon_assertM(childAdded != NULL);
		indexCount = 0;
		for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++indexCount)
		{
			if(indexCount != index && childAdded == *iter)
			{
				m_childrenList.erase(iter);
				break;
			}
		}
		childAdded->setParent(this);
		return childAdded;
	}
}

JCDisplayObject* JCDisplayObjectContainer::removeChild(JCDisplayObject* child)
{
	if(child == NULL)
	{
		return NULL;
	}

	for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		if(*iter == child)
		{
			m_childrenList.erase(iter);
			child->setParent(NULL);
			return child;
		}
	}

	return NULL;
}

JCDisplayObject* JCDisplayObjectContainer::removeChildAt(INT index)
{
	if(index < 0 || index >= getNumChildren())
	{
		return NULL;
	}
	else
	{
		INT indexCount = 0;
		for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++indexCount)
		{
			if(index == indexCount)
			{
				JCDisplayObject* child = *iter;
				m_childrenList.erase(iter);
				child->setParent(NULL);
				return child;
			}
		}
		jccommon_assertM(FALSE);
		return NULL;
	}
}

BOOL JCDisplayObjectContainer::containsChild(JCDisplayObject* child)
{
	if(child == NULL)
	{
		return FALSE;
	}

	for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		if(*iter == child)
		{
			return TRUE;
		}
	}

	return FALSE;
}

INT JCDisplayObjectContainer::getChildIndex(JCDisplayObject* child)
{
	if(child == NULL)
	{
		return -1;
	}

	INT index = 0;
	for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++index)
	{
		if(child == *iter)
		{
			return index;
		}
	}

	return -1;
}

JCDisplayObject* JCDisplayObjectContainer::getChildAt(INT index)
{
	if(index < 0 || index >= getNumChildren())
	{
		return NULL;
	}
	else
	{
		INT indexCount = 0;
		for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter, ++indexCount)
		{
			if(indexCount == index)
			{
				return *iter;
			}
		}
		jccommon_assertM(FALSE);
		return NULL;
	}
}

JCDisplayObject* JCDisplayObjectContainer::getChildByName(LPCWCHAR name)
{
	for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		if(jcstring_wcsequ(name, (*iter)->getName()))
		{
			return *iter;
		}
	}

	return NULL;
}

JCDisplayObject* JCDisplayObjectContainer::setChildIndex(JCDisplayObject* child, INT index)
{
	if(child == NULL)
	{
		return NULL;
	}
	if(index < 0 || index >= getNumChildren())
	{
		return NULL;
	}
	
	if(containsChild(child))
	{
		return addChildAt(child, index);
	}
	else
	{
		return NULL;
	}
}

VOID JCDisplayObjectContainer::render()
{
	JCDisplayObject::render();
	for (list<JCDisplayObject*>::iterator iter = m_childrenList.begin(); iter != m_childrenList.end(); ++iter)
	{
		(*iter)->render();
	}
}