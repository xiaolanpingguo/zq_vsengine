#pragma once
#include "graphic/posteffect/posteffectfunction.h"
namespace zq
{
	class VSPEEndFunction : public VSPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		enum
		{
			INPUT_COLOR
		};
		VSPEEndFunction(const VSUsedName & ShowName,VSPostEffectSet * pPostEffectSet);
		
		~VSPEEndFunction();

	protected:
		VSPEEndFunction();
		virtual VSPostEffectSceneRender * CreateSceneRender();
		virtual void OnDraw(VSCuller & Culler,double dAppTime);
	};
	DECLARE_Ptr(VSPEEndFunction);
	VSTYPE_MARCO(VSPEEndFunction);

}
