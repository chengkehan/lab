#include "common.h"
#include "D3DXFrameEx.h"
#include "D3DXMeshContainerEx.h"
#include "jcd3d.h"
#include "jcwin32.h"

using namespace jcd3d;
using namespace jcwin32;

D3DXFrameEx* lpBoneRoot = NULL;
D3DXMeshContainerEx* lpMeshContainerRoot = NULL;

VOID parseXFile(ID3DXFile* lpXFile, CHAR* lpFileChar);
VOID parseXFileData(ID3DXFileData* lpXFileData, ID3DXFileData* lpXFileDataParent, D3DXFrameEx* lpFrameParent);
VOID printBone(D3DXFrameEx* lpBone, INT depth);
D3DXFrameEx* findBone(D3DXFrameEx* lpBone, LPCSTR boneName);
VOID updateHierarchy(D3DXFrameEx* bone, D3DXMATRIX* lpTransform);

BOOL jcd3d::jcd3d_setup()
{
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, TRUE);
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, 800, 600);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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

	parseXFile(lpXFile, lpFileChar);
	printBone(lpBoneRoot, 0);
	CHAR* boneName = "Bip01_Neck";
	D3DXFrameEx* neckBone = findBone(lpBoneRoot, boneName);
	if(neckBone != NULL)
	{
		trace("get the bone %s\n", boneName);
	}
	else
	{
		trace("cannot find the bone %s\n", boneName);
	}

finallyDo:
	delete []lpFileChar; lpFileChar = NULL;
	DXreleaseCom(lpXFile);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	D3DXMATRIX matrixRoot;
	D3DXMatrixIdentity(&matrixRoot);
	D3DXMatrixScaling(&matrixRoot, 0.5f, 0.5f, 0.5f);
	updateHierarchy(lpBoneRoot, &matrixRoot);

	D3DXMeshContainerEx* lpMeshContainer = lpMeshContainerRoot;
	while(lpMeshContainer != NULL)
	{
		for (DWORD boneIndex = 0; boneIndex < lpMeshContainer->pSkinInfo->GetNumBones(); ++boneIndex)
		{
			lpMeshContainer->lpBoneMatrices[boneIndex] = *lpMeshContainer->pSkinInfo->GetBoneOffsetMatrix(boneIndex);
			if(lpMeshContainer->lplpFrameMatrices[boneIndex] != NULL)
			{
				lpMeshContainer->lpBoneMatrices[boneIndex] *= (*lpMeshContainer->lplpFrameMatrices[boneIndex]);
			}
		}

		VOID* lpSrc, *lpDest;
		DXvBegin(lpMeshContainer->lpSkinMesh->LockVertexBuffer(D3DLOCK_READONLY, &lpSrc))DXvEnd
		DXvBegin(lpMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &lpDest))DXvEnd
		DXvBegin(lpMeshContainer->pSkinInfo->UpdateSkinnedMesh(lpMeshContainer->lpBoneMatrices, NULL, lpSrc, lpDest))DXvEnd
		DXvBegin(lpMeshContainer->lpSkinMesh->UnlockVertexBuffer())DXvEnd
		DXvBegin(lpMeshContainer->MeshData.pMesh->UnlockVertexBuffer())DXvEnd

		for (DWORD materialIndex = 0; materialIndex < lpMeshContainer->NumMaterials; ++materialIndex)
		{
			DXvBegin(jcd3d_lpd3dd->SetMaterial(&lpMeshContainer->pMaterials[materialIndex].MatD3D))DXvEnd
			DXvBegin(jcd3d_lpd3dd->SetTexture(0, lpMeshContainer->lplpTextures[materialIndex]))DXvEnd
			DXvBegin(lpMeshContainer->MeshData.pMesh->DrawSubset(materialIndex))DXvEnd
		}
		trace("%d\n", lpMeshContainer->NumMaterials);

		lpMeshContainer = (D3DXMeshContainerEx*)lpMeshContainer->pNextMeshContainer;
	}

	static FLOAT angle = 0.0f;
	static FLOAT radius = 50.0f;
	jcd3d_setViewTransform(jcd3d_lpd3dd, cosf(angle) * radius, 0.0f, sinf(angle) * radius, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	angle += 0.05f;
}

VOID jcd3d::jcd3d_release()
{
	delete lpBoneRoot; lpBoneRoot = NULL;
	delete lpMeshContainerRoot; lpMeshContainerRoot = NULL;
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}

