#include <Windows.h>
#include <dxfile.h>
#include <d3dx9xof.h>
#include <cstdio>
#include <InitGuid.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3dxof.lib")

DEFINE_GUID(Test1Tmpl, 0xbf36e2b1, 0xb82c, 0x49e9, 0xa7, 0xee, 0xe2, 0xcc, 0x6, 0x24, 0xaa, 0x63);

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	char* tmpl = "xof 0303txt 0032 \
				 template Test1Tmpl \
				 { \
				 <BF36E2B1-B82C-49e9-A7EE-E2CC0624AA63> \
				 STRING name; \
				 DWORD age; \
				 }";

	ID3DXFile* lpXFile;
	ID3DXFileEnumObject* lpEnum;
	if(FAILED(D3DXFileCreate(&lpXFile)))
	{
		return 0;
	}

	if(FAILED(lpXFile->RegisterTemplates(tmpl, strlen(tmpl))))
	{
		MessageBox(NULL, L"RegisterTemplates Error", L"Error", MB_OK);
		lpXFile->Release();
		return 0;
	}
	if(FAILED(lpXFile->CreateEnumObject("test1.x", DXFILELOAD_FROMFILE, &lpEnum)))
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

	char buffer[256];
	sprintf(buffer, "numChildren:%d\n", numChildren);
	OutputDebugStringA(buffer);
	for (SIZE_T i = 0; i < numChildren; ++i)
	{
		ID3DXFileData* lpChild = NULL;
		if(FAILED(lpEnum->GetChild(i, &lpChild)))
		{
			MessageBox(NULL, L"GetChild Error", L"Error", MB_OK);
			break;
		}
		GUID childGUID;
		if(FAILED(lpChild->GetType(&childGUID)))
		{
			MessageBox(NULL, L"GetId Error", L"Error", MB_OK);
			lpChild->Release();
			break;
		}

		if(childGUID == Test1Tmpl)
		{
			OutputDebugStringA("Find the Test1Tmpl\n");

			LPCVOID lpChildData;
			SIZE_T childDataSize;
			lpChild->Lock(&childDataSize, &lpChildData);
			sprintf(buffer, "name:%s\n", (CHAR*)lpChildData);
			OutputDebugStringA(buffer);
			CHAR* lpValue = (CHAR*)lpChildData + strlen((CHAR*)lpChildData) + 1;
			sprintf(buffer, "age:%d\n", *lpValue);
			OutputDebugStringA(buffer);
			lpChild->Unlock();
		}

		lpChild->Release();
	}

	lpEnum->Release();
	lpXFile->Release();

	return 0;
}