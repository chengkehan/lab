#include "common.h"

CHAR printBuffer[256];

BOOL readXFile(LPCSTR file, CHAR** lplpFileData, INT* lpFileDataBytes)
{
	std::ifstream reader;
	reader.open(file, std::ios_base::in);
	BOOL r = FALSE;
	if(reader.good())
	{
		reader.seekg(0, std::ios_base::end);
		INT bytes = (INT)reader.tellg();
		reader.seekg(0, std::ios_base::beg);
		if(lpFileDataBytes != NULL)
		{
			*lpFileDataBytes = bytes;
		}
		if(lplpFileData != NULL)
		{
			ZeroMemory(*lplpFileData, bytes);
			reader.read(*lplpFileData, bytes);
		}

		r = TRUE;
	}

	reader.close();
	return r;
}