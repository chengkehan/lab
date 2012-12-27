#ifndef __JC_XFILE_H__
#define __JC_XFILE_H__

#include <Windows.h>
#include <fstream>
#include <d3dx9xof.h>

#include "jccommon.h"
#include "jcxcommon.h"
#include "jcstring.h"

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

class JCXFile
{
public:
	JCXFile();
	~JCXFile();

	BOOL parse(LPCSTR lpXFileData);
	static BOOL readXFile(LPCSTR file, CHAR* lpFileData, UINT* lpFileDataBytes);

protected:
	virtual VOID* parseChild(ID3DXFileData* lpXFileData, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid) = 0;
	virtual VOID cleanUp() = 0;

private:
	JCXFile(CONST JCXFile& value);

	VOID parseChildren(ID3DXFileData* lpXFileData, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent);
};

#endif