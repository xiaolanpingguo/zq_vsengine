#pragma once
#include "graphic/controller/animtree/animbasefunction.h"
namespace zq
{
	class GRAPHIC_API VSAnimMainFunction : public VSAnimBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~VSAnimMainFunction();
		virtual bool Update(double dAppTime);
		VSAnimMainFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree);
		const VSAnimAtom &GetRootDelta();
		virtual bool IsSupportSimpleInstance();
	protected:
		VSAnimMainFunction();


	};
	DECLARE_Ptr(VSAnimMainFunction);
	VSTYPE_MARCO(VSAnimMainFunction);
}
