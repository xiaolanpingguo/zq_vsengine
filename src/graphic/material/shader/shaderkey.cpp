#include "graphic/material/shader/shaderkey.h"
#include "graphic/node/geometry.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/material/shaderfunction/orennayarshaderfunction.h"
#include "graphic/material/shaderfunction/phoneshaderfunction.h"
#include "graphic/core/stream/stream.h"
#include "graphic/node/nodecomponent/light/pointlight.h"
#include "graphic/node/nodecomponent/light/directionlight.h"
#include "graphic/node/nodecomponent/light/spotlight.h"
using namespace zq;
IMPLEMENT_RTTI(VSShaderKey, VSObject)
BEGIN_ADD_PROPERTY(VSShaderKey, VSObject)
REGISTER_PROPERTY(m_KeyMap, KeyMap, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSShaderKey)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSShaderKey> VSShaderKey::ms_NULLShaderKey = NULL;
bool VSShaderKey::InitialDefaultState()
{
	ms_NULLShaderKey = MEM_NEW VSShaderKey();
	return 1;
}
bool VSShaderKey::TerminalDefaultState()
{
	ms_NULLShaderKey = NULL;
	return 1;
}
uint32 VSShaderKey::GetLightNum(uint32 uiLightKey,uint32 uiLightType)
{
	uint32 uiNum = 0;
	if (uiLightType == VSLight::LT_MAX)
	{
		for (uint32 i = 0 ; i < VSLight::LT_MAX ; i++)
		{
			uiNum += (uiLightKey >> (i * MAX_LIGTH_TYPE_MASK)) & MAX_LIGHT_TYPE_NUM;
		}
	}
	else
	{
		uiNum += (uiLightKey >> (uiLightType * MAX_LIGTH_TYPE_MASK)) & MAX_LIGHT_TYPE_NUM;
	}
	return uiNum;
}
uint32 VSShaderKey::GenerateKey(const VSArray<VSLight*> & LightArray,ShadowKeyInfo ShadowInfo[VSLight::LT_MAX],uint32 &uiLightFunctionKey)
{
	uint32 uiLightNum[VSLight::LT_MAX] = { 0 };
	unsigned char DirLightFunKey = 0;
	unsigned char PointLightFunKey = 0;
	unsigned char SpotLightFunKey = 0;
	for (uint32 i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (uint32 j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{
					
					uiLightNum[j]++;

					VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
					if (pLocalLight && pLocalLight->HaveLightFun())
					{
						if (j == VSLight::LT_POINT)
						{
							PointLightFunKey |= 1 << uiLightNum[j];
						}
						else if (j == VSLight::LT_DIRECTION)
						{
							DirLightFunKey |= 1 << uiLightNum[j];
						}
						else if (j == VSLight::LT_SPOT)
						{
							SpotLightFunKey |= 1 << uiLightNum[j];
						}
					}
					if (pLocalLight && pLocalLight->GetCastShadow())
					{
						ShadowInfo[j].uiShadowNum++;
						if (j == VSLight::LT_POINT)
						{
							if(((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_VOLUME)
							{
								ShadowInfo[j].uiVolumeShadowNum++;
							}
							else if (((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_DUAL_PARABOLOID)
							{
								ShadowInfo[j].uiDualParaboloidShadowNum++;
							}
							else if (((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_PROJECT)
							{
								ShadowInfo[j].uiShadowNum--;
							}
						}
						else if (j == VSLight::LT_DIRECTION)
						{
							if(((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_VOLUME)
							{
								ShadowInfo[j].uiVolumeShadowNum++;
							}
							else if (((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_CSM)
							{
								ShadowInfo[j].uiCSMShadowNum++;
							}
							else if (((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_PROJECT)
							{
								ShadowInfo[j].uiShadowNum--;
							}
						}
						else if (j == VSLight::LT_SPOT)
						{
							if (((VSSpotLight*)pLocalLight)->GetShadowType() == VSSpotLight::ST_PROJECT)
							{
								ShadowInfo[j].uiShadowNum--;
							}
						}
					}
					break;
				}
			}
		}
	}
	uint32 uiKey = 0;
	for (uint32 i = 0 ; i < VSLight::LT_MAX ; i++)
	{
		uiKey += uiLightNum[i] << (i * MAX_LIGTH_TYPE_MASK);
	}
	uiLightFunctionKey  = (PointLightFunKey) | (SpotLightFunKey << (1 << MAX_LIGTH_TYPE_MASK)) | (DirLightFunKey << ((1 << MAX_LIGTH_TYPE_MASK) * 2 ));
	return uiKey;
}
void VSShaderKey::SetMaterialHShaderKey(VSShaderKey * pKey, MaterialShaderPara & MSPara)
{
	VSMAC_ASSERT(pKey);
	uint32 uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL || uiPassType == VSPass::PT_NORMALDEPTH
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW || uiPassType == VSPass::PT_POINT_VOLUME_SHADOW
		|| uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW || uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW || uiPassType == VSPass::PT_INDIRECT)
	{

		uint32 uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshData())
			{

				VSVertexFormat *pVertexFormat = VSResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshData()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(VSUsedName::ms_cMaterialVertexFormat, uiVertexFormatCode);
	}
	else if (uiPassType == VSPass::PT_PREZ)
	{
		unsigned uiValue = 0;
		if (MSPara.pGeometry)
		{

			if (MSPara.pGeometry->GetAffectBoneNum() > 0)
			{
				uiValue = 1;
			}

		}
		pKey->SetTheKey(VSUsedName::ms_cPreZBeUsedBone, uiValue);
	}
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		pKey->SetTheKey(VSUsedName::ms_cUseInstance, 1);
	}
	if (MSPara.bUseTessellation)
	{
		pKey->SetTheKey(VSUsedName::ms_cUseTessellation, 1);
	}
}

void VSShaderKey::SetMaterialDShaderKey(VSShaderKey * pKey, MaterialShaderPara & MSPara)
{
	VSMAC_ASSERT(pKey);
	uint32 uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL || uiPassType == VSPass::PT_NORMALDEPTH
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW || uiPassType == VSPass::PT_POINT_VOLUME_SHADOW
		|| uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW || uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW || uiPassType == VSPass::PT_INDIRECT)
	{

		uint32 uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshData())
			{

				VSVertexFormat *pVertexFormat = VSResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshData()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(VSUsedName::ms_cMaterialVertexFormat, uiVertexFormatCode);
	}
	else if (uiPassType == VSPass::PT_PREZ)
	{
		unsigned uiValue = 0;
		if (MSPara.pGeometry)
		{

			if (MSPara.pGeometry->GetAffectBoneNum() > 0)
			{
				uiValue = 1;
			}

		}
		pKey->SetTheKey(VSUsedName::ms_cPreZBeUsedBone, uiValue);
	}
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		pKey->SetTheKey(VSUsedName::ms_cUseInstance, 1);
	}
	if (MSPara.bUseTessellation)
	{
		pKey->SetTheKey(VSUsedName::ms_cUseTessellation, 1);
	}
}
void VSShaderKey::SetMaterialGShaderKey(VSShaderKey * pKey, MaterialShaderPara &MSPara)
{
	VSMAC_ASSERT(pKey);
	uint32 uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL || uiPassType == VSPass::PT_NORMALDEPTH
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW || uiPassType == VSPass::PT_POINT_VOLUME_SHADOW
		|| uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW || uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW || uiPassType == VSPass::PT_INDIRECT)
	{

		uint32 uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshData())
			{

				VSVertexFormat *pVertexFormat = VSResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshData()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(VSUsedName::ms_cMaterialVertexFormat, uiVertexFormatCode);
		if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
		{
			pKey->SetTheKey(VSUsedName::ms_cVolumeVertexFormat, 1);
		}
	}
	else if (uiPassType == VSPass::PT_PREZ)
	{
		unsigned uiValue = 0;
		if (MSPara.pGeometry)
		{

			if (MSPara.pGeometry->GetAffectBoneNum() > 0)
			{
				uiValue = 1;
			}

		}
		pKey->SetTheKey(VSUsedName::ms_cPreZBeUsedBone, uiValue);
	}
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		pKey->SetTheKey(VSUsedName::ms_cUseInstance, 1);
	}
}
void VSShaderKey::SetMaterialVShaderKey(VSShaderKey * pKey,MaterialShaderPara &MSPara)
{
	VSMAC_ASSERT(pKey); 

	uint32 uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL || uiPassType == VSPass::PT_NORMALDEPTH  
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW || uiPassType == VSPass::PT_POINT_VOLUME_SHADOW 
		|| uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW|| uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW || uiPassType == VSPass::PT_INDIRECT)
	{
	
		uint32 uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshData())
			{
				
				VSVertexFormat *pVertexFormat = VSResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshData()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}
						
			}
			
		}
		pKey->SetTheKey(VSUsedName::ms_cMaterialVertexFormat,uiVertexFormatCode);
		if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_GS))
		{
			pKey->SetTheKey(VSUsedName::ms_cVolumeVertexFormat, 1);
		}
	}
	else if (uiPassType == VSPass::PT_PREZ)
	{
		unsigned uiValue = 0;
		if (MSPara.pGeometry)
		{

			if (MSPara.pGeometry->GetAffectBoneNum() > 0)
			{
				uiValue = 1;
			}

		}
		pKey->SetTheKey(VSUsedName::ms_cPreZBeUsedBone,uiValue);
	}
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		pKey->SetTheKey(VSUsedName::ms_cUseInstance, 1);
		if (/*!VSRenderer::ms_pRenderer->IsSupportVShaderSampler() && */VSResourceManager::GetLerpSimpleInstanceAnim())
		{
			pKey->SetTheKey(VSUsedName::ms_cSimpleAnimInstanceLerp, 1);
		}
	}
	if (MSPara.bUseTessellation)
	{
		pKey->SetTheKey(VSUsedName::ms_cUseTessellation, 1);
	}
	if (MSPara.pGeometry->HasMorphTarget() && VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_VertexIDInShader))
	{
		pKey->SetTheKey(VSUsedName::ms_cGPUMorphTarget, 1);
	}
}

