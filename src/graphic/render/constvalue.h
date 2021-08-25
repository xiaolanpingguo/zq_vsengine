#pragma once
#include "math/maths.h"
#include "graphic/render/shaderfunction.h"
#include "graphic/material/shader/userconstant.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSConstValue : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:

		VSConstValue(const VSUsedName & ShowName,VSMaterial * pMaterial,uint32 uiValueNum,bool bIsCustom);
		virtual ~VSConstValue();
		virtual uint32 GetType() = 0;
		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const = 0;
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const = 0;
		inline bool IsCustom()const
		{
			return m_bIsCustom;
		}
		virtual bool GetDeclareString(VSString &OutString, uint32 uiRegisterID, MaterialShaderPara &MSPara)const = 0;

		virtual uint32 GetValueNum()const = 0;

		virtual uint32 GetSize()const  = 0;
	protected:
		VSConstValue();
		bool m_bIsCustom;
		
	};
	DECLARE_Ptr(VSConstValue);
	VSTYPE_MARCO(VSConstValue);
	
	

}
