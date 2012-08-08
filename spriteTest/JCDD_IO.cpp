#include "JCDD_IO.h"

namespace JCDD_NS
{
	BOOL jcdd_loadFile(LPWCH filePath, LPJCDD_File lpjcddFile)
	{
		if(filePath == NULL || lpjcddFile == NULL)
		{
			return FALSE;
		}

		lpjcddFile->filePath = filePath;
		lpjcddFile->fileData = NULL;

		std::ifstream istream;
		istream.open(filePath, std::ifstream::in | std::ifstream::binary);
		if(istream.good())
		{
			istream.seekg(0, std::ifstream::end);
			UINT fileSize = (UINT)istream.tellg();
			istream.seekg(0, std::ifstream::beg);

			lpjcddFile->fileData = (CHAR*)malloc(sizeof(CHAR) * fileSize);
			if(lpjcddFile->fileData == NULL)
			{
				return FALSE;
			}
		}
		istream.close();

		return TRUE;
	}

	VOID jcdd_unloadFile(LPJCDD_File* lplpjcddFile)
	{
		if(*lplpjcddFile != NULL)
		{
			(*lplpjcddFile)->filePath = NULL;
			free((*lplpjcddFile)->fileData);
			(*lplpjcddFile)->fileData = NULL;
			*lplpjcddFile = NULL;
		}
	}
};