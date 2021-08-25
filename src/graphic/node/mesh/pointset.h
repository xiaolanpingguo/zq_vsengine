#pragma once
#include "graphic/node/mesh/meshdata.h"
#include "math/vector3.h"
namespace zq
{
	class GRAPHIC_API VSPointSet : public VSMeshData
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPointSet();
		VSPointSet(const Vector3 & Point,float fSize);
		virtual ~VSPointSet();
		bool CreateIndex();
		virtual uint32 GetTotalNum()const;
		virtual uint32 GetGirdNum(uint32 uiInputNum)const;


	
	
	};
	DECLARE_Ptr(VSPointSet);
	VSTYPE_MARCO(VSPointSet);

}
