#ifndef __COMMON_H__
#define __COMMON_H__

#include <Windows.h>
#include <dxfile.h>
#include <d3dx9xof.h>
#include <DxErr.h>
#include <cstdio>
#include <InitGuid.h>
#include <rmxftmpl.h>
#include <rmxfguid.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3dxof.lib")
#pragma comment(lib, "DxErr.lib")

CONST INT PRINT_BUFFER_SIZE = 256;
CHAR printBuffer[PRINT_BUFFER_SIZE];
#define trace(format, value) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value); OutputDebugStringA(printBuffer); }
#define trace2(format, value1, value2) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value1, value2); OutputDebugStringA(printBuffer); }
#define trace3(format, value1, value2, value3) { sprintf_s(printBuffer, PRINT_BUFFER_SIZE, format, value1, value2, value3); OutputDebugStringA(printBuffer); }

#define DXvBegin(hr) if(FAILED(hr)){ DXTRACE_ERR_MSGBOX(DXGetErrorDescription(hr), hr); DXTRACE_ERR_MSGBOX(DXGetErrorString(hr), hr); DXTRACE_MSG(DXGetErrorDescription(hr)); DXTRACE_ERR(DXGetErrorDescription(hr), hr); 
#define DXvEnd }
#define DXreleaseCom(com) if(com != NULL){ com->Release(); com = NULL; }

#endif