#pragma once


#include "graphic/core/graphic.h"
#include "graphic/core/type.h"

namespace zq{


	class VSObject;
	class VSStream;
	class VSEditorElement;
	//��֧��ָ�����л�����֧��postload �� postclone
	//���Ա�����VSType ��֧�ֵ�
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
