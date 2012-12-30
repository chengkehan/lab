#ifndef __JC_XFILE_BONE_H__
#define __JC_XFILE_BONE_H__

#include <Windows.h>
#include <rmxfguid.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "JCXFile.h"
#include "JCXFrame.h"
#include "jcxcommon.h"
#include "jccommon.h"
#include "jcstring.h"

class JCXFileBone : public JCXFile
{
public:
	JCXFileBone();
	~JCXFileBone();

	JCXFrame* getRootBone();
	JCXFrame* findBone(LPCSTR lpBoneName);
	static BOOL updateHierarchy(JCXFrame* lpBone, D3DXMATRIX* lpTransform = NULL);
	static BOOL resetHierarchy(JCXFrame* lpBone);

protected:
	virtual VOID* parseChild(ID3DXFileData* lpXFileData, BOOL isReference, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid);
	virtual VOID cleanUp();

private:
	JCXFrame* m_boneRoot;
	JCXFileBone(CONST JCXFileBone& value);
	JCXFrame* fineBoneRecursion(LPCSTR lpBoneName, JCXFrame* lpBone);
};

#endif