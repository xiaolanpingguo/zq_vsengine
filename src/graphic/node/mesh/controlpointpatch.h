#pragma once
#include "graphic/node/mesh/meshdata.h"

namespace zq
{



	class GRAPHIC_API VSControlPointPatch : public VSMeshData
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSControlPointPatch();
		virtual ~VSControlPointPatch();

		virtual uint32 GetTotalNum()const;
		virtual uint32 GetMeshDataType(){ return MDT_TRIANGLE; }
		virtual uint32 GetActiveNum()const;
		virtual uint32 GetGirdNum(uint32 uiInputNum)const;


	};
	DECLARE_Ptr(VSControlPointPatch);
	VSTYPE_MARCO(VSControlPointPatch);

}
