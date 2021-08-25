#pragma once
#include "graphic/node/mesh/meshdata.h"

namespace zq
{

	

	class GRAPHIC_API VSTriangleSet : public VSMeshData
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSTriangleSet();
		virtual ~VSTriangleSet();
		bool CreateFanIndex();
		bool CreateNormal(uint32 uiVertexLevel = 0);
		bool CreateTangent(uint32 uiTexCoordLevel = 0);
		virtual uint32 GetTotalNum()const;
		virtual uint32 GetActiveNum()const;
		virtual uint32 GetGirdNum(uint32 uiInputNum)const;
		
	
	};
	DECLARE_Ptr(VSTriangleSet);
	VSTYPE_MARCO(VSTriangleSet);
	
}
