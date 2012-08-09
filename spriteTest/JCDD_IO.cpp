#include "JCDD_IO.h"
#include <fstream>
#include "JCDD_Util.h"

namespace JCDD_NS
{
	JCDD_File::JCDD_File():path(NULL), data(NULL)
	{

	}

	JCDD_File::~JCDD_File()
	{
		destroy();
	}

	LPWCH JCDD_File::getPath()
	{
		return path;
	}

	CHAR* JCDD_File::getData()
	{
		return data;
	}

	BOOL JCDD_File::loadData(LPWCH path)
	{
		if(path == NULL)
		{
			destroy();
			return FALSE;
		}

		this->path = path;
		this->data = NULL;

		std::ifstream istream;
		istream.open(path, std::ifstream::in | std::ifstream::binary);
		if(istream.good())
		{
			istream.seekg(0, std::ifstream::end);
			UINT fileSize = (UINT)istream.tellg();
			istream.seekg(0, std::ifstream::beg);

			data = (CHAR*)malloc(sizeof(CHAR) * fileSize);
			if(data == NULL)
			{
				destroy();
				return FALSE;
			}
		}
		istream.close();

		return TRUE;
	}

	VOID JCDD_File::destroy()
	{
		path = NULL;
		jcdd_free(&data);
	}
};