void VSShaderKey::SetMaterialPShaderKey(VSShaderKey * pKey,MaterialShaderPara & MSPara)
{
	VSMAC_ASSERT(pKey);
	uint32 uiPassType = MSPara.uiPassType;
	pKey->SetTheKey(VSUsedName::ms_cPassID,MSPara.uiPassId);
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		pKey->SetTheKey(VSUsedName::ms_cUseInstance, 1);
	}
	if (uiPassType == VSPass::PT_MATERIAL)
	{
		uint32 uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshData())
			{

				VSVertexFormat *pVertexFormat = VSResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshData()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}
		pKey->SetTheKey(VSUsedName::ms_cMaterialVertexFormat,uiVertexFormatCode);



		
		ShadowKeyInfo ShadowInfo[VSLight::LT_MAX];
		uint32 uiLightKey = 0;
		uint32 uiLightFunKey = 0;
		if (MSPara.LightArray.GetNum() > 0)
		{
			uiLightKey = GenerateKey(MSPara.LightArray,ShadowInfo,uiLightFunKey);
		}
		
		pKey->SetTheKey(VSUsedName::ms_cLightFunKey,uiLightFunKey);
		pKey->SetTheKey(VSUsedName::ms_cMaterialLightKey,uiLightKey);
		

		if (ShadowInfo[VSLight::LT_POINT].uiShadowNum > 0)
		{
			pKey->SetTheKey(VSUsedName::ms_cPointLightShadowNum,ShadowInfo[VSLight::LT_POINT].uiShadowNum);
			if (ShadowInfo[VSLight::LT_POINT].uiVolumeShadowNum > 0)
			{
				pKey->SetTheKey(VSUsedName::ms_cPointLightVolumeShadowNum,ShadowInfo[VSLight::LT_POINT].uiVolumeShadowNum);
			}
			else if (ShadowInfo[VSLight::LT_POINT].uiDualParaboloidShadowNum > 0)
			{
				pKey->SetTheKey(VSUsedName::ms_cPointLightParaboloidShadowNum,ShadowInfo[VSLight::LT_POINT].uiDualParaboloidShadowNum);
			}
			else
			{

				if(MSPara.pCamera->GetFov() <= AngleToRadian(90.0f))
				{
					pKey->SetTheKey(VSUsedName::ms_cCubShadowFov,1);
				}

			}
		}
		if (ShadowInfo[VSLight::LT_DIRECTION].uiShadowNum > 0)
		{
			pKey->SetTheKey(VSUsedName::ms_cDirectionLightShadowNum,ShadowInfo[VSLight::LT_DIRECTION].uiShadowNum);
			if (ShadowInfo[VSLight::LT_DIRECTION].uiVolumeShadowNum > 0)
			{
				pKey->SetTheKey(VSUsedName::ms_cDirectionLightVolumeShadowNum,ShadowInfo[VSLight::LT_DIRECTION].uiVolumeShadowNum);
			}
			else if (ShadowInfo[VSLight::LT_DIRECTION].uiCSMShadowNum > 0)
			{
				pKey->SetTheKey(VSUsedName::ms_cDirectionLightCSMShadowNum,ShadowInfo[VSLight::LT_DIRECTION].uiCSMShadowNum);
			}
		}
		if (ShadowInfo[VSLight::LT_SPOT].uiShadowNum > 0)
		{
			pKey->SetTheKey(VSUsedName::ms_cSpotLightShadowNum,ShadowInfo[VSLight::LT_SPOT].uiShadowNum);
		}
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		
		if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == VSShaderMainFunction::SM_PHONE)
		{
			VSPhoneShaderFunction * pPhoneShaderFunction = (VSPhoneShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
			if (pPhoneShaderFunction->GetSpecularType() == VSPhoneShaderFunction::ST_BlinnPhong)
			{
				pKey->SetTheKey(VSUsedName::ms_cBlinnPhong,1);
			}
		}
		else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == VSShaderMainFunction::SM_OREN_NAYAR)
		{
			VSOrenNayarShaderFunction * pOrenNayarShaderFunction = (VSOrenNayarShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
			if (pOrenNayarShaderFunction->UseLookUpTable())
			{
				pKey->SetTheKey(VSUsedName::ms_cOrenNayarLookUpTable,1);
			}		
		}
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		uint32 uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshData())
			{

				VSVertexFormat *pVertexFormat = VSResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshData()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(VSUsedName::ms_cMaterialVertexFormat,uiVertexFormatCode);
	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		bool bLighted = false;
		VSMeshNode * pMeshNode = MSPara.pGeometry->GetMeshNode();
		if (pMeshNode && pMeshNode->m_bLighted)
		{
			bLighted = true;
		}
		else if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
		{
			bLighted = true;
		}
		pKey->SetTheKey(VSUsedName::ms_cLighted,bLighted);

		uint32 uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshData())
			{

				VSVertexFormat *pVertexFormat = VSResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshData()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(VSUsedName::ms_cMaterialVertexFormat, uiVertexFormatCode);
	}
	else if (uiPassType == VSPass::PT_SHADOW || uiPassType == VSPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW || uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		uint32 uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshData())
			{

				VSVertexFormat *pVertexFormat = VSResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshData()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(VSUsedName::ms_cMaterialVertexFormat, uiVertexFormatCode);
	}
}

