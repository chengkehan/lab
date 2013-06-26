#pragma once

#include <MyGUI.h>
#include "BaseManager.h"

class Demo1 : public base::BaseManager
{
public:
	Demo1();

	virtual void createScene();
	virtual void destroyScene();

	virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);

protected:
	virtual void setupResources();

private:
	MyGUI::EditBox* mEdit;

	void cbClickHandler(MyGUI::Widget* lpWidget);
};