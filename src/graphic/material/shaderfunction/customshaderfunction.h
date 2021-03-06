#pragma once
#include "graphic/material/shaderfunction/shadermainfunction.h"
namespace zq
{
	// good for plastics
	class GRAPHIC_API VSCustomShaderFunction : public VSShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY

	public:
		enum
		{
			IN_EMISSIVE_COLOR,
			IN_NORMAL,
			IN_ALPHA,
			IN_CUSTOM,
			IN_DIFFUSE_COLOR,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_WORLD_OFFSET,
			IN_TESSELLATION_VALUE,
			IN_WORLD_DISPLACEMENT,
			IN_MAX
		};
		VSCustomShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSCustomShaderFunction();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual bool GetPShaderTreeString(VSString &OutString, MaterialShaderPara &MSPara);
		virtual bool GetInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual inline uint32 GetSMType()const
		{
			return SM_CUSTOM;
		}
		virtual VSInputNode * GetDiffuseNode()const
		{
			return GetInputNode(IN_DIFFUSE_COLOR);
		}
		virtual VSInputNode * GetNormalNode()const
		{
			return GetInputNode(IN_NORMAL);
		}
		virtual VSInputNode * GetWorldOffsetNode()const
		{
			return GetInputNode(IN_WORLD_OFFSET);
		}
		virtual VSInputNode * GetTessellationValueNode()const
		{
			return GetInputNode(IN_TESSELLATION_VALUE);
		}
		virtual VSInputNode * GetWorldDisplacementNode()const
		{
			return GetInputNode(IN_WORLD_DISPLACEMENT);
		}
		virtual VSInputNode * GetAlphaNode()const
		{
			return GetInputNode(IN_ALPHA);
		}
		virtual VSInputNode * GetEmissiveNode()const
		{
			return GetInputNode(IN_EMISSIVE_COLOR);
		}
		virtual VSInputNode * GetReflectMipNode()const
		{
			return GetInputNode(IN_REFLECT_MIP);
		}
		virtual VSInputNode * GetReflectPowNode()const
		{
			return GetInputNode(IN_REFLECT_POW);
		}
		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction, MaterialShaderPara &MSPara);

		virtual void CreatLightFunctionString(MaterialShaderPara &MSPara);

		VSString m_LightFunctionString;
		VSString m_CustomContentString;
		VSString m_CustomDefine;
	protected:
		VSCustomShaderFunction();

		DECLARE_INITIAL
	public:
		virtual bool PostLoad(VSStream* pStream);
	};
	DECLARE_Ptr(VSCustomShaderFunction);
	VSTYPE_MARCO(VSCustomShaderFunction);
}
