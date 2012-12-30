#include <rmxftmpl.h>

#include "JCXFile.h"

using namespace std;
using namespace jcstring;

JCXFile::JCXFile()
{

}

JCXFile::~JCXFile()
{

}

BOOL JCXFile::parse(LPCSTR lpXFileData)
{
	if(lpXFileData == NULL)
	{
		return FALSE;
	}

	cleanUp();

	ID3DXFile* lpXFile = NULL;
	xVerifyFailedIf(D3DXFileCreate(&lpXFile))
		jccommon_releaseComM(lpXFile);
		return FALSE;
	xVerifyFailedEndIf

	xVerifyFailedIf(lpXFile->RegisterTemplates(D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES))
		jccommon_releaseComM(lpXFile);
		return FALSE;
	xVerifyFailedEndIf

	xVerifyFailedIf(lpXFile->RegisterTemplates(lpXFileData, jcstring_cslen(lpXFileData)))
		jccommon_releaseComM(lpXFile);
		return FALSE;
	xVerifyFailedEndIf

	D3DXF_FILELOADMEMORY fileMemData;
	fileMemData.lpMemory = lpXFileData;
	fileMemData.dSize = jcstring_cslen(lpXFileData);

	ID3DXFileEnumObject* lpEnum = NULL;
	xVerifyFailedIf(lpXFile->CreateEnumObject(&fileMemData, D3DXF_FILELOAD_FROMMEMORY, &lpEnum))
		jccommon_releaseComM(lpXFile);
		return FALSE;
	xVerifyFailedEndIf

	SIZE_T numChildren;
	xVerifyFailedIf(lpEnum->GetChildren(&numChildren))
		jccommon_releaseComM(lpXFile);
		jccommon_releaseComM(lpEnum);
		return FALSE;
	xVerifyFailedEndIf

	for (SIZE_T childIndex = 0; childIndex < numChildren; ++childIndex)
	{
		ID3DXFileData* lpXFileData = NULL;
		xVerifyFailedIf(lpEnum->GetChild(childIndex, &lpXFileData))
			break;
		xVerifyFailedEndIf
		parseChildren(lpXFileData, FALSE, NULL, NULL);
		jccommon_releaseComM(lpXFileData);
	}

	jccommon_releaseComM(lpXFile);
	jccommon_releaseComM(lpEnum);

	return TRUE;
}

BOOL JCXFile::readXFile(LPCSTR file, CHAR* lpFileData, UINT* lpFileDataBytes)
{
	ifstream reader;
	reader.open(file, ios_base::in);
	BOOL r = FALSE;
	if(reader.good())
	{
		reader.seekg(0, ios_base::end);
		UINT bytes = (UINT)reader.tellg();
		reader.seekg(0, ios_base::beg);
		if(lpFileDataBytes != NULL)
		{
			*lpFileDataBytes = bytes;
		}
		if(lpFileData != NULL)
		{
			ZeroMemory(lpFileData, bytes);
			reader.read(lpFileData, bytes);
		}
		r = TRUE;
	}
	reader.close();

	return r;
}

VOID JCXFile::parseChildren(ID3DXFileData* lpXFileData, BOOL isReference, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent)
{
	if(lpXFileData == NULL)
	{
		return;
	}

	GUID guid;
	xVerifyFailedIf(lpXFileData->GetType(&guid))
		return;
	xVerifyFailedEndIf

	VOID* lpData = parseChild(lpXFileData, isReference || lpXFileData->IsReference(), lpXFileDataParent, lpDataParent, &guid);

	SIZE_T numChlidren;
	xVerifyFailedIf(lpXFileData->GetChildren(&numChlidren))
		return;
	xVerifyFailedEndIf
	for (SIZE_T childIndex = 0; childIndex < numChlidren; ++childIndex)
	{
		ID3DXFileData* lpChildXFileData = NULL;
		xVerifyFailedIf(lpXFileData->GetChild(childIndex, &lpChildXFileData))
			return;
		xVerifyFailedEndIf
		parseChildren(lpChildXFileData, isReference || lpXFileData->IsReference() || lpChildXFileData->IsReference(), lpXFileData, lpData);
		jccommon_releaseComM(lpChildXFileData);
	}
}