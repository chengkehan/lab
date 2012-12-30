#ifndef __JC_XFILE_SKINMESH_H__
#define __JC_XFILE_SKINMESH_H__

#include <Windows.h>
#include <rmxfguid.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "JCXFile.h"
#include "JCXMeshContainer.h"
#include "JCXFileBone.h"
#include "JCXFrame.h"
#include "jccommon.h"
#include "jcxcommon.h"

class JCXFileSkinMesh : public JCXFile
{
public:
	JCXFileSkinMesh(IDirect3DDevice9* lpd3dd, JCXFileBone* lpXFileBone);
	~JCXFileSkinMesh();

	JCXMeshContainer* getMeshContainerHeadNode();

protected:
	virtual VOID* parseChild(ID3DXFileData* lpXFileData, BOOL isReference, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid);
	virtual VOID cleanUp();

private:
	JCXFileSkinMesh();
	JCXFileSkinMesh(CONST JCXFileSkinMesh& value);

	JCXMeshContainer* m_lpMeshContainerHeadNode;
	IDirect3DDevice9* m_lpd3dd;
	JCXFileBone* m_lpXFileBone;
};

#endif