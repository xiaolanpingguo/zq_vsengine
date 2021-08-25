#pragma once
#include "graphic/render/bind.h"
namespace zq
{
	class GRAPHIC_API VSQuery : public VSBind
	{

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		enum //QueryType
		{
			QT_OCCLUSION,
			QT_MAX
		};
		virtual ~VSQuery();
		VSQuery(uint32 uiQueryType);
		inline uint32 GetQueryType()const
		{
			return m_uiQueryType;
		}
		bool GetQueryData(void * pData, uint32 uiDataSize);
	protected:
		VSQuery();
		
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
	protected:
		uint32 m_uiQueryType;
	};
	DECLARE_Ptr(VSQuery);
	VSTYPE_MARCO(VSQuery);
}