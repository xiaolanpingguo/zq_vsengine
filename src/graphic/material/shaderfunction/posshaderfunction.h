#pragma once
#include "graphic/render/shaderfunction.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSWorldPos : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSWorldPos(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSWorldPos();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
	protected:
		VSWorldPos();
	};
	DECLARE_Ptr(VSWorldPos);
	VSTYPE_MARCO(VSWorldPos);


	class GRAPHIC_API VSViewPos : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSViewPos(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSViewPos();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
	protected:
		VSViewPos();
	};
	DECLARE_Ptr(VSViewPos);
	VSTYPE_MARCO(VSViewPos);

	class GRAPHIC_API VSProjectPos : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSProjectPos(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSProjectPos();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
	protected:
		VSProjectPos();
	};
	DECLARE_Ptr(VSProjectPos);
	VSTYPE_MARCO(VSProjectPos);
}
