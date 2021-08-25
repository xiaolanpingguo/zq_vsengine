#pragma once


#include "graphic/node/nodecomponent/meshcomponent.h"


namespace zq{


DECLARE_Proxy(VSSkeletonMeshNode);
DECLARE_Ptr(VSSkeletonMeshNode);
class VSAnimAtom;
class GRAPHIC_API VSSkeletonMeshComponent : public VSMeshComponent
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	VSSkeletonMeshComponent();
	virtual ~VSSkeletonMeshComponent();
	void SetSkeletonMeshResource(VSSkeletonMeshNodeR * pSkeletonMeshResource);
	VSSkeletonMeshNode * GetSkeletonMeshNode();
	virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data = NULL);
	virtual void PostCreate();
	void SetIsDrawSkeleton(bool bIsDrawSkeleton);
		
	virtual bool BeforeSaveWithUserData(void* pData = NULL);
	bool PlayAnim(const VSString & AnimName, float fRatio = 1.0f, uint32 uiRepeatType = VSController::RT_NONE);
	VSSocketNode * GetSocketNode(const VSUsedName & SocketName);
	void SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara);
	virtual bool PostLoad(VSStream* pStream);
	const VSAnimAtom &GetRootDelta();
protected:
	VSSkeletonMeshNodeRPtr m_pSkeletonMeshResource;
};

DECLARE_Ptr(VSSkeletonMeshComponent);
VSTYPE_MARCO(VSSkeletonMeshComponent);
}
