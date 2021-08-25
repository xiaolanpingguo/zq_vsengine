#pragma once


#include "math/maths.h"
#include "datastruct/VSString.h"
#include "datastruct/VSMap.h"
#include "core/memorymanager.h"
#include "core/system.h"


namespace zq{


class VSCommand : public MemObject
{
public:
    VSCommand(int32 numArguments, char** arguments);
    VSCommand(char* commandLine);
    ~VSCommand();

    bool GetName(const char* name);
    bool GetInteger(const char* name, int& value);
    bool GetReal(const char* name, float& value);
    bool GetString(const char* name, VSString & value);

protected:

    VSMap<VSString, VSString> m_CommandMap;
};


}
