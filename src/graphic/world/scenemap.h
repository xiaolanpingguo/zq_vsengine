#pragma once
#include "graphic/render/scenemanager/scene.h"
#include "graphic/world/actor/actor.h"
namespace zq
{
	class GRAPHIC_API VSSceneMap : public VSObject, public VSResource
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSceneMap(const char * pName);
		VSSceneMap();
		virtual ~VSSceneMap();
		void AddActor(VSActor * pActor);
		VSActor *GetActor(VSUsedName Name);
		void DeleteActor(VSActor * pActor);
		VSUsedName m_Name;
		friend class VSViewFamliyInfo;
		friend class VSWorld;
		friend class VSReflectCubLightActor;
		virtual bool PostLoad(VSStream* pStream);
		VSScene *GetScene()
		{
			return m_pScene;
		}
		virtual void OnDestroy();
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		DECLARE_RESOURCE(VSSceneMap, RA_ASYN_LOAD, RT_MAP,MAP, Resource/VSMap, VSCacheResource)
	protected:

		VSScenePtr m_pScene;
		VSArray<VSActorPtr> m_pActorArray;

	};
	DECLARE_Ptr(VSSceneMap);
	VSTYPE_MARCO(VSSceneMap);
	DECLARE_Proxy(VSSceneMap);
}
