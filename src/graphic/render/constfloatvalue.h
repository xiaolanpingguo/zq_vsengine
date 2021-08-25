#pragma once
#include "graphic/render/constvalue.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSConstFloatValue : public VSConstValue
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSConstFloatValue(const VSUsedName & ShowName,VSMaterial * pMaterial,uint32 uiValueNum,bool bIsCustom);
		virtual ~VSConstFloatValue();
		virtual uint32 GetType()
		{
			return VSUserConstant::VT_FLOAT;
		}
		void SetValue(uint32 uiIndex,float Value);

		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual uint32 GetValueNum()const
		{
			return m_Value.GetNum();
		}
		virtual uint32 GetSize()const
		{
			return m_Value.GetNum() * sizeof(float);
		}
		bool GetDeclareString(VSString &OutString, uint32 uiRegisterID, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		VSArray<float>& GetValue() { return m_Value;}
	protected:
		VSConstFloatValue();
		VSArray<float> m_Value;
	};
	DECLARE_Ptr(VSConstFloatValue);
	VSTYPE_MARCO(VSConstFloatValue);

}
