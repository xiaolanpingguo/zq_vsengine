#pragma once


#include "graphic/core/object.h"
#include "math/vector3.h"
#include "math/quat.h"

#include "graphic/world/actor/actor.h"
#include "graphic/world/scenemap.h"
#include "graphic/world/actor/cameraactor.h"


namespace zq{


class VSStream;
class VSLightActor;
class GRAPHIC_API VSWorld : public VSObject
{
	//PRIORITY
		
	//RTTI
	DECLARE_RTTI;
public:
	VSWorld();
	virtual ~VSWorld();

	DECLARE_INITIAL
	static VSWorld * ms_pWorld;

protected:
	VSArray<VSSceneMapPtr>	m_SceneArray;
	VSArray<VSActor *>	m_ActorArray;
	VSArray<VSMessage> m_MessageArray;

public:
		
		
	VSSceneMap* CreateScene(const char * pName);

	VSSceneMap * GetScene(const VSUsedName & Name);
		
	VSActor * CreateActor(const char *  ActorPath, const Vector3 & Pos = Vector3::ms_Zero, const Matrix3X3 & Rotate = Matrix3X3::ms_Identity, const Vector3 &Scale = Vector3::ms_One, VSSceneMap * pSceneMap = NULL);
	void DestroyActor(VSActor * pActor);

	template<typename T>
	VSActor *CreateActor(const Vector3 & Pos = Vector3::ms_Zero, const Matrix3X3 & Rotate = Matrix3X3::ms_Identity, const Vector3 &Scale = Vector3::ms_One, VSSceneMap * pSceneMap = NULL);

	bool LoadMap(const char * MapPath,bool IsAsyn);
	bool UnLoadMap(const char * MapPath);
	void AttachCaptureViewFamilyToCamera(VSCameraActor * pCameraActor,
		uint32 uiCaptureViewFamilyType,const VSString & ViewFamilyName,
		uint32 uiWidth,uint32 uiHeight,
		VSArray<VSString> & SceneMapName,
		const char * RenderMethodRTTIName,
		VSPostEffectSetR *  pPostEffectSet = NULL);
	void AttachWindowViewFamilyToCamera(VSCameraActor * pCameraActor,
		uint32 uiWindowViewFamilyType,const VSString & ViewFamilyName,
		VSArray<VSString> & SceneMapName,
		const char * RenderMethodRTTIName,
		int32 iWindowID = -1,
		VSPostEffectSetR *  pPostEffectSet = NULL);
	VSActor *GetActor(VSUsedName Name);
	void Update(double dAppTime);
	void AddMessage(const VSMessage & Message);
	void UpdateLightActorScene(VSLightActor * pActor);
	void ProcessInput(uint32 uiInputType, uint32 uiEvent, uint32 uiKey, int32 x, int32 y, int32 z);
		
protected:
	friend class VSResourceManager;
	void AddActor(VSActor * pActor);
	void DeleteActor(VSActor * pActor);
	virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy,void * Data = NULL);
	void DestroyScene(const VSUsedName & Name);
	void AddSceneMap(VSSceneMap * pSceneMap);
};
DECLARE_Ptr(VSWorld);
VSTYPE_MARCO(VSWorld);
template<typename T>
VSActor *VSWorld::CreateActor(const Vector3 & Pos, const Matrix3X3 & Rotate, const Vector3 &Scale, VSSceneMap * pSceneMap)
{
	if (!T::ms_Type.IsDerived(VSActor::ms_Type))
		return NULL;
	VSActor * pActor = NULL;
	pActor = (VSActor*)VSObject::GetInstance<T>();
	pActor->CreateDefaultComponentNode();
	pActor->SetWorldPos(Pos);
	pActor->SetWorldRotate(Rotate);
	pActor->SetWorldScale(Scale);
	if (pSceneMap)
	{
		pSceneMap->AddActor(pActor);
	}
	else
	{
		m_SceneArray[0]->AddActor(pActor);
	}
	AddActor(pActor);
	return pActor;
}

}
