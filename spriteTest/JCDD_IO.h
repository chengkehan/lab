#ifndef __JCDD_IO_H__
#define __JCDD_IO_H__

#include <Windows.h>
#include <fstream>

namespace JCDD_NS
{
	typedef struct JCDD_File
	{
		LPWCH filePath;
		CHAR* fileData;
	} JCDD_File, *LPJCDD_File;

	BOOL jcdd_loadFile(LPWCH filePath, LPJCDD_File lpjcddFile);
	VOID jcdd_unloadFile(LPJCDD_File* lplpjcddFile);
};

#endif