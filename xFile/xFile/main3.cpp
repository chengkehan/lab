#include <Windows.h>
#include <dxfile.h>
#include <d3dx9xof.h>
#include <DxErr.h>
#include <cstdio>
#include <InitGuid.h>
#include <rmxftmpl.h>
#include <rmxfguid.h>
#include <fstream>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3dxof.lib")
#pragma comment(lib, "DxErr.lib")

using namespace std;

CONST INT PRINT_BUFFER_SIZE = 256;
CHAR printBuffer[PRINT_BUFFER_SIZE];
#define trace(format, value) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value); OutputDebugStringA(printBuffer); }
#define trace2(format, value1, value2) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value1, value2); OutputDebugStringA(printBuffer); }
#define trace3(format, value1, value2, value3) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value1, value2, value3); OutputDebugStringA(printBuffer); }

#define DXvBegin(hr) if(FAILED(hr)){ DXTRACE_ERR_MSGBOX(DXGetErrorDescription(hr), hr); DXTRACE_ERR_MSGBOX(DXGetErrorString(hr), hr); DXTRACE_MSG(DXGetErrorDescription(hr)); DXTRACE_ERR(DXGetErrorDescription(hr), hr); 
#define DXvEnd }
#define DXreleaseCom(com) if(com != NULL){ com->Release(); com = NULL; }

VOID parseXFile(ID3DXFile* lpXFile);
VOID parseXFileData(ID3DXFileData* lpXFileData, INT depth);

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	char* xFile = NULL;
	ifstream xFileReader;
	xFileReader.open("Warrior.x", ios_base::in);
	if(xFileReader.good())
	{
		xFileReader.seekg(0, ios_base::end);
		INT xFileSize = (INT)xFileReader.tellg();
		xFileReader.seekg(0, ios_base::beg);
		xFile = new CHAR[xFileSize];
		ZeroMemory(xFile, xFileSize);
		xFileReader.read(xFile, xFileSize);
	}
	else
	{
		goto finallyDo;
	}

	ID3DXFile* lpXFile;
	DXvBegin(D3DXFileCreate(&lpXFile))
		goto finallyDo;
	DXvEnd

	DXvBegin(lpXFile->RegisterTemplates(D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES))
		goto finallyDo;
	DXvEnd

	DXvBegin(lpXFile->RegisterTemplates(xFile, strlen(xFile)))
		goto finallyDo;
	DXvEnd

	parseXFile(lpXFile);

finallyDo:
	delete []xFile;
	xFileReader.close();
	DXreleaseCom(lpXFile);

	return 0;
}

VOID parseXFile(ID3DXFile* lpXFile)
{
	if(lpXFile == NULL)
	{
		return;
	}

	ID3DXFileEnumObject* lpEnum = NULL;
	DXvBegin(lpXFile->CreateEnumObject("Warrior.x", D3DXF_FILELOAD_FROMFILE, &lpEnum))
		goto finallyDo;
	DXvEnd

	SIZE_T numChildren;
	DXvBegin(lpEnum->GetChildren(&numChildren))
		goto finallyDo;
	DXvEnd

	for (SIZE_T i = 0; i < numChildren; ++i)
	{
		ID3DXFileData* lpXFileData = NULL;
		DXvBegin(lpEnum->GetChild(i, &lpXFileData))
			goto finallyDo;
		DXvEnd
		parseXFileData(lpXFileData, 0);
		DXreleaseCom(lpXFileData);
	}

finallyDo:
	DXreleaseCom(lpEnum);
}

VOID parseXFileData(ID3DXFileData* lpXFileData, INT depth)
{
	if(lpXFileData == NULL || lpXFileData->IsReference())
	{
		return;
	}

	SIZE_T nameSize;
	CHAR* lpName = NULL;
	DXvBegin(lpXFileData->GetName(NULL, &nameSize))
		goto finallyDo;
	DXvEnd

	lpName = new CHAR[nameSize];
	DXvBegin(lpXFileData->GetName(lpName, &nameSize))
		goto finallyDo;
	DXvEnd

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
	DXvBegin(lpXFileData->GetType(&guid))
		goto finallyDo;
	DXvEnd

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

	SIZE_T numChildren;
	DXvBegin(lpXFileData->GetChildren(&numChildren))
		goto finallyDo;
	DXvEnd

	for (SIZE_T i = 0; i < numChildren; ++i)
	{
		ID3DXFileData* lpXFileSubData = NULL;
		DXvBegin(lpXFileData->GetChild(i, &lpXFileSubData))
			goto finallyDo;
		DXvEnd
		parseXFileData(lpXFileSubData, depth + 1);
		DXreleaseCom(lpXFileSubData);
	}

finallyDo:
	delete []lpName;
	delete []lpTab;
}
