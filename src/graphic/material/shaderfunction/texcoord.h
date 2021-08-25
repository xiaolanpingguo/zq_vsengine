#pragma once
#include "graphic/render/shaderfunction.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSTexCoord : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSTexCoord(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSTexCoord();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
	protected:
		VSTexCoord();
		uint32 m_uiTexCoordIndex;

	public:
		enum //TexCoord Index
		{
			TI_1,
			TI_2,
			TI_MAX
		};
		inline void SetTexCoordIndex(uint32 uiIndex)
		{
			if (uiIndex >= TI_MAX)
			{
				return;
			}
			m_uiTexCoordIndex = uiIndex;
		}
	};
	DECLARE_Ptr(VSTexCoord);
	VSTYPE_MARCO(VSTexCoord);
}
