#include <Windows.h>
#include <fstream>

using namespace std;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CHAR* fileData = NULL;
	INT fileSize = 0;

	ifstream istream;
	istream.open(L"J:\\work\\lab_github\\textIO\\mytxt.txt", ifstream::in | ifstream::binary);
	if(istream.good())
	{
		istream.seekg(0, istream::end);
		fileSize = (INT)istream.tellg();
		istream.seekg(0, istream.beg);

		if(fileSize == 0)
		{
			istream.close();
			return 0;
		}

		fileData = (CHAR*)malloc(sizeof(CHAR) * (fileSize + 1));
		istream.read(fileData, fileSize);
		fileData[fileSize] = 0x00;
		OutputDebugStringA(fileData);
	}
	istream.close();

	ofstream ostream;
	ostream.open(L"J:\\work\\lab_github\\textIO\\mytxt_o.txt", ofstream::out | ofstream::binary);
	if(ostream.good())
	{
		ostream.write(fileData, fileSize);
	}
	ostream.close();

	if(fileData != NULL)
	{
		free(fileData);
		fileData = NULL;
	}

	return 0;
}