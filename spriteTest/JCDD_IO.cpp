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
		if(istream.eof())
		{
			OutputDebugString(L"");
		}
		if(istream.bad())
		{
			OutputDebugString(L"");
		}
		if(istream.fail())
		{
			OutputDebugString(L"");
		}
		if(istream.good())
		{
			istream.seekg(0, std::ios::end);
			UINT fileSize = (UINT)istream.tellg();
			istream.seekg(0, std::ios::beg);

			data = (CHAR*)malloc(fileSize);
			if(data == NULL)
			{
				destroy();
				istream.close();
				return FALSE;
			}
			istream.read(data, fileSize);
		}
		else
		{
			destroy();
			istream.close();
			return FALSE;
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