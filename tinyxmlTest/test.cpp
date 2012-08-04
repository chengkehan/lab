#include <Windows.h>
#include "tinyxml2.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("J:\\work\\lab_github\\tinyxmlTest\\myXML.xml");
	if(doc.Error())
	{
		OutputDebugString(L"load xml file error");
		return 0;
	}
	
	for (tinyxml2::XMLNode* role = doc.FirstChildElement("data")->FirstChild(); role != NULL; role = role->NextSibling())
	{
		OutputDebugStringA(((tinyxml2::XMLElement*)(role))->FirstAttribute()->Value());
		OutputDebugStringA("\n");
		OutputDebugStringA(role->FirstChildElement("name")->GetText());
		OutputDebugStringA("\n");
		OutputDebugStringA(role->FirstChildElement("age")->GetText());
		OutputDebugStringA("\n");
	}

	return 0;
}