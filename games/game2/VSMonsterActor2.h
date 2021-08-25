#pragma once


#include "graphic/world/actor/actor.h"
#include "graphic/node/nodecomponent/camera.h"
#include "graphic/node/nodecomponent/skeletonmeshcomponent.h"
#include "graphic/render/scenemanager/viewfamily.h"


namespace zq
{
	class VSMonsterActor2 : public VSActor
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSMonsterActor2();
		~VSMonsterActor2();
		virtual void CreateDefaultComponentNode();
		virtual void ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z);
		void AddCanSeeSence(VSArray<VSString> & SceneMapName);
		virtual void Update(double dAppTime);
	protected:
		VSSkeletonMeshComponent *	m_pMonsterMC;
		VSCamera *					m_pThirdCamera;
		VSWindowViewFamily	*		m_pViewFamily;
		float						m_fMoveSpeed;
		bool m_bLMouseDowning;
		int		m_iDetMouseX;

		int		m_iLastMouseX;
		float						m_RotYDelta;
		void ChangeAnimState();
		float						m_AttackTime;
		bool						m_bAttack;
		float						m_fAttackStartTime;
	};
	DECLARE_Ptr(VSMonsterActor2);
	VSTYPE_MARCO(VSMonsterActor2);
}
