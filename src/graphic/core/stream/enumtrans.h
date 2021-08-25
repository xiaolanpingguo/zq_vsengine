#pragma once


#include "datastruct/VSString.h"
#include "datastruct/VSMap.h"
#include "datastruct/VSArray.h"
#include "graphic/core/graphic.h"
#include "stl/vector.h"
#include "stl/ustring.h"
#include "stl/map.h"


namespace zq{


class GRAPHIC_API VSEnumTrans
{
public:
	VSEnumTrans();
	~VSEnumTrans();
	static VSEnumTrans& GetInstance();
	void Add(const char* EnumName,const char* Value);
	void Get(const char* EnumName, VSArray<VSString>& AS);

protected:
	VSMap<VSString,VSArray<VSString>> m_EnumSaver;
};



#define ADD_ENUM(EnumName,Value) \
	VSEnumTrans::GetInstance().Add(_T(#EnumName),_T(#Value));
#define GET_ENUMARRAY(EnumName,Array) \
	VSEnumTrans::GetInstance().Get(_T(#EnumName),Array);


}


