#ifndef __JC_XFILE_ANIMATION_H__
#define __JC_XFILE_ANIMATION_H__

#include <Windows.h>
#include <rmxfguid.h>
#include <d3dx9.h>

#include "JCXFile.h"
#include "JCXFileBone.h"
#include "JCAnimationSet.h"
#include "JCAnimation.h"
#include "JCXFrame.h"
#include "jccommon.h"
#include "jcxcommon.h"
#include "jcstring.h"

class JCXFileAnimation : public JCXFile
{
public:
	JCXFileAnimation(JCXFileBone* lpXFileBone);
	~JCXFileAnimation();

	BOOL play(LPCSTR lpAnimationSetName, DWORD time);

protected:
	virtual VOID* parseChild(ID3DXFileData* lpXFileData, BOOL isReference, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid);
	virtual VOID cleanUp();

private:
	JCXFileAnimation();
	JCXFileAnimation(CONST JCXFileAnimation& value);

	JCAnimationSet* m_lpAnimationSetRoot;
	JCXFileBone* m_lpXFileBone;
};

#endif