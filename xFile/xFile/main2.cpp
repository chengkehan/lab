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

CONST INT PRINT_BUFFER_SIZE = 256;
CHAR printBuffer[PRINT_BUFFER_SIZE];
#define trace(format, value) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value); OutputDebugStringA(printBuffer); }
#define trace2(format, value1, value2) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value1, value2); OutputDebugStringA(printBuffer); }
#define trace3(format, value1, value2, value3) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value1, value2, value3); OutputDebugStringA(printBuffer); }

VOID parseXFile(ID3DXFile* lpXFile);
VOID parseXFileData(ID3DXFileData* lpXFileData, INT depth);

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	ID3DXFile* lpXFile;

	if(FAILED(D3DXFileCreate(&lpXFile)))
	{
		return 0;
	}

	if(FAILED(lpXFile->RegisterTemplates(D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
	{
		lpXFile->Release();
		return 0;
	}

	parseXFile(lpXFile);
	lpXFile->Release();

	return 0;
}

VOID parseXFile(ID3DXFile* lpXFile)
{
	if(lpXFile == NULL)
	{
		return;
	}

	ID3DXFileEnumObject* lpEnum;
	if(FAILED(lpXFile->CreateEnumObject("airplane.x", D3DXF_FILELOAD_FROMFILE, &lpEnum)))
	{
		return;
	}

	SIZE_T numChildren;
	if(FAILED(lpEnum->GetChildren(&numChildren)))
	{
		lpEnum->Release();
		return;
	}

	for (SIZE_T i = 0; i < numChildren; ++i)
	{
		ID3DXFileData* lpXFileData;
		if(FAILED(lpEnum->GetChild(i, &lpXFileData)))
		{
			break;
		}
		parseXFileData(lpXFileData, 0);
		lpXFileData->Release();
	}

	lpEnum->Release();
}

VOID parseXFileData(ID3DXFileData* lpXFileData, INT depth)
{
	if(lpXFileData == NULL)
	{
		return;
	}

	SIZE_T nameSize;
	CHAR* lpName;
	if(FAILED(lpXFileData->GetName(NULL, &nameSize)))
	{
		return;
	}
	lpName = new CHAR[nameSize];
	if(FAILED(lpXFileData->GetName(lpName, &nameSize)))
	{
		delete []lpName;
		return;
	}

	CHAR* lpTab = NULL;
	if(depth > 0)
	{
		lpTab = new CHAR[depth + 1];
		for (INT depthIndex = 0; depthIndex < depth; ++depthIndex)
		{
			lpTab[depthIndex] = '	';
		}
		lpTab[depth] = '\0';
	}

	GUID guid;
	if(FAILED(lpXFileData->GetType(&guid)))
	{
		delete []lpName;
		delete []lpTab;
		return;
	}

	char* tmpl = 
		guid == TID_D3DRMFrame ? "Frame" : 
		guid == TID_D3DRMFrameTransformMatrix ? "FrameTransformMatrix" : 
		guid == TID_D3DRMMeshMaterialList ? "MeshMaterialList" : 
		guid == TID_D3DRMMeshTextureCoords ? "MeshTextureCoords" : 
		guid == TID_D3DRMMeshNormals ? "MeshNormal" : 
		guid == TID_D3DRMMesh ? "Mesh" : 
		guid == TID_D3DRMMaterial ? "Material" : 
		guid == TID_D3DRMTextureFilename ? "TextureFilename" : NULL;

	trace2("%s%s", lpTab == NULL ? "" : lpTab,"---------------------------------------------------\n");
	trace3("%sname:%s, tmpl:%s\n", lpTab == NULL ? "" : lpTab, lpName, tmpl == NULL ? "" : tmpl);
	delete []lpName;
	delete []lpTab;

	SIZE_T numChildren;
	if(FAILED(lpXFileData->GetChildren(&numChildren)))
	{
		return;
	}
	for (SIZE_T i = 0; i < numChildren; ++i)
	{
		ID3DXFileData* lpXFileSubData;
		if(FAILED(lpXFileData->GetChild(i, &lpXFileSubData)))
		{
			return;
		}
		parseXFileData(lpXFileSubData, depth + 1);
		lpXFileSubData->Release();
	}
}