#pragma once


#include "core/file.h"


namespace zq{


class SYSTEM_API Log : public File
{
public:
	Log();
	~Log();
	bool Open(const char * pFileName);
	bool WriteInfo(const char *pcString);

};
}
