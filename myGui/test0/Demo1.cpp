#include "Demo1.h"
#include "Base/Main.h"

Demo1::Demo1()
{

}

void Demo1::setupResources()
{
	base::BaseManager::setupResources();
	addResourceLocation(getRootMedia() + "/Demos/Demo_Console");
	addResourceLocation(getRootMedia() + "/Common/Demos");
}

void Demo1::createScene()
{
	const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("project0/test0.layout");
	root.at(0)->findWidget("cb")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &Demo1::cbClickHandler);
}

void Demo1::destroyScene()
{
	base::BaseManager::destroyScene();
}

void Demo1::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
{
	base::BaseManager::injectKeyPress(_key, _text);
}

void Demo1::cbClickHandler(MyGUI::Widget* lpWidget)
{
	MyGUI::Button* button = lpWidget->castType<MyGUI::Button>();
	button->setStateSelected(!button->getStateSelected());
}

MYGUI_APP(Demo1)