VSShaderKey::VSShaderKey()
{

}
VSShaderKey::~VSShaderKey()
{
	m_KeyMap.Clear();
}
bool VSShaderKey::IsHaveTheKey(const VSUsedName & Name,uint32 &uiKeyId)
{
	uiKeyId = m_KeyMap.Find(Name);
	if(uiKeyId == m_KeyMap.GetNum())
	{
		return false;
	}

	return true;
}
void VSShaderKey::SetTheKey(const VSUsedName & Name,uint32 Value)
{
	uint32 uiKeyId;
	if (IsHaveTheKey(Name,uiKeyId))
	{
		m_KeyMap[uiKeyId].Value = Value;
		return;
	}
	m_KeyMap.AddElement(Name,Value);
}
void VSShaderKey::GetDefine(VSMap<VSString,VSString> & Define)
{
	Define.Clear();
	for (uint32 i = 0 ; i < m_KeyMap.GetNum() ; i++)
	{
		MapElement<VSUsedName,unsigned int>& Element =  m_KeyMap[i];
		Define.AddElement(Element.Key.GetString(),IntToString(Element.Value));
	}
}
void VSShaderKey::Clear()
{
	m_KeyMap.Clear();
}
void VSShaderKey::operator =(const VSShaderKey &ShaderKey)
{
	m_KeyMap = ShaderKey.m_KeyMap;
}
namespace zq
{
	bool operator == (const VSShaderKey & Key1,const VSShaderKey & Key2)
	{

		if (Key1.m_KeyMap.GetNum() != Key2.m_KeyMap.GetNum())
		{
			return 0;
		}

		//????????
		for (uint32 i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<VSUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<VSUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Key != Element2.Key || Element1.Value != Element2.Value)
			{
				return false;

			}		

		}
		return true;
	}

	bool operator > (const VSShaderKey & Key1,const VSShaderKey & Key2)
	{

		//??????????
		if (Key1.m_KeyMap.GetNum() > Key2.m_KeyMap.GetNum())
		{
			return 1;
		}
		else if(Key1.m_KeyMap.GetNum() < Key2.m_KeyMap.GetNum())
		{
			return 0;
		}

		//????????????key??????
		for (uint32 i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<VSUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<VSUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Key > Element2.Key)
			{
				return 1;

			}
			else if (Element1.Key < Element2.Key)
			{
				return 0;
			}			
		}

		//??value??????
		for (uint32 i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<VSUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<VSUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Value > Element2.Value)
			{
				return 1;

			}
			else if (Element1.Value < Element2.Value)
			{
				return 0;
			}			
		}
		return 0;
	}

	bool operator < (const VSShaderKey & Key1,const VSShaderKey & Key2)
	{

		//??????????
		if (Key1.m_KeyMap.GetNum() > Key2.m_KeyMap.GetNum())
		{
			return 0;
		}
		else if(Key1.m_KeyMap.GetNum() < Key2.m_KeyMap.GetNum())
		{
			return 1;
		}

		//????????????key??????
		for (uint32 i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<VSUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<VSUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Key > Element2.Key)
			{
				return 0;

			}
			else if (Element1.Key < Element2.Key)
			{
				return 1;
			}			
		}

		//??value??????
		for (uint32 i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<VSUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<VSUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Value > Element2.Value)
			{
				return 0;

			}
			else if (Element1.Value < Element2.Value)
			{
				return 1;
			}			
		}
		return 0;
	}

}