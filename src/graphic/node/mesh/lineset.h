#pragma once
#include "graphic/node/mesh/meshdata.h"
namespace zq
{
	class GRAPHIC_API VSLineSet:public VSMeshData
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSLineSet();
		virtual ~VSLineSet();
		enum	//Line Type
		{
			LT_OPEN,
			LT_CLOSE,
			LT_SEGMENT,
			LT_MAX
		
		};
		bool CreateIndex(uint32 uiLineType);
		virtual uint32 GetTotalNum()const;

		virtual uint32 GetActiveNum()const;

		virtual uint32 GetGirdNum(uint32 uiInputNum)const;

		
	};
	DECLARE_Ptr(VSLineSet);
	VSTYPE_MARCO(VSLineSet);



}
