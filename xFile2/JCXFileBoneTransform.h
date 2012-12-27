#ifndef __JC_XFILE_BONE_TRANSFORM_H__
#define __JC_XFILE_BONE_TRANSFORM_H__

#include <Windows.h>
#include <rmxfguid.h>

#include "JCXFileBone.h"
#include "JCXFrame.h"
#include "JCXFile.h"
#include "jccommon.h"
#include "jcxcommon.h"

class JCXFileBoneTransform : public JCXFile
{
public:
	JCXFileBoneTransform(JCXFileBone* lpXFileBone);
	~JCXFileBoneTransform();

protected:
	virtual VOID* parseChild(ID3DXFileData* lpXFileData, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid);
	virtual VOID cleanUp();

private:
	JCXFileBoneTransform();
	JCXFileBoneTransform(CONST JCXFileBoneTransform* value);

	JCXFileBone* m_lpXFileBone;
};

#endif