#include "common.h"

VOID parseXFile(ID3DXFile* lpXFile);
VOID parseXFileData(ID3DXFileData* lpXFileData, INT depth);

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	char* lpFileChar = NULL;
	INT fileChars;
	if(!readXFile("Warrior.x", NULL, &fileChars))
	{
		goto finallyDo;
	}
	lpFileChar = new CHAR[fileChars];
	ZeroMemory(lpFileChar, fileChars);
	if(!readXFile("Warrior.x", &lpFileChar, &fileChars))
	{
		goto finallyDo;
	}

	ID3DXFile* lpXFile = NULL;
	DXvBegin(D3DXFileCreate(&lpXFile))
		goto finallyDo;
	DXvEnd

	DXvBegin(lpXFile->RegisterTemplates(D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES))
		goto finallyDo;
	DXvEnd

	DXvBegin(lpXFile->RegisterTemplates(lpFileChar, strlen(lpFileChar)))
		goto finallyDo;
	DXvEnd

	parseXFile(lpXFile);

finallyDo:
	delete []lpFileChar;
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
