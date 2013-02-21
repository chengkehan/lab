#ifndef __JC_XCOMMON_H__
#define __JC_XCOMMON_H__

#include <Windows.h>
#include <DxErr.h>

#pragma comment(lib, "DxErr.lib")

#define xVerifyFailedIf(hr) if(FAILED(hr)){ DXTRACE_ERR_MSGBOX(DXGetErrorDescription(hr), hr); DXTRACE_ERR_MSGBOX(DXGetErrorString(hr), hr); DXTRACE_MSG(DXGetErrorDescription(hr)); DXTRACE_ERR(DXGetErrorDescription(hr), hr); 
#define xVerifyFailedEndIf }

#endif