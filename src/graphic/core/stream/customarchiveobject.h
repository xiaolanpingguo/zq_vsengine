#pragma once


#include "graphic/core/graphic.h"
#include "graphic/core/type.h"

namespace zq{


	class VSObject;
	class VSStream;
	class VSEditorElement;
	//不支持指针序列化，不支持postload 和 postclone
	//类成员如果是VSType 是支持的
	class GRAPHIC_API VSCustomArchiveObject:public MemObject
	{
	public:
		VSCustomArchiveObject();
		virtual ~VSCustomArchiveObject();
		virtual void Archive(VSStream & Stream) = 0;
		virtual void CopyFrom(VSCustomArchiveObject *,VSMap<VSObject *,VSObject*>& CloneMap) = 0;
		virtual VSEditorElement *  CreateEElement(VSString& Name,VSObject * pOwner);
	};
	CUSTOMTYPE_MARCO(VSCustomArchiveObject)
}
