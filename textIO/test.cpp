#include <Windows.h>
#include <fstream>

using namespace std;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	ifstream istream;
	istream.open(L"G:\\work\\lab_github\\textIO\\mytxt.txt", ifstream::in);
	if(istream.good())
	{
		istream.seekg(0, istream::end);
		INT fileLength = (INT)istream.tellg();
		istream.seekg(0, istream.beg);

		CHAR* fileData = (CHAR*)malloc(sizeof(CHAR) * fileLength);
		istream.get(fileData, fileLength);
		OutputDebugStringA(fileData);
		OutputDebugStringA("\n");

		istream.seekg(0, istream.beg);
		while(istream.getline(fileData, fileLength))
		{
			OutputDebugStringA(fileData);
			OutputDebugStringA("\n");
		}

		free(fileData);
	}
	istream.close();

	return 0;
}