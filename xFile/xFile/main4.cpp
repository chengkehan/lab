#include "common.h"
#include "D3DXFrameEx.h"

D3DXFrameEx* lpFrameRoot = NULL;

VOID parseXFile(ID3DXFile* lpXFile);
VOID parseXFileData(ID3DXFileData* lpXFileData, ID3DXFileData* lpXFileDataParent, D3DXFrameEx* lpFrameParent);
VOID printBone(D3DXFrameEx* lpBone, INT depth);

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	CHAR* lpFileChar = NULL;
	INT fileChars;
	if(!readXFile("Warrior.x", NULL, &fileChars))
	{
		goto finallyDo;
	}
	lpFileChar = new CHAR[fileChars];
	if(!readXFile("Warrior.x", &lpFileChar, NULL))
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
	printBone(lpFrameRoot, 0);

finallyDo:
	delete []lpFileChar;
	DXreleaseCom(lpXFile);
	delete lpFrameRoot;
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
		parseXFileData(lpXFileData, NULL, lpFrameRoot);
		DXreleaseCom(lpXFileData);
	}

finallyDo:
	DXreleaseCom(lpEnum);
}

VOID parseXFileData(ID3DXFileData* lpXFileData, ID3DXFileData* lpXFileDataParent, D3DXFrameEx* lpFrameParent)
{
	if(lpXFileData == NULL || lpXFileData->IsReference())
	{
		return;
	}

	GUID guid;
	DXvBegin(lpXFileData->GetType(&guid))
		goto finallyDo;
	DXvEnd

	D3DXFrameEx* lpFrameThis = NULL;
	if(guid == TID_D3DRMFrame)
	{
		lpFrameThis = new D3DXFrameEx();

		SIZE_T nameSize;
		CHAR* lpName = NULL;
		DXvBegin(lpXFileData->GetName(NULL, &nameSize))
			goto finallyDo;
		DXvEnd
		lpName = new CHAR[nameSize];
		DXvBegin(lpXFileData->GetName(lpName, &nameSize))
			delete []lpName;
			goto finallyDo;
		DXvEnd
		lpFrameThis->Name = lpName;

		if(lpXFileDataParent == NULL)
		{
			lpFrameThis->pFrameSibling = lpFrameRoot;
			lpFrameRoot = lpFrameThis;
		}
		else
		{
			lpFrameThis->pFrameSibling = lpFrameParent->pFrameFirstChild;
			lpFrameParent->pFrameFirstChild = lpFrameThis;
		}
	}

	if(guid == TID_D3DRMFrameTransformMatrix)
	{
		SIZE_T dataSize;
		LPCVOID lpData = NULL;
		DXvBegin(lpXFileData->Lock(&dataSize, &lpData))
			goto finallyDo;
		DXvEnd

		lpFrameParent->TransformationMatrix = *(D3DXMATRIX*)lpData;
		lpFrameParent->matrixOriginal = lpFrameParent->TransformationMatrix;

		DXvBegin(lpXFileData->Unlock())
			goto finallyDo;
		DXvEnd
	}

	SIZE_T numChildren;
	DXvBegin(lpXFileData->GetChildren(&numChildren))
		goto finallyDo;
	DXvEnd
	for (SIZE_T i = 0; i < numChildren; ++i)
	{
		ID3DXFileData* lpXFileDataChild = NULL;
		DXvBegin(lpXFileData->GetChild(i, & lpXFileDataChild))
			goto finallyDo;
		DXvEnd
		parseXFileData(lpXFileDataChild, lpXFileData, lpFrameThis);
		DXreleaseCom(lpXFileDataChild);
	}

finallyDo:
	return;
}

VOID printBone(D3DXFrameEx* lpBone, INT depth)
{
	if(lpBone == NULL)
	{
		return;
	}

	printBone((D3DXFrameEx*)lpBone->pFrameSibling, depth);

	CHAR* lpTab = NULL;
	if(depth > 0)
	{
		lpTab = new CHAR[depth + 1];
		for (INT i = 0; i < depth; ++i)
		{
			lpTab[i] = '	';
		}
		lpTab[depth] = '\0';
	}
	trace2("%s%s\n", lpTab == NULL ? "" : lpTab, lpBone->Name);
	delete []lpTab;

	printBone((D3DXFrameEx*)lpBone->pFrameFirstChild, depth + 1);
}