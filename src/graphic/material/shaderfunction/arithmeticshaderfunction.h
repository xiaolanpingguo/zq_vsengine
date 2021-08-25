#pragma once
#include "graphic/render/shaderfunction.h"
namespace zq
{
	class VSStream;

	class GRAPHIC_API VSMul : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSMul(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSMul();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool ResetValueType(MaterialShaderPara &MSPara);
	protected:
		VSMul();
	};
	DECLARE_Ptr(VSMul);
	VSTYPE_MARCO(VSMul);


	class GRAPHIC_API VSDot : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSDot(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSDot();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool ResetValueType(MaterialShaderPara &MSPara);
	protected:
		VSDot();
	};
	DECLARE_Ptr(VSDot);
	VSTYPE_MARCO(VSDot);

	class GRAPHIC_API VSLength : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSLength(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSLength();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool ResetValueType(MaterialShaderPara &MSPara);
	protected:
		VSLength();
	};
	DECLARE_Ptr(VSLength);
	VSTYPE_MARCO(VSLength);

	class GRAPHIC_API VSSub : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSSub(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSSub();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool ResetValueType(MaterialShaderPara &MSPara);
	protected:
		VSSub();
	};
	DECLARE_Ptr(VSSub);
	VSTYPE_MARCO(VSSub);

	class GRAPHIC_API VSAdd : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSAdd(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSAdd();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool ResetValueType(MaterialShaderPara &MSPara);
	protected:
		VSAdd();
	};
	DECLARE_Ptr(VSAdd);
	VSTYPE_MARCO(VSAdd);

	class GRAPHIC_API VSMakeValue : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSMakeValue(const VSUsedName & ShowName, VSMaterial * pMaterial,uint32 uiInputNum);
		virtual ~VSMakeValue();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool ResetValueType(MaterialShaderPara &MSPara);
	protected:
		VSMakeValue();
	};
	DECLARE_Ptr(VSMakeValue);
	VSTYPE_MARCO(VSMakeValue);

	class GRAPHIC_API VSDiv : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSDiv(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSDiv();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool ResetValueType(MaterialShaderPara &MSPara);
	protected:
		VSDiv();
	};
	DECLARE_Ptr(VSDiv);
	VSTYPE_MARCO(VSDiv);


	class GRAPHIC_API VSSaturate : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSSaturate(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSSaturate();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool ResetValueType(MaterialShaderPara &MSPara);
	protected:
		VSSaturate();
	};
	DECLARE_Ptr(VSSaturate);
	VSTYPE_MARCO(VSSaturate);
}
