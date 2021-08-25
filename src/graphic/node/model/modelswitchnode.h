#pragma once
#include "graphic/node/switchnode.h"
#include "graphic/node/geometrynode.h"
namespace zq
{
	class GRAPHIC_API VSModelSwitchNode : public VSSwitchNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSModelSwitchNode();
		virtual ~VSModelSwitchNode();
	public:
		virtual VSGeometryNode * GetGeometryNode(uint32 uiLodLevel);
		virtual void UpdateView(VSCuller & Culler, double dAppTime);
		void SetUseMaterialInstance(uint32 SubMeshID, uint32 UsedID);
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	};
	DECLARE_Ptr(VSModelSwitchNode);
	VSTYPE_MARCO(VSModelSwitchNode);

}
