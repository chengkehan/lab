#ifndef __JCDD_IO_H__
#define __JCDD_IO_H__

#include <Windows.h>

namespace JCDD_NS
{
	class JCDD_File
	{
	public:
		JCDD_File();
		~JCDD_File();

	public:
		BOOL loadData(LPWCH filePath);
		CHAR* getData();
		LPWCH getPath();

	private:
		LPWCH path;
		CHAR* data;
		VOID destroy();
	};
};

#endif