VOID parseXFile(ID3DXFile* lpXFile, CHAR* lpFileChar)
{
	if(lpXFile == NULL)
	{
		return;
	}

	D3DXF_FILELOADMEMORY fileData;
	fileData.lpMemory = lpFileChar;
	fileData.dSize = strlen(lpFileChar);

	ID3DXFileEnumObject* lpEnum = NULL;
	DXvBegin(lpXFile->CreateEnumObject(&fileData, D3DXF_FILELOAD_FROMMEMORY, &lpEnum))
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
		parseXFileData(lpXFileData, NULL, lpBoneRoot);
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
			lpFrameThis->pFrameSibling = lpBoneRoot;
			lpBoneRoot = lpFrameThis;
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

	if(guid == TID_D3DRMMesh)
	{
		ID3DXBuffer* lpAdjacency = NULL;
		ID3DXBuffer* lpMaterials = NULL;
		DWORD numMaterials = 0;
		ID3DXSkinInfo* lpSkinInfo = NULL;
		ID3DXMesh* lpMesh = NULL;
		DXvBegin(D3DXLoadSkinMeshFromXof(lpXFileData, D3DXMESH_SYSTEMMEM, jcd3d_lpd3dd, &lpAdjacency, &lpMaterials, NULL, &numMaterials, &lpSkinInfo, &lpMesh))
			goto finallyDo;
		DXvEnd

		if(lpSkinInfo == NULL || lpSkinInfo->GetNumBones() == 0)
		{
			jcwin32_messageBoxErrorM("Not a skin mesh");
			DXreleaseCom(lpAdjacency);
			DXreleaseCom(lpMaterials);
			goto finallyDo;
		}

		D3DXMeshContainerEx* lpMeshContainer = new D3DXMeshContainerEx();
		if(lpMeshContainerRoot == NULL)
		{
			lpMeshContainerRoot = lpMeshContainer;
		}
		else
		{
			lpMeshContainerRoot->pNextMeshContainer = lpMeshContainer;
		}
		lpMeshContainer->MeshData.pMesh = lpMesh;
		lpMeshContainer->NumMaterials = numMaterials;
		lpMeshContainer->pSkinInfo = lpSkinInfo;
		lpMeshContainer->pMaterials = new D3DXMATERIAL[numMaterials];
		lpMeshContainer->lplpTextures = new IDirect3DTexture9*[numMaterials];
		D3DXMATERIAL* lpMaterialsBufferData = (D3DXMATERIAL*)lpMaterials->GetBufferPointer();
		for (DWORD materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			lpMeshContainer->pMaterials[materialIndex].MatD3D = lpMaterialsBufferData[materialIndex].MatD3D;
			lpMeshContainer->pMaterials[materialIndex].MatD3D.Ambient = lpMeshContainer->pMaterials[materialIndex].MatD3D.Diffuse;
			lpMeshContainer->lplpTextures[materialIndex] = NULL;
			if(lpMaterialsBufferData[materialIndex].pTextureFilename != NULL)
			{
				DXvBegin(D3DXCreateTextureFromFileA(jcd3d_lpd3dd, lpMaterialsBufferData[materialIndex].pTextureFilename, &lpMeshContainer->lplpTextures[materialIndex]))
					DXreleaseCom(lpAdjacency);
					DXreleaseCom(lpMaterials);
					goto finallyDo;
				DXvEnd
			}
		}
		lpMeshContainer->pAdjacency = new DWORD[lpAdjacency->GetBufferSize()];
		memcpy(lpMeshContainer->pAdjacency, lpAdjacency->GetBufferPointer(), lpAdjacency->GetBufferSize());

		DXvBegin(lpMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, lpMeshContainer->MeshData.pMesh->GetFVF(), jcd3d_lpd3dd, &lpMeshContainer->lpSkinMesh))
			DXreleaseCom(lpAdjacency);
			DXreleaseCom(lpMaterials);
			goto finallyDo;
		DXvEnd

		DXreleaseCom(lpAdjacency);
		DXreleaseCom(lpMaterials);

		lpMeshContainer->lplpFrameMatrices = new D3DXMATRIX*[lpMeshContainer->pSkinInfo->GetNumBones()];
		lpMeshContainer->lpBoneMatrices = new D3DXMATRIX[lpMeshContainer->pSkinInfo->GetNumBones()];
		for (DWORD boneIndex = 0; boneIndex < lpMeshContainer->pSkinInfo->GetNumBones(); ++boneIndex)
		{
			LPCSTR boneName = lpMeshContainer->pSkinInfo->GetBoneName(boneIndex);
			D3DXFrameEx* bone = findBone(lpBoneRoot, boneName);
			if(bone == NULL)
			{
				jcwin32_messageBoxErrorM("Cannot find the bone");
				trace("cannot fine the bone %s\n", boneName);
				goto finallyDo;
			}
			else
			{
				lpMeshContainer->lplpFrameMatrices[boneIndex] = &bone->matrixCombine;
			}
		}
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

D3DXFrameEx* findBone(D3DXFrameEx* lpBone, LPCSTR boneName)
{
	if(lpBone == NULL || boneName == NULL || lpBone->Name == NULL)
	{
		return NULL;
	}

	if(strcmp(lpBone->Name, boneName) == 0)
	{
		return lpBone;
	}

	D3DXFrameEx* lpR = findBone((D3DXFrameEx*)lpBone->pFrameSibling, boneName);
	if(lpR != NULL)
	{
		return lpR;
	}

	lpR = findBone((D3DXFrameEx*)lpBone->pFrameFirstChild, boneName);
	if(lpR != NULL)
	{
		return lpR;
	}

	return NULL;
}

VOID updateHierarchy(D3DXFrameEx* bone, D3DXMATRIX* lpTransform)
{
	if(bone == NULL || lpTransform == NULL)
	{
		return;
	}

	bone->matrixCombine = bone->TransformationMatrix * (*lpTransform);
	updateHierarchy((D3DXFrameEx*)bone->pFrameSibling, lpTransform);
	updateHierarchy((D3DXFrameEx*)bone->pFrameFirstChild, &bone->matrixCombine);
}