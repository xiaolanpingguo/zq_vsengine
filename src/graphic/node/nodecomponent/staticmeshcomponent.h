#pragma once


#include "graphic/node/nodecomponent/meshcomponent.h"


namespace zq{


DECLARE_Ptr(VSStaticMeshNode);
DECLARE_Proxy(VSStaticMeshNode);
class GRAPHIC_API VSStaticMeshComponent : public VSMeshComponent
{
    DECLARE_RTTI;
    DECLARE_INITIAL
public:
    VSStaticMeshComponent();
    virtual ~VSStaticMeshComponent();
    void SetStaticMeshResource(VSStaticMeshNodeR * pStaticMeshResource);
    VSStaticMeshNode * GetStaticMeshNode();
    virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data = NULL);
    virtual void PostCreate();
    virtual bool BeforeSaveWithData(void* pData = NULL);
    virtual bool PostLoad(VSStream* pStream);

protected:
    VSStaticMeshNodeRPtr m_pStaticMeshResource;
};


DECLARE_Ptr(VSStaticMeshComponent);
VSTYPE_MARCO(VSStaticMeshComponent);


}
