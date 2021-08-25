#include "core/log.h"


namespace zq{


Log::Log()
{


}
/********************************************************************************/
Log::~Log()
{

}
/********************************************************************************/
bool Log::Open(const char * pFileName)
{
	return File::Open(pFileName,OM_WT);
}
/********************************************************************************/
bool Log::WriteInfo(const char *pcString)
{
	Write(pcString, VSStrLen(pcString) + 1, 1);
	return 1;
}

}