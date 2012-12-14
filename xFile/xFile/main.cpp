#include <Windows.h>
#include <dxfile.h>
#include <d3dx9xof.h>
#include <cstdio>
#include <InitGuid.h>
#include <rmxftmpl.h>
#include <rmxfguid.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3dxof.lib")

DEFINE_GUID(Test1Tmpl, 0xbf36e2b1, 0xb82c, 0x49e9, 0xa7, 0xee, 0xe2, 0xcc, 0x6, 0x24, 0xaa, 0x63);
DEFINE_GUID(Test2Tmpl, 0x7823ef1e, 0xaf8d, 0x45bb, 0x99, 0x3a, 0xa2, 0x51, 0x5c, 0x1d, 0x54, 0x70);

VOID parseXFileData(ID3DXFileData* lpXFileData);

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	char* tmpl = "xof 0303txt 0032 \
				 template Test1Tmpl \
				 { \
				 <BF36E2B1-B82C-49e9-A7EE-E2CC0624AA63> \
				 STRING name; \
				 DWORD age; \
				 [...] \
				 } \
				 template Test2Tmpl \
				 { \
				 <7823EF1E-AF8D-45bb-993A-A2515C1D5470> \
				 DWORD value; \
				 }";

	ID3DXFile* lpXFile;
	ID3DXFileEnumObject* lpEnum;
	if(FAILED(D3DXFileCreate(&lpXFile)))
	{
		return 0;
	}

	if(FAILED(lpXFile->RegisterTemplates(D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
	{
		lpXFile->Release();
		return 0;
	}

	if(FAILED(lpXFile->RegisterTemplates(tmpl, strlen(tmpl))))
	{
		MessageBox(NULL, L"RegisterTemplates Error", L"Error", MB_OK);
		lpXFile->Release();
		return 0;
	}
	if(FAILED(lpXFile->CreateEnumObject("airplane.x", DXFILELOAD_FROMFILE, &lpEnum)))
	{
		MessageBox(NULL, L"CreateEnumObject Error", L"Error", MB_OK);
		lpXFile->Release();
		return 0;
	}

	SIZE_T numChildren;
	if(FAILED(lpEnum->GetChildren(&numChildren)))
	{
		MessageBox(NULL, L"GetChildren Error", L"Error", MB_OK);
		lpEnum->Release();
		lpXFile->Release();
		return 0;
	}

	const size_t bufferSize = 256;
	char buffer[bufferSize];
	sprintf_s(buffer, bufferSize, "numChildren:%d\n", numChildren);
	OutputDebugStringA(buffer);
	for (SIZE_T i = 0; i < numChildren; ++i)
	{
		ID3DXFileData* lpChild;
		if(FAILED(lpEnum->GetChild(i, &lpChild)))
		{
			break;
		}
		parseXFileData(lpChild);
		lpChild->Release();
	}

	lpEnum->Release();
	lpXFile->Release();

	return 0;
}

VOID parseXFileData(ID3DXFileData* lpXFileData)
{
	if(lpXFileData == NULL)
	{
		return;
	}

	OutputDebugStringA("----------------------------------------------\n");

	const size_t bufferSize = 256;
	char buffer[bufferSize];

	SIZE_T nameSize;
	if(FAILED(lpXFileData->GetName(NULL, &nameSize)))
	{
		return;
	}
	char* name = new char[nameSize];
	if(FAILED(lpXFileData->GetName(name, &nameSize)))
	{
		delete []name;
		return;
	}
	sprintf_s(buffer, bufferSize, "object name:%s\n", name);
	OutputDebugStringA(buffer);
	delete []name;

	GUID guid;
	if(FAILED(lpXFileData->GetType(&guid)))
	{
		return;
	}

	if(guid == Test1Tmpl)
	{
		SIZE_T dataSize;
		LPCVOID lpData;
		if(FAILED(lpXFileData->Lock(&dataSize, &lpData)))
		{
			return;
		}

		sprintf_s(buffer, bufferSize, "name:%s\n",lpData);
		OutputDebugStringA(buffer);
		CHAR* valuePtr = (CHAR*)lpData + strlen((CHAR*)lpData) + 1;
		sprintf_s(buffer, bufferSize, "age:%d\n", *valuePtr);
		OutputDebugStringA(buffer);

		if(FAILED(lpXFileData->Unlock()))
		{
			return;
		}
	}
	else if(guid == Test2Tmpl)
	{
		SIZE_T dataSize;
		LPCVOID lpData;
		if(FAILED(lpXFileData->Lock(&dataSize, &lpData)))
		{
			return;
		}

		sprintf_s(buffer, bufferSize, "value:%d\n", *(CHAR*)lpData);
		OutputDebugStringA(buffer);

		if(FAILED(lpXFileData->Unlock()))
		{
			return;
		}
	}

	SIZE_T numChildren;
	if(FAILED(lpXFileData->GetChildren(&numChildren)))
	{
		return;
	}
	for (SIZE_T i = 0; i < numChildren; ++i)
	{
		ID3DXFileData* lpChild;
		if(FAILED(lpXFileData->GetChild(i, &lpChild)))
		{
			return;
		}
		parseXFileData(lpChild);
		lpChild->Release();
	}
}