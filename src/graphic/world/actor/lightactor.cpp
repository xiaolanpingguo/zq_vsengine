#include "graphic/world/actor/lightactor.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/node/nodecomponent/light/directionlight.h"
#include "graphic/node/nodecomponent/light/pointlight.h"
#include "graphic/node/nodecomponent/light/spotlight.h"
#include "graphic/node/nodecomponent/light/skylight.h"
#include "graphic/world/world.h"


using namespace zq;

IMPLEMENT_RTTI_NoCreateFun(VSLightActor,VSActor)
BEGIN_ADD_PROPERTY(VSLightActor,VSActor)
REGISTER_PROPERTY(m_ShadowSceneNameArray, ShadowSceneNameArray, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSLightActor)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSLightActor::VSLightActor()
{

}
VSLightActor::~VSLightActor()
{

}
void VSLightActor::Update(double dAppTime)
{
	if (m_ShadowSceneNameArray.GetNum() && ((VSLight *)(m_pNode.GetObject()))->GetLightType() != VSLight::LT_SKY)
	{
		VSLocalLight * pLocalLight = (VSLocalLight *)(m_pNode.GetObject());
		pLocalLight->ClearAllShadowScene();
		for (uint32 i = 0; i < m_ShadowSceneNameArray.GetNum(); i++)
		{
			VSSceneMap *pSceneMap = VSWorld::ms_pWorld->GetScene(m_ShadowSceneNameArray[i]);
			pLocalLight->AddShadowScene(pSceneMap->GetScene());
		}
	}
}
void VSLightActor::AddToSceneMap(VSSceneMap * pSceneMap)
{
	VSActor::AddToSceneMap(pSceneMap);
	if (pSceneMap)
	{
		m_ShadowSceneNameArray.AddElement(m_pSceneMap->m_Name);
	}
	else
	{
		m_ShadowSceneNameArray.Clear();
	}
}
IMPLEMENT_RTTI(VSDirectionLightActor,VSLightActor)
BEGIN_ADD_PROPERTY(VSDirectionLightActor,VSLightActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDirectionLightActor)
IMPLEMENT_INITIAL_END
VSDirectionLightActor::VSDirectionLightActor()
{
	
}
VSDirectionLightActor::~VSDirectionLightActor()
{

}
void VSDirectionLightActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponent<VSDirectionLight>();
}

IMPLEMENT_RTTI(VSPointLightActor,VSLightActor)
BEGIN_ADD_PROPERTY(VSPointLightActor,VSLightActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPointLightActor)
IMPLEMENT_INITIAL_END
VSPointLightActor::VSPointLightActor()
{

}
VSPointLightActor::~VSPointLightActor()
{

}
void VSPointLightActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponent<VSPointLight>();
}
IMPLEMENT_RTTI(VSSpotLightActor,VSLightActor)
BEGIN_ADD_PROPERTY(VSSpotLightActor,VSLightActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSpotLightActor)
IMPLEMENT_INITIAL_END
VSSpotLightActor::VSSpotLightActor()
{

}
VSSpotLightActor::~VSSpotLightActor()
{

}
void VSSpotLightActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponent<VSSpotLight>();
}
IMPLEMENT_RTTI(VSSkyLightActor,VSLightActor)
BEGIN_ADD_PROPERTY(VSSkyLightActor,VSLightActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSkyLightActor)
IMPLEMENT_INITIAL_END
VSSkyLightActor::VSSkyLightActor()
{

}
VSSkyLightActor::~VSSkyLightActor()
{

}
void VSSkyLightActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponent<VSSkyLight>();
}

