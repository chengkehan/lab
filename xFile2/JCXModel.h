#ifndef __JC_XMODEL_H__
#define __JC_XMODEL_H__

#include <Windows.h>
#include <d3d9.h>

#include "JCXFileBone.h"
#include "JCXFileBoneTransform.h"
#include "JCXFileSkinMesh.h"
#include "JCXFileAnimation.h"
#include "jccommon.h"

class JCXModel
{
public:
	JCXModel(IDirect3DDevice9* lpd3dd);
	~JCXModel();

	JCXFileBone* getBone();
	JCXFileSkinMesh* getSkinMesh();
	JCXFileAnimation* getAnimation();
	VOID draw();
	BOOL parse(LPCSTR lpXFileData);

private:
	JCXModel();
	JCXModel(CONST JCXModel& value);

	JCXFileBone* m_lpBone;
	JCXFileSkinMesh* m_lpSkinMesh;
	JCXFileAnimation* m_lpAnimation;
	IDirect3DDevice9* m_lpd3dd;

	VOID cleanUp();
};

#endif