#pragma once
#include "graphic/core/object.h"
#include "graphic/material/shaderfunction/shadermap.h"
#include "graphic/material/shader/shaderkey.h"
#include "graphic/material/shader/pshader.h"
#include "graphic/material/shader/vshader.h"
#include "graphic/material/shader/gshader.h"
#include "graphic/material/shader/dshader.h"
#include "graphic/material/shader/hshader.h"
#include "graphic/material/shader/cshader.h"
#include "math/matrix3X3w.h"
namespace zq
{
	class VSSpatial;
	class VSCamera;
	class GRAPHIC_API VSPass : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
		enum //pass type
		{
			PT_MATERIAL,
			PT_INDIRECT,
			PT_NORMALDEPTH,
			PT_PREZ,
			PT_POINT_CUBE_SHADOW,		
			PT_SHADOW,
			PT_DUAL_PARABOLOID_SHADOW,

			PT_POINT_VOLUME_SHADOW,
			PT_DIRECT_VOLUME_SHADOW,

			PT_LIGHT_FUNCTION,			
			PT_POSTEFFECT,
			PT_MAX
		};
	public:
		virtual ~VSPass() = 0;
	protected:
		VSPass();
		VSShaderSet * m_pVShaderSet;
		VSShaderKey m_VShaderkey;

		VSShaderSet * m_pPShaderSet;
		VSShaderKey m_PShaderkey;

		VSShaderSet * m_pGShaderSet;
		VSShaderKey m_GShaderkey;

		VSShaderSet * m_pHShaderSet;
		VSShaderKey m_HShaderkey;

		VSShaderSet * m_pDShaderSet;
		VSShaderKey m_DShaderkey;
		MaterialShaderPara MSPara;
	public:
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, uint32 uiPassId)
		{
			MSPara.pCamera = pCamera;
			MSPara.pGeometry = pGeometry;
			MSPara.pMaterialInstance = pMaterialInstance;
			MSPara.uiPassId = uiPassId;
			MSPara.uiPassType = GetPassType();
		}
		virtual bool Draw(VSRenderer * pRenderer) = 0;
		virtual uint32 GetPassType() = 0;

		bool GetPShader(VSShaderMap & ShaderMap,const VSUsedName &Name);
		bool GetVShader(VSShaderMap & ShaderMap,const VSUsedName &Name);
		bool GetGShader(VSShaderMap & ShaderMap, const VSUsedName &Name);
		bool GetHShader(VSShaderMap & ShaderMap, const VSUsedName &Name);
		bool GetDShader(VSShaderMap & ShaderMap, const VSUsedName &Name);
	};
	DECLARE_Ptr(VSPass);
	VSTYPE_MARCO(VSPass);
}
