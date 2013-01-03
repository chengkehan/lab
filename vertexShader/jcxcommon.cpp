#include "jcxcommon.h"

VOID jcxcommon_printBones(JCXFrame* lpBone, UINT depth)
{
	if(lpBone == NULL)
	{
		return;
	}

	jcxcommon_printBones((JCXFrame*)lpBone->pFrameSibling, depth);

	CHAR* lpTabStr = NULL;
	if(depth > 0)
	{
		lpTabStr = new CHAR[depth + 1];
		memset(lpTabStr, '	', depth);
		lpTabStr[depth] = '\0';
	}
	jccommon_trace2("%s%s\n", lpTabStr == NULL ? "" : lpTabStr, lpBone->Name == NULL ? "" : lpBone->Name);

	jcxcommon_printBones((JCXFrame*)lpBone->pFrameFirstChild, depth + 1);
}