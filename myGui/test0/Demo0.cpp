#include "Demo0.h"
#include "Base/Main.h"

Demo0::Demo0()
{

}

void Demo0::setupResources()
{
	base::BaseManager::setupResources();
	addResourceLocation(getRootMedia() + "/Demos/Demo_Console");
	addResourceLocation(getRootMedia() + "/Common/Demos");
}

void Demo0::createScene()
{
	MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
	const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
	root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Write commands in console to change some widget parameters. For example \"colour 1 0 0 1\" changes text colour to red.");

	mEdit = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("EditBoxStretch", MyGUI::IntCoord(50, 50, 300, 300), MyGUI::Align::Default, "Overlapped");
	mEdit->setCaption("some edit");
	mEdit->setTextAlign(MyGUI::Align::Default);
	mEdit->setEditMultiLine(true);
	mEdit->setEditWordWrap(true);
}

void Demo0::destroyScene()
{
	base::BaseManager::destroyScene();
}

void Demo0::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
{
	base::BaseManager::injectKeyPress(_key, _text);
}

MYGUI_APP(Demo0)