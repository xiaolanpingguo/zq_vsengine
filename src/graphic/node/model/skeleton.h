#pragma once
#include "graphic/node/node.h"
#include "graphic/node/geometry.h"
namespace zq
{
	DECLARE_Ptr(VSBoneNode);
	class VSStream;
	class GRAPHIC_API VSSkeleton : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSkeleton();
		virtual ~VSSkeleton();

	protected:
		VSArray<VSBoneNode *> m_pBoneArray;

		void LinkBoneArray();
		virtual void UpdateNoChild(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);
		static float ms_fBoneAxisLength;
		void Draw(VSCamera * pCamera);
		AABB	m_LocalBV;
		AABB m_OriginLocalBV;
	public:
		friend class VSSkeletonMeshNode;
		virtual void CreateLocalAABB();
		inline void SetLocalBV(const AABB & BV)
		{
			m_LocalBV = BV;
		}
		inline AABB GetLocalBV()const
		{
			return m_LocalBV;
		}
		void CreateBoneArray();
		
		

		uint32 GetBoneNum()const;
		VSBoneNode *GetBoneNode(const VSUsedName & Name)const;
		VSBoneNode * GetBoneNode(uint32 i)const;

		int32 GetBoneIndex(const VSUsedName &Name)const;
		bool m_bIsDrawSkeleton;
	

		virtual void UpdateView(VSCuller & Culler,double dAppTime);
		const VSTransform &GetRootTransform();
	};
	DECLARE_Ptr(VSSkeleton);
	VSTYPE_MARCO(VSSkeleton);
}
