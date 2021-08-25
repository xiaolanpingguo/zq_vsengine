#pragma once
#include "graphic/node/model/modelmeshnode.h"
#include "graphic/node/model/skeleton.h"
#include "graphic/controller/animtree/animset.h"
#include "graphic/controller/animtree/animsequencefunc.h"
#include "graphic/node/socketnode.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSSkeletonMeshNode : public VSModelMeshNode, public VSResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSkeletonMeshNode();
		virtual ~VSSkeletonMeshNode();
	protected:
		VSSkeletonPtr m_pSkeleton;
		VSAnimSetPtr   m_pAnimSet;
		VSAnimSequenceFuncPtr m_pAnimSequence;

	public:
		void SetSkeleton(VSSkeleton * pSkeleton);
		void SetAnimSet(VSAnimSet * pAnimSet);
		void SetAnimTree(VSAnimTreeR * pAnimTree);
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data = NULL);
		inline VSSkeleton * GetSkeleton()const
		{
			return m_pSkeleton;
		}
		inline VSAnimSet * GetAnimSet()const
		{
			return m_pAnimSet;
		}
		inline void SetIsDrawSkeleton(bool bIsDrawSkeleton)
		{
			if (m_pSkeleton)
			{
				m_pSkeleton->m_bIsDrawSkeleton = bIsDrawSkeleton;
			}
		}

		inline bool IsDrawSkeleton()const
		{
			if (m_pSkeleton)
			{
				return m_pSkeleton->m_bIsDrawSkeleton;
			}
			return 0;
		}
		virtual void UpdateController(double dAppTime);
		//can not use with anim tree
		bool PlayAnim(const VSString & AnimName,float fRatio = 1.0f,uint32 uiRepeatType = VSController::RT_NONE);
		void StopAnim();

		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void UpdateAll(double dAppTime);
		VSSocketNode * CreateSocket(const VSUsedName & BoneName,const VSUsedName &SocketName);
		void DeleteSocket(const VSUsedName &SocketName);
		VSSocketNode * GetSocket(const VSUsedName &SocketName);
		void SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara);
		inline const VSAnimTree * GetAnimTree()const
		{
			return m_pAnimTreeInstance;
		}
	protected:
		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);
	protected:
		VSAnimTreeRPtr m_pAnimTree;
		
		VSAnimTreePtr m_pAnimTreeInstance;
		
		void UpdateLocalAABB();
		VSArray<VSSocketNodePtr> m_pSocketArray;
		void UpdateInstanceTexture();
		void CreateAnimInstanceData();
		VSArray<Vector3W> m_AnimData;
		VSArray<HalfVector3W> m_HalfAnimData;
		uint32 m_uiAnimInstanceTextureHeight;
		uint32 m_uiAnimInstanceTextureWidth;
		VSArray<unsigned int> m_uiKeyFrameLengthArrray;
		VSTexAllStatePtr m_AnimInstanceTexture;
	public:
		DECLARE_RESOURCE(VSSkeletonMeshNode, RA_ASYN_LOAD | RA_ENABLE_GC, RT_SKELECTON_MODEL,SKMODEL, Resource/SkelectionMesh, VSCacheResource)
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		const VSAnimAtom &GetRootDelta();
		virtual bool SetDrawInstance(bool bDrawInstance);
		VSArray<ANIM_INSTANCE_SHADER_DATA> m_AnimInstanceData;
		inline VSTexAllState * GetAnimInstanceTexture()const
		{
			return m_AnimInstanceTexture;
		}
	};
	DECLARE_Ptr(VSSkeletonMeshNode);
	VSTYPE_MARCO(VSSkeletonMeshNode);
}
