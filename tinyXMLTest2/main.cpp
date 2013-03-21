#include <Windows.h>

#include "tinyxml.h"

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	TiXmlDocument doc;

	TiXmlElement rootElement("data");
	doc.InsertEndChild(rootElement);

	char buffer[50];
	for(INT i = 0; i < 6; ++i)
	{
		TiXmlElement itemElement("item");

		sprintf_s(buffer, 50, "element index %d", i);

		itemElement.SetAttribute("index", i);
		itemElement.InsertEndChild(TiXmlText(buffer));

		doc.RootElement()->InsertEndChild(itemElement);
	}

	doc.SaveFile("myXML.xml");

	return 0